/*
 * @Descripttion: 
 * @version: 
 * @Author: sueRimn
 * @Date: 2021-09-30 10:54:03
 * @LastEditors: sueRimn
 * @LastEditTime: 2021-09-30 15:47:46
 */
#ifndef _PRIMO_SINGLETON_H_
#define _PRIMO_SINGLETON_H_

#include <memory>
namespace primo
{

template <typename T, typename X = void, int N = 0>
class Singleton
{
public:
    //typedef std::shared_ptr<T> ptr;
    static T* GetInstance()
    {
        static T instance;
        return &instance;
    }
};


template <typename T, typename X = void, int N = 0>
class SingletonPtr
{
public:
    //typedef std::shared_ptr<T> ptr;
    static std::shared_ptr<T> GetInstance()
    {
        static std::shared_ptr<T> instance(new T());
        return instance;
    }
};

}
#endif