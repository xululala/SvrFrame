/*
 * @Descripttion: 日志系统自测
 * @version: 
 * @Author: primoxu
 * @Date: 2021-09-25 22:41:53
 * @LastEditors: sueRimn
 * @LastEditTime: 2021-09-30 15:50:25
 */
#include "../src/log/Logger.h"


int main(int argc, char* argv[])
{
     
    //primo::Logger::ptr logger(new primo::Logger());
    //logger->AddAppender(primo::FileAppender::ptr(new primo::FileAppender("LogTest.txt")));
    
    //logger->AddAppender(primo::StdOutAppender::ptr(new primo::StdOutAppender()));
    //primo::LogEvent::ptr event(new primo::LogEvent(logger, primo::LOG_LEVEL::DEBUG, "filename", 32, 2, 45550, 88, 9999, "test log thread"));

    //logger->Log(primo::LOG_LEVEL::DEBUG, event);
    //P_LOG_INFO(logger) << "test macro log";
    //P_LOGF_INFO(logger, "%s %d", "test", 5);

    primo::Logger::ptr logger2(primo::LOGMGR::GetInstance()->GetLogger("root"));
    std::cout << logger2->GetName() << std::endl;
    //logger2->AddAppender(primo::FileAppender::ptr(new primo::FileAppender("LogTest2.txt")));
    P_LOGF_INFO(logger2, "test logmgr info %s, %d", logger2->GetName().c_str(), 1030);
    //std::cout << " Test log system accomplished! " << std::endl;
    return 0;
}

