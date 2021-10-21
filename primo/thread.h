#ifndef _PRIMO_THREAD_H_
#define _PRIMO_THREAD_H_

#include <thread>
#include <pthread.h>
#include <functional>
#include <memory>
#include <semaphore.h>

namespace primo
{

class Semaphore
{
public:
    Semaphore(uint32_t count = 0);

    int notify();
    int wait();

private:
    Semaphore(const Semaphore& ) = delete;
    Semaphore(const Semaphore&& ) = delete;
    Semaphore& operator=(const Semaphore& ) = delete;

private:

    uint32_t mCount;
    sem_t mSemaphore;
};

template <class T>
struct ScopeLockImpl
{
public:
    ScopeLockImpl(T& mutex) : mMutex(mutex)
    {
        mMutex.lock();
        mLocked = true;
    }

    ~ScopeLockImpl()
    {
        unlock();
    }

    void lock()
    {
        if (!mLocked)
        {
            mMutex.lock();
            mLocked = true;
        }
    }

    void unlock()
    {
        if (mLocked)
        {
            mMutex.unlock();
            mLocked = false;
        }
    }

private:
    T& mMutex;
    bool mLocked;
};

template <class T>
struct ReadScopeLockImpl
{
public:
    ReadScopeLockImpl(T& mutex) : mMutex(mutex)
    {
        mMutex.rdlock();
        mLocked = true;
    }

    ~ReadScopeLockImpl()
    {
        unlock();
    }

    void lock()
    {
        if (!mLocked)
        {
            mMutex.rdlock();
            mLocked = true;
        }
    }

    void unlock()
    {
        if (mLocked)
        {
            mMutex.unlock();
            mLocked = false;
        }
    }

private:
    T& mMutex;
    bool mLocked;
};

template <class T>
struct WriteScopeLockImpl
{
public:
    WriteScopeLockImpl(T& mutex) : mMutex(mutex)
    {
        mMutex.wrlock();
        mLocked = true;
    }

    ~WriteScopeLockImpl()
    {
        unlock();
    }

    void lock()
    {
        if (!mLocked)
        {
            mMutex.wrlock();
            mLocked = true;
        }
    }

    void unlock()
    {
        if (mLocked)
        {
            mMutex.unlock();
            mLocked = false;
        }
    }

private:
    T& mMutex;
    bool mLocked;
};

class Mutex
{
public:
    typedef ScopeLockImpl<Mutex> Lock;
    Mutex()
    {
        pthread_mutex_init(&mMutex, nullptr);
    }

    ~Mutex()
    {
        unlock();
    }

    void unlock()
    {
        pthread_mutex_unlock(&mMutex);
    }

    void lock()
    {
        pthread_mutex_lock(&mMutex);
    }

private:
    pthread_mutex_t mMutex;
};

class RWMutex
{
public:

    typedef ReadScopeLockImpl<RWMutex> ReadLock;
    typedef WriteScopeLockImpl<RWMutex> WriteLock;
    RWMutex()
    {
        pthread_rwlock_init(&mMutex, nullptr);
    }

    void rdlock()
    {
        pthread_rwlock_rdlock(&mMutex);
    }

    void wrlock()
    {
        pthread_rwlock_wrlock(&mMutex);
    }

    void unlock()
    {
        pthread_rwlock_unlock(&mMutex);
    }

    ~RWMutex()
    {
        pthread_rwlock_unlock(&mMutex);
    }

private:
    pthread_rwlock_t mMutex;
};



class Thread
{
public:
    typedef std::shared_ptr<Thread> ptr;
    Thread (std::function<void()> cb, const std::string& name);
    ~Thread();

    pid_t GetId() const {return mId;}
    const std::string& GetName () const {return mName;}
    void join();

    static Thread* GetThis();
    static void SetName(const std::string& name);
    static const std::string& GetStaticThreadName();
private:
    Thread (const Thread&) = delete;
    Thread (const Thread&&) = delete;
    Thread& operator= (const Thread&) = delete;

    static void* run (void* arg);
private:
    pid_t mId = -1;
    pthread_t mThread = 0;
    std::function<void()> mCb;
    std::string mName;
    Semaphore mSemaphore;
};

}


#endif