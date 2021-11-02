/*
 * @Descripttion: 
 * @version: 
 * @Author: sueRimn
 * @Date: 2021-10-27 22:31:06
 * @LastEditors: sueRimn
 * @LastEditTime: 2021-11-02 20:22:57
 */
#include "fiber.h"
#include <atomic>
//#include "schedule.h"
namespace primo
{

static Logger::ptr g_logger = P_LOG_NAME("system");

static std::atomic<uint64_t> sFiberId {0};
static std::atomic<uint64_t> sFiberCount{0};

//thread_local修饰的变量具有线程的生命周期，
//每个线程都有一份，并且具有static属性，
//因为对线程使用shared指针有线程安全问题，所以这里使用裸指针
static thread_local Fiber* t_fiber = nullptr;
static thread_local Fiber::ptr t_threadFiber = nullptr;

static CfgVar<uint32_t>::ptr gFiberStackSize = Config::LookUp<uint32_t>("fiber stack size", 128 * 1024, "fiber stack size");

class MallocStackAllocator 
{
public:
    static void* Alloc(size_t size) 
    {
        return malloc(size);
    }

    static void Dealloc(void* vp, size_t size) 
    {
        return free(vp);
    }
};

using StackAllocator = MallocStackAllocator;

Fiber::Fiber(std::function<void()> cb, size_t stacksize, bool usecaller)
    : mId (++sFiberId)
    , mCallback (cb)
{
    ++sFiberCount;
    mStackSize = stacksize ? stacksize : gFiberStackSize->GetValue();
    pStack = StackAllocator::Alloc(mStackSize);

    if (getcontext(&mContex))
    {
        P_ASSERT2(false, "get context error");
    }
    mContex.uc_stack.ss_sp = pStack;
    mContex.uc_stack.ss_size = mStackSize;
    mContex.uc_link = nullptr;

    if(!use_caller) 
    {
        makecontext(&mContex, &Fiber::MainFunc, 0);
    } 
    else 
    {
        makecontext(&mContex, &Fiber::CallerMainFunc, 0);
    }

    P_LOG_DEBUG(g_logger) << "Fiber::Fiber id=" << mId;
}

Fiber::Fiber()
{
    mState = EXEC;
    SetThis(this);

    if(getcontext(&mContex)) 
    {
        P_ASSERT2(false, "getcontext");
    }

    ++sFiberCount;

    P_LOG_DEBUG(g_logger) << "Fiber::Fiber main";
}

Fiber::~Fiber()
{
    --sFiberCount;
    if (pStack)
    {
        P_ASSERT(mState == INIT 
                || mState == EXCEPT 
                || mState == TERM);
    }
    else
    {
        P_ASSERT(!mCallback);
        P_ASSERT(mState == EXEC);
        
        Fiber* cur = t_fiber;
        if (cur == this)
        {
            SetThis(nullptr);
        }
    }
    P_LOG_DEBUG(g_logger) << "Fiber::~Fiber id=" << mId
                              << " total=" << sFiberCount;
}

void Fiber::Reset(std::function<void()> cb)
{
    P_ASSERT(pStack);
    P_ASSERT(mState == TERM
            || mState == EXCEPT
            || mState == INIT);
    mCallback = cb;

    if (getcontext(&mContex)) 
    {
        P_ASSERT2(false, "getcontext");
    }

    mContex.uc_link = nullptr;
    mContex.uc_stack.ss_sp = pStack;
    mContex.uc_stack.ss_size = mStackSize;

    //把上下文结构体下一条要执行的指令改变为MainFunc函数的地址。
    makecontext(&mContex, &Fiber::MainFunc, 0);
    mState = INIT;
}

void Fiber::Call()
{
    SetThis(this);
    mState = EXEC;
    if (swapcontext(&t_threadFiber->mContex, &mContex))
    {
        P_ASSERT2(false, "swapcontext");
    }
}


void Fiber::Back()
{
    SetThis(t_threadFiber.get());
    if (swapcontext(&mContex, &t_threadFiber->mContex))
    {
        P_ASSERT2(false, "swapcontext");
    }
}

//切换到后台执行
void Fiber::SwapOut()
{
   SetThis(t_threadFiber.get()); 
   if (swapcontext(&mContex, &t_threadFiber->mContex))
   {
       P_ASSERT2(false, "swapcontext");
   }
}

//切换到当前协程执行
void Fiber::SwapIn()
{
    SetThis(this);
    P_ASSERT(mState != EXEC);
    mState = EXEC;
    if(swapcontext( &t_threadFiber->mContex, &mContex))
    {
        P_ASSERT2(false, "swapcontext");
    }
}

//设置当前协程
void Fiber::SetThis(Fiber* ptr)
{
    t_fiber = ptr;
}

//返回当前协程
Fiber::ptr GetThis()
{
    if (t_fiber)
    {
        return t_fiber->shared_from_this();
    }
    Fiber::ptr main_fiber(new Fiber);
    P_ASSERT(main_fiber.get() == t_fiber);
    t_threadFiber = main_fiber;
    return t_fiber->shared_from_this();
}

//切换到后台，并设置为Ready状态
void Fiber::YieldToReady()
{
    Fiber:ptr cur = GetThis();
    P_ASSERT(cur->mState == EXEC);

    cur->mState = READY;
    cur->SwapOut();
}

//切换到后台，并设置为Hold状态
void Fiber::YieldToHold()
{
    Fiber:ptr cur = GetThis();
    P_ASSERT(cur->mState == EXEC);

    //cur->mState = HOLD;
    cur->SwapOut();
}

Fiber::State Fiber::GetState() const
{
    return mState;
}

uint64_t Fiber::TotalFibers()
{
    return sFiberCount;
}

void Fiber::MainFunc() 
{
    Fiber::ptr cur = GetThis();
    P_ASSERT(cur);
    try 
    {
        cur->mCallback();
        cur->mCallback = nullptr;
        cur->mState = TERM;
    } 
    catch (std::exception& ex) 
    {
        cur->mState = EXCEPT;
        P_LOG_ERROR(g_logger) << "Fiber Except: " << ex.what()
            << " fiber_id=" << cur->GetId()
            << std::endl
            << primo::BacktraceToString();
    } 
    catch (...) 
    {
        cur->mState = EXCEPT;
        P_LOG_ERROR(g_logger) << "Fiber Except"
            << " fiber_id=" << cur->GetId()
            << std::endl
            << primo::BacktraceToString();
    }

    auto raw_ptr = cur.get();
    cur.reset();
    raw_ptr->SwapOut();

    P_ASSERT2(false, "never reach fiber_id=" + std::to_string(raw_ptr->GetId()));
}

void Fiber::CallerMainFunc() 
{
    Fiber::ptr cur = GetThis();
    P_ASSERT(cur);
    try 
    {
        cur->mCallback();
        cur->mCallback = nullptr;
        cur->mState = TERM;
    } 
    catch (std::exception& ex) 
    {
        cur->mState = EXCEPT;
        P_LOG_ERROR(g_logger) << "Fiber Except: " << ex.what()
            << " fiber_id=" << cur->GetId()
            << std::endl
            << primo::BacktraceToString();
    } 
    catch (...) 
    {
        cur->mState = EXCEPT;
        P_LOG_ERROR(g_logger) << "Fiber Except"
            << " fiber_id=" << cur->GetId()
            << std::endl
            << primo::BacktraceToString();
    }

    auto raw_ptr = cur.get();
    cur.reset();
    raw_ptr->Back();
    P_ASSERT2(false, "never reach fiber_id=" + std::to_string(raw_ptr->GetId()));

}

}