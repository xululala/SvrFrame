/*
 * @Descripttion: 日志系统自测
 * @version: 
 * @Author: primoxu
 * @Date: 2021-09-25 22:41:53
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2021-09-26 17:34:10
 */
#include "../src/log/Logger.h"

int main(int argc, char* argv[])
{
     
    primo::Logger::ptr logger(new primo::Logger());
    
    logger->AddAppender(primo::StdOutAppender::ptr(new primo::StdOutAppender()));
    primo::LogEvent::ptr event(new primo::LogEvent(logger, primo::LOG_LEVEL::DEBUG, "filename", 32, 2, 45550, 88, 9999, "test log thread"));

    logger->Log(primo::LOG_LEVEL::DEBUG, event);
    std::cout << " Test log system accomplished! " << std::endl;
    return 0;
}

