#include "thread.h"
#include "log/Logger.h"

namespace primo
{

static thread_local Thread* t_thread = nullptr;
static thread_local std::string t_thread_name = "UNKNOW";

static primo::Logger::ptr g_logger = P_LOG_NAME("system");

Semaphore::Semaphore(uint32_t count) 
    : mCount(count)
{
    int iRet = sem_init(&mSemaphore, 0, mCount);
    if (iRet)
    {
        P_LOG_ERROR(g_logger) << " sem_init faild";
    }
}

int Semaphore::notify()
{
    int iRet = sem_post(&mSemaphore);
    if (iRet)
    {
        throw std::logic_error("sem_post error");
    }
}

int Semaphore::wait()
{
    int iRet = sem_wait(&mSemaphore);
    if (iRet)
    {
        throw std::logic_error("sem_wait error");
    }
}

Thread::Thread(std::function<void()> cb, const std::string& name)
    : mCb(cb),
      mName(name)
{
    if (name.empty())
    {
        mName = "UNKNOW";
    }
    int iRet = pthread_create(&mThread, nullptr, &Thread::run, this);
    
    if (iRet)
    {
        P_LOG_ERROR(g_logger) << " pthread_create fail, iRet = " << iRet
            <<" name = " << mName;
        throw std::logic_error("pthread_create error");
    }
    mSemaphore.wait();
}

Thread* Thread::GetThis()
{
    return t_thread;
}

void Thread::SetName(const std::string& name)
{
    if (t_thread)
    {
        t_thread->mName = name;
    }
    t_thread_name = name;
}

const std::string& Thread::GetStaticThreadName()
{
    return t_thread->mName;
}

void* Thread::run(void* arg)
{
    Thread* thread = (Thread*)arg;
    t_thread = thread;
    thread->mId = primo::GetThreadId();
    pthread_setname_np(pthread_self(), thread->mName.substr(0, 15).c_str());

    std::function<void()> cb;
    cb.swap(thread->mCb);

    thread->mSemaphore.notify();
    try
    {
        cb();
    }
    catch(const std::exception& e)
    {
        P_LOG_ERROR(P_LOG_ROOT()) << "run function failed, e.what: " << e.what() << '\n';
    }
    return 0;
}

Thread::~Thread()
{
    if (mThread)
    {
        pthread_detach(mThread);
    }
}

void Thread::join()
{
    if (mThread)
    {
        int iRet = pthread_join(mThread, nullptr);
        if (iRet)
        {
            P_LOG_ERROR(g_logger) <<"pthread_join thread fail, iRet = " << iRet
                                  << " name = " << mName;
            throw std::logic_error("pthread_join error");
        }
        mThread = 0;
    }
}

}