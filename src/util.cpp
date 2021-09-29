/*
 * @Descripttion: 
 * @version: 
 * @Author: sueRimn
 * @Date: 2021-09-28 11:36:03
 * @LastEditors: sueRimn
 * @LastEditTime: 2021-09-28 17:19:27
 */
#include "util.h"

namespace primo
{

pid_t GetThreadId()
{
    return syscall(SYS_gettid);
}

uint32_t GetFiberId()
{
    return 0;
}

}