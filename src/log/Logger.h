#ifndef LOGGER_H
#define LOGGER_H
#include <string>
#include <memory>
#include <list>
#include <sstream>
#include <ostream>
namespace Primo {

//日志级别枚举
class LOG_LEVEL {
public:
    typedef enum LEVEL {
        UNKNOWN = 0,
        DEBUG = 1,
        INFO= 2,
        WARN = 3,
        ERROR = 4,
        FATAL = 5,
    } LEVEL;
};

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
    ~LogFormatter();
    std::string format(LogEvent::ptr event);
private:

};

//日志输出器
class LogAppender {
public:
    LogAppender();
    typedef std::shared_ptr<LogAppender> ptr;
    virtual void Log(LOG_LEVEL level, LogEvent::ptr) = 0;

    LogFormatter::ptr GetFormatter() const { return mFormatter;};

    void SetFormatter(LogFormatter::ptr formatter) { 
        mFormatter = formatter; 
    };
protected:
    LOG_LEVEL mLevel;
    LogFormatter::ptr mFormatter;
};

//添加日志器Logger类
class Logger {
public:
    typedef std::shared_ptr<Logger> ptr;
    Logger();
    Logger(const std::string& name = "root");
    void Log(LOG_LEVEL level, LogEvent::ptr event);
    void debug(LogEvent::ptr event);
    void info(LogEvent::ptr event);
    void warn(LogEvent::ptr event);
    void error(LogEvent::ptr event);

    void AddAppender(LogAppender::ptr appender);
    void DelAppender(LogAppender::ptr appender);

    inline void SetName(std::string name) { mName = name; };

    inline void SetLevel(LOG_LEVEL level) { mLevel = level; };

    std::string GetName() const { return mName; };

    LOG_LEVEL GetLevel() const {return mLevel;};
private:
    std::string mName;                                     //日志名称
    LOG_LEVEL mLevel;
    std::list<LogAppender::ptr> mAppenderList;
};

//输出到控制台的appender
class StdOutAppender : public LogAppender {
public:
    typedef std::shared_ptr<StdOutAppender> ptr; 
    StdOutAppender();
    void Log(LOG_LEVEL level, LogEvent::ptr);

};


//输出到文件的appender
class FileAppender : public LogAppender {
public:
    typedef std::shared_ptr<FileAppender> ptr; 
    FileAppender(const std::string& name);
    void Log(LOG_LEVEL level, LogEvent::ptr event);

    void SetName(const std::string& name) { mFileName = name; };
    std::string GetName() const { return mFileName;};
    bool ReOpen();       //重新打开文件


private:
    std::string mFileName;
    std::ofstream mFileStream;
};

}
#endif