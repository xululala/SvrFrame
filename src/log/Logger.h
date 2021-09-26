/*
 * @Descripttion: 日志系统
 * @version: 0.1
 * @Author: Primoxu
 * @Date: 2021-08-20
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2021-09-26 17:24:12
 */

#ifndef _PRIMO_LOGGER_H
#define _PRIMO_LOGGER_H
#include <string>
#include <memory>
#include <list>
#include <vector>
#include <sstream>
#include <fstream>
#include <iostream>
#include <map>
#include <functional>
#include <tuple>
namespace primo 
{

class Logger;

//日志级别枚举
class LOG_LEVEL 
{
public:
    enum LEVEL 
    {
        UNKNOW = 0,
        DEBUG = 1,
        INFO= 2,
        WARN = 3,
        ERROR = 4,
        FATAL = 5,
    };

    static const char* ToString(LOG_LEVEL::LEVEL level);
    static LOG_LEVEL::LEVEL FromString(const std::string& str);
};

//日志事件
class LogEvent 
{
public:
    typedef std::shared_ptr<LogEvent> ptr;

     LogEvent(std::shared_ptr<Logger> logger, LOG_LEVEL::LEVEL level
             ,const char* file, int32_t line, uint32_t elapse
             ,uint32_t thread_id, uint32_t fiber_id, uint64_t time
             ,const std::string& thread_name);

    const char* GetFile() const {return mFile;}

    uint32_t GetLine() const {return mLine;}

    uint32_t GetElapse() const {return mElapse;}

    uint32_t GetThreadId() const {return mThreadId;}

    uint32_t GetFiberId() const {return mFiberId;}

    time_t GetTime() const {return mTime;}

    const std::string& GetThreadName() const { return mThreadName;}

    std::string GetContent() const { return m_ss.str();}

    std::shared_ptr<Logger> GetLogger() const { return mLogger;}

    LOG_LEVEL::LEVEL GetLevel() const { return mLevel;}

    //std::string GetContent() const { return mContent;} 
private:
    const char* mFile = nullptr;         //文件名
    int32_t mLine = 0;                   //行号
    uint32_t mElapse = 0;                //程序运行到现在的毫秒数
    uint32_t mThreadId = 0;              //线程ID
    uint32_t mFiberId = 0;               //协程ID
    uint64_t mTime = 0;                  //时间戳
    std::string mThreadName;             //线程名称
    std::stringstream m_ss;              //日志内容流
    std::shared_ptr<Logger> mLogger;     //日志器
    LOG_LEVEL::LEVEL mLevel;             //日志等级
};


//日志格式器
class LogFormatter 
{
public:
    typedef std::shared_ptr<LogFormatter> ptr;
    LogFormatter(const std::string& pattern);
    LogFormatter();
    ~LogFormatter();
    std::string format(std::shared_ptr<Logger> logger, LOG_LEVEL::LEVEL level, LogEvent::ptr event);
public:
    class FormatItem
    {
    public:
        typedef std::shared_ptr<FormatItem> ptr;
        virtual ~FormatItem() {};
        virtual void format(std::ostream& os, std::shared_ptr<Logger> logger, 
                            LOG_LEVEL::LEVEL level, LogEvent::ptr event) = 0; 
    };

    void Init();

    bool IsError() const {return bError;}

    const std::string GetPattern() const {return mPattern;}

private:
    std::string mPattern;
    std::vector<FormatItem::ptr> mItems;
    bool bError = false;
};



//日志输出器
class LogAppender 
{
//friend class Logger;
public:
    //typedef Spinlock MutexType; 
    LogAppender(){};
    virtual ~LogAppender(){};
    
    typedef std::shared_ptr<LogAppender> ptr;
    virtual void Log(std::shared_ptr<Logger> logger, LOG_LEVEL::LEVEL level, LogEvent::ptr) = 0;
    
    //virtual std::string ToYamlString() = 0;

    LogFormatter::ptr GetFormatter() const { return mFormatter;};

    void SetFormatter(LogFormatter::ptr formatter) 
    { 
        mFormatter = formatter; 
    };

    void SetLevel(LOG_LEVEL::LEVEL level)
    {
        mLevel = level;
    }
protected:
    LOG_LEVEL::LEVEL mLevel = LOG_LEVEL::DEBUG;
    bool bHasFormatter = false;
    LogFormatter::ptr mFormatter;
    //MutexType mMutex;
};


//输出到控制台的appender
class StdOutAppender : public LogAppender 
{
public:
    typedef std::shared_ptr<StdOutAppender> ptr; 
    StdOutAppender(){};
    virtual ~StdOutAppender(){};
    void Log(std::shared_ptr<Logger> logger, LOG_LEVEL::LEVEL level, LogEvent::ptr event) override;
    //std::string ToYamlString() override;
};


//输出到文件的appender
class FileAppender : public LogAppender 
{
public:
    typedef std::shared_ptr<FileAppender> ptr;
    FileAppender(){};
    FileAppender(const std::string& name);
    virtual ~FileAppender(){};
    void Log(std::shared_ptr<Logger> logger, LOG_LEVEL::LEVEL level, LogEvent::ptr event) override;
    void SetName(const std::string& name) { mFileName = name; } ;
    std::string GetName() const { return mFileName;};
    //std::string ToYamlString() override;
    std::ofstream& ReOpen();       //重新打开文件
private:
    std::string mFileName;
    std::ofstream mFileStream;
};


//添加日志器Logger类
class Logger : public std::enable_shared_from_this<Logger> 
{
public:
    typedef std::shared_ptr<Logger> ptr;
    //Logger();
    Logger(const std::string& name = "root");
    void Log(LOG_LEVEL::LEVEL level, LogEvent::ptr event);
    void debug(LogEvent::ptr event);
    void info(LogEvent::ptr event);
    void warn(LogEvent::ptr event);
    void error(LogEvent::ptr event);
    void fatal(LogEvent::ptr event);

    void AddAppender(LogAppender::ptr appender);
    void DelAppender(LogAppender::ptr appender);

    inline void SetName(std::string name) { mName = name; };

    inline void SetLevel(LOG_LEVEL::LEVEL level) { mLevel = level; };

    std::string GetName() const { return mName; };

    LOG_LEVEL::LEVEL GetLevel() const {return mLevel;};

    //设置日志格式
    void SetFormatter(LogFormatter::ptr fmt);

    //设置文本日志格式
    void SetFormatter(const std::string& str);

private:
    std::string mName;                         //日志名称
    LOG_LEVEL::LEVEL mLevel;                   //日志级别
    std::list<LogAppender::ptr> mAppenderList;  
    LogFormatter::ptr mFormatter;
    
    //主日志器， 如果当前日志未定义，则使用主日志器输出
    Logger::ptr mRoot;
};





}
#endif