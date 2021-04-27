#ifndef LOGGER_H
#define LOGGER_H
#include<string>
#include <memory>
namespace Primo {

//日志级别枚举
typedef enum LOG_LEVEL {
    INFO = 1,
    DEBUG = 2,
    WARN = 3,
    ERROR = 4,
    FATAL = 5,
} LOG_LEVEL;

//日志事件
class LogEvent {
public:
    typedef std::shared_ptr<LogEvent> ptr;
    LOG_LEVEL mLevel;
    LogEvent();
private:
    const char* mFile = nullptr;    //文件名
    int32_t mLime = 0;              //行号
    int64_t mMs = 0;                //程序运行到现在的毫秒数
    uint32_t mThreadId = 0;         //线程ID
    uint32_t mFiberId = 0;          //协程ID
    time_t mTime = 0;               //时间戳
    std::string mContent;           //目录
};

//日志格式器
class LogFormatter {
public:
    typedef std::shared_ptr<LogFormatter> ptr;
    LogFormatter();
    std::string fomat(LOG_LEVEL level, LogEvent::ptr event_ptr);

private:

};

//日志输出器
class LogAppender {
public:
    LogAppender();


private:

    

};

}

#endif