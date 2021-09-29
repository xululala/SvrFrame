/*
 * @Descripttion: 日志系统自测
 * @version: 
 * @Author: primoxu
 * @Date: 2021-09-25 22:41:53
 * @LastEditors: sueRimn
 * @LastEditTime: 2021-09-28 17:15:29
 */
#include "../src/log/Logger.h"


int main(int argc, char* argv[])
{
     
    primo::Logger::ptr logger(new primo::Logger());
    
    //logger->AddAppender(primo::StdOutAppender::ptr(new primo::StdOutAppender()));
    //primo::LogEvent::ptr event(new primo::LogEvent(logger, primo::LOG_LEVEL::DEBUG, "filename", 32, 2, 45550, 88, 9999, "test log thread"));

    //logger->Log(primo::LOG_LEVEL::DEBUG, event);
    P_LOG_INFO(logger) << "test macro log" << std::endl;

    P_LOGF_INFO(logger, "%s", "test");

    
    std::cout << " Test log system accomplished! " << std::endl;
    return 0;
}

