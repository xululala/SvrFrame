/*
 * @Descripttion: 
 * @version: 
 * @Author: primoxu
 * @Date: 2021-09-28 11:29:35
 * @LastEditors: sueRimn
 * @LastEditTime: 2021-09-28 17:18:47
 */
#ifndef _PRIMO_UTIL_H
#define _PRIMO_UTIL_H
#include <pthread.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/syscall.h>

namespace primo
{

pid_t GetThreadId();


uint32_t GetFiberId();

}
#endif