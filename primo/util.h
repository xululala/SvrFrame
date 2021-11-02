/*
 * @Descripttion: 
 * @version: 
 * @Author: primoxu
 * @Date: 2021-09-28 11:29:35
 * @LastEditors: sueRimn
 * @LastEditTime: 2021-11-02 16:12:11
 */
#ifndef _PRIMO_UTIL_H
#define _PRIMO_UTIL_H
#include <pthread.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <cxxabi.h>
#include <string>
#include <vector>
#include "log/Logger.h"
namespace primo
{

pid_t GetThreadId();


uint32_t GetFiberId();

template<class T>
std::string TypeToName() {
    static const char* s_name = abi::__cxa_demangle(typeid(T).name(), nullptr, nullptr, nullptr);
    return std::string(s_name);
}

void Backtrace(std::vector<std::string>& br, int size, int skip = 1);

std::string BacktraceToString(int size = 64, int skip = 2, const std::string& prefix = "");

}
#endif