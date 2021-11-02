/*
 * @Descripttion: 
 * @version: 
 * @Author: sueRimn
 * @Date: 2021-09-28 11:36:03
 * @LastEditors: sueRimn
 * @LastEditTime: 2021-11-02 16:12:40
 */
#include "util.h"
#include <string>
#include <execinfo.h>

namespace primo
{

primo::Logger::ptr g_logger = P_LOG_NAME("system");

pid_t GetThreadId()
{
    return syscall(SYS_gettid);
}

uint32_t GetFiberId()
{
    return 0;
}

std::string GetThreadName()
{
    return " ";
}

void Backtrace(std::vector<std::string>& strs, int size, int skip)
{
    void** array = (void**)malloc((sizeof(void*)*size));
    int s = backtrace(array, size);
    char** str = backtrace_symbols(array, size);
    if (str == NULL)
    {
        P_LOG_ERROR(g_logger) << "backtrace_symbols error";
        return;
    }

    for (int i = skip; i < s; ++i)
    {
        strs.push_back(str[i]);
    }
    free(array);
    free(str);
}

std::string BacktraceToString(int size, int skip, const std::string& prefix)
{
    std::vector<std::string> strs;
    Backtrace(strs, size, skip);
    std::stringstream ss;
    for(int i = 0; i < strs.size(); ++i)
    {
        ss << strs[i] << std::endl;
    }
    return ss.str();
}

}