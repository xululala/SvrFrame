/*
 * @Descripttion: 
 * @version: 
 * @Author: sueRimn
 * @Date: 2021-10-27 16:14:14
 * @LastEditors: sueRimn
 * @LastEditTime: 2021-11-02 16:09:57
 */
#ifndef __PRIMO_FIBER_H_
#define __PRIMO_FIBER_H_

#include <ucontext.h>
#include "include.h"

namespace primo
{

class Fiber : public std::enable_shared_from_this<Fiber>
{
public:
    typedef std::shared_ptr<Fiber> ptr;
    enum State
    {
        INIT,
        HOLD,
        EXEC,
        TERM,
        READY,
        EXCEPT
    };

    /**
     * @name: 默认构造函数 
     * @msg: 用于构造main主协程
     */    
    Fiber();

    /**
     * @name: 
     * @msg: 
     * @param {function<void()>} cb
     * @param {size_t} stacksize
     * @param {bool} use_caller
     * @return 
     */    
    Fiber(std::function<void()> cb, size_t stacksize = 0, 
        bool use_caller = false);

    ~Fiber();

    void Reset(std::function<void()> cb);

    void SwapIn();

    void SwapOut();

    void Call();

    void Back();

    void SetThis(Fiber* ptr);

    State GetState() const;

    uint64_t GetId() const {return mId;}

    static Fiber::ptr GetThis();

    static void YieldToReady();

    static void YieldToHold();

    static uint64_t TotalFibers();
    
    static void MainFunc();

    static void CallerMainFunc();
    
private:
    uint64_t mId = 0;

    uint32_t mStackSize = 0;

    State mState = INIT;

    ucontext_t mContex;

    void* pStack = nullptr;
    
    std::function<void ()> mCallback;

    bool use_caller;
};

}


#endif