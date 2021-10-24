/*
 * @Descripttion: 日志系统
 * @version: 
 * @Author: primoxu
 * @Date: 2021-08-20 22:25:03
 * @LastEditors: sueRimn
 * @LastEditTime: 2021-10-01 15:55:29
 */

#include "Logger.h"
#include "../config.h"
namespace primo
{

const char* LOG_LEVEL::ToString(LOG_LEVEL::LEVEL level)
{
    switch(level)
    {
    #define XX(name) \
        case LOG_LEVEL::name: \
            return #name; \
            break;

        XX(DEBUG);
        XX(INFO);
        XX(WARN);
        XX(ERROR);
        XX(FATAL);
    #undef XX
        default:
            return "UNKNOW";
    }
    return "UNKNOWN";
}

LOG_LEVEL::LEVEL LOG_LEVEL::FromString(const std::string& str)
{
#define XX(level, v) \
    if(str == #v) { \
        return LOG_LEVEL::level; \
    }
    XX(DEBUG, debug);
    XX(INFO, info);
    XX(WARN, warn);
    XX(ERROR, error);
    XX(FATAL, fatal);

    XX(DEBUG, DEBUG);
    XX(INFO, INFO);
    XX(WARN, WARN);
    XX(ERROR, ERROR);
    XX(FATAL, FATAL);
    return LOG_LEVEL::UNKNOW;
#undef XX 
}

LogEvent::LogEvent(std::shared_ptr<Logger> logger, LOG_LEVEL::LEVEL level
            ,const char* file, int32_t line, uint32_t elapse
            ,uint32_t thread_id, uint32_t fiber_id, uint64_t time
            ,const std::string& thread_name)
    :mFile(file)
    ,mLine(line)
    ,mElapse(elapse)
    ,mThreadId(thread_id)
    ,mFiberId(fiber_id)
    ,mTime(time)
    ,mThreadName(thread_name)
    ,mLogger(logger)
    ,mLevel(level) 
{
}

void LogEvent::format(char* fmt, ...)
{
    va_list arg_list;
    va_start(arg_list, fmt);
    format(fmt, arg_list);
    va_end(arg_list);
}

void LogEvent::format(char* fmt, va_list arg)
{
    char* buf = nullptr;
    int len = vasprintf(&buf, fmt, arg);
    if (len != -1)
    {
        m_ss << std::string(buf, len);
        free(buf);
    }
}

Logger::Logger(const std::string& name) 
    : mName(name)
    , mLevel(LOG_LEVEL::INFO)
{
    mFormatter.reset(new LogFormatter("%d{%Y-%m-%d %H:%M:%S}%T%t%T%N%T%F%T[%p]%T[%c]%T%f:%l%T%m%n"));

}

void Logger::AddAppender(LogAppender::ptr appender) 
{
    MutexType::Lock lock(mMutex);
    if (!appender->GetFormatter())
    {
        appender->mFormatter = mFormatter;
    }
    mAppenderList.push_back(appender);
}

void Logger::DelAppender(LogAppender::ptr appender) 
{
    MutexType::Lock lock(mMutex);
    for(auto itr = mAppenderList.begin(); 
        itr != mAppenderList.end(); ++itr) 
    {
        if(appender == *itr) 
        {
            mAppenderList.erase(itr);
            break;
        }
    }
}

void Logger::SetFormatter(LogFormatter::ptr val)
{
    //TODO:这里循环里面为什么还要再加锁
    MutexType::Lock lock(mMutex);
    mFormatter = val;

    for(auto& i : mAppenderList) 
    {
        MutexType::Lock ll(i->mMutex);
        if(!i->bHasFormatter) 
        {
            i->mFormatter = mFormatter;
        }
    }
}

void Logger::SetFormatter(const std::string& val)
{
    primo::LogFormatter::ptr new_val(new primo::LogFormatter(val));
    if (new_val->IsError())
    {
        std::cout << "Logger SetFormatter name = " << mName << 
            " value = " << val <<" invalid formatter" << std::endl;
        return;
    }
    SetFormatter(new_val);
}

std::string Logger::ToYamlString()
{
    MutexType::Lock lock(mMutex);
    YAML::Node node;
    node["name"] = mName;
    if (mLevel != LOG_LEVEL::UNKNOW)
    {
        node["Level"] = LOG_LEVEL::ToString(mLevel);
    }

    if (mFormatter)
    {
        node["formatter"] = mFormatter->GetPattern();
    }

    for (auto& i : mAppenderList)
    {
        node["appenders"].push_back(YAML::Load(i->ToYamlString()));
    }

    std::stringstream ss;
    ss << node;
    return ss.str();
}

void Logger::Log(LOG_LEVEL::LEVEL level, LogEvent::ptr event) 
{
    if (level >= mLevel) 
    {
        auto self = shared_from_this();
        MutexType::Lock lock(mMutex);
        if (!mAppenderList.empty())
        {    
            for (auto &itr : mAppenderList)
            {
                itr->Log(self, level, event);
            }
        }
        else if (mRoot)
        {
            mRoot->Log(level, event);
        }
    }
}

void Logger::debug(LogEvent::ptr event) 
{
    Log(LOG_LEVEL::DEBUG, event);
}

void Logger::info(LogEvent::ptr event) 
{
    Log(LOG_LEVEL::INFO, event);
}

void Logger::warn(LogEvent::ptr event) 
{
    Log(LOG_LEVEL::WARN, event);
}

void Logger::error(LogEvent::ptr event) 
{
    Log(LOG_LEVEL::ERROR, event);
}

void Logger::fatal(LogEvent::ptr event)
{
    Log(LOG_LEVEL::FATAL, event);
}

void LogAppender::SetFormatter(LogFormatter::ptr val) 
{ 
    MutexType::Lock lock(mMutex);
    mFormatter = val;
    if(mFormatter) 
    {
        bHasFormatter = true;
    } 
    else 
    {
        bHasFormatter = false;
    }
}

LogFormatter::ptr LogAppender::GetFormatter()
{ 
    MutexType::Lock lock(mMutex);
    return mFormatter;
}

void StdOutAppender::Log(Logger::ptr logger, LOG_LEVEL::LEVEL level, LogEvent::ptr event) 
{
    if(level >= mLevel) 
    {
        MutexType::Lock lock(mMutex);
        std::cout << mFormatter->format(logger, level, event) << std::endl;
    }
}

std::string StdOutAppender::ToYamlString()
{
    YAML::Node node;
    node["type"] = "StdOutAppender";
    if (mLevel != LOG_LEVEL::UNKNOW)
    {
        node["level"] = LOG_LEVEL::ToString(mLevel);
    }

    if (bHasFormatter && mFormatter)
    {
        node["formatter"] = mFormatter->GetPattern();
    }

    std::stringstream ss;
    ss << node;
    return ss.str();
}

FileAppender::FileAppender(const std::string& name) : mFileName(name) 
{
    ReOpen();
}


void FileAppender::Log(Logger::ptr logger, LOG_LEVEL::LEVEL level, LogEvent::ptr event) 
{
    if(level >= mLevel) 
    {
        MutexType::Lock lock(mMutex);
        mFileStream << mFormatter->format(logger, level, event);
    }
}


std::ofstream& FileAppender::ReOpen() 
{
    MutexType::Lock lock(mMutex);
    if(mFileStream) 
    {
        mFileStream.close();
    }
    mFileStream.open(mFileName);
    return mFileStream;
}

std::string FileAppender::ToYamlString()
{
    MutexType::Lock lock(mMutex);
    YAML::Node node;
    node["type"] = "FileAppender";
    if (mLevel != LOG_LEVEL::UNKNOW)
    {
        node["level"] = LOG_LEVEL::ToString(mLevel);
    }

    if (bHasFormatter && mFormatter)
    {
        node["formatter"] = mFormatter->GetPattern();
    }

    std::stringstream ss;
    ss << node;
    return ss.str();
}


LogFormatter::LogFormatter(const std::string& pattern) 
                            : mPattern(pattern)
{
    Init();
}

std::string LogFormatter::format(Logger::ptr logger, LOG_LEVEL::LEVEL level, LogEvent::ptr event)
{
    std::stringstream ss;
    for (auto& i : mItems)
    {
        i->format(ss, logger, level, event);
    }
    return ss.str();
}



class MessageFormatItem : public LogFormatter::FormatItem 
{
public:
    MessageFormatItem(const std::string& str = "") {}
    void format(std::ostream& os, Logger::ptr logger, LOG_LEVEL::LEVEL level, LogEvent::ptr event) override 
    {
        os << event->GetContent();
    }
};

class LevelFormatItem : public LogFormatter::FormatItem {
public:
    LevelFormatItem(const std::string& str = "") {}
    void format(std::ostream& os, Logger::ptr logger, LOG_LEVEL::LEVEL level, LogEvent::ptr event) override {
        os << LOG_LEVEL::ToString(level);
    }
};

class ElapseFormatItem : public LogFormatter::FormatItem {
public:
    ElapseFormatItem(const std::string& str = "") {}
    void format(std::ostream& os, Logger::ptr logger, LOG_LEVEL::LEVEL level, LogEvent::ptr event) override {
        os << event->GetElapse();
    }
};

class NameFormatItem : public LogFormatter::FormatItem {
public:
    NameFormatItem(const std::string& str = "") {}
    void format(std::ostream& os, Logger::ptr logger, LOG_LEVEL::LEVEL level, LogEvent::ptr event) override {
        os << event->GetLogger()->GetName();
    }
};

class ThreadIdFormatItem : public LogFormatter::FormatItem {
public:
    ThreadIdFormatItem(const std::string& str = "") {}
    void format(std::ostream& os, Logger::ptr logger, LOG_LEVEL::LEVEL level, LogEvent::ptr event) override {
        os << event->GetThreadId();
    }
};

class FiberIdFormatItem : public LogFormatter::FormatItem {
public:
    FiberIdFormatItem(const std::string& str = "") {}
    void format(std::ostream& os, Logger::ptr logger, LOG_LEVEL::LEVEL level, LogEvent::ptr event) override {
        os << event->GetFiberId();
    }
};

class ThreadNameFormatItem : public LogFormatter::FormatItem {
public:
    ThreadNameFormatItem(const std::string& str = "") {}
    void format(std::ostream& os, Logger::ptr logger, LOG_LEVEL::LEVEL level, LogEvent::ptr event) override {
        os << event->GetThreadName();
    }
};

class DateTimeFormatItem : public LogFormatter::FormatItem {
public:
    DateTimeFormatItem(const std::string& format = "%Y-%m-%d %H:%M:%S")
        :m_format(format) {
        if(m_format.empty()) {
            m_format = "%Y-%m-%d %H:%M:%S";
        }
    }

    void format(std::ostream& os, Logger::ptr logger, LOG_LEVEL::LEVEL level, LogEvent::ptr event) override {
        struct tm tm;
        time_t time = event->GetTime();
        localtime_r(&time, &tm);
        char buf[64];
        strftime(buf, sizeof(buf), m_format.c_str(), &tm);
        os << buf;
    }
private:
    std::string m_format;
};

class FilenameFormatItem : public LogFormatter::FormatItem {
public:
    FilenameFormatItem(const std::string& str = "") {}
    void format(std::ostream& os, Logger::ptr logger, LOG_LEVEL::LEVEL level, LogEvent::ptr event) override {
        os << event->GetFile();
    }
};

class LineFormatItem : public LogFormatter::FormatItem {
public:
    LineFormatItem(const std::string& str = "") {}
    void format(std::ostream& os, Logger::ptr logger, LOG_LEVEL::LEVEL level, LogEvent::ptr event) override {
        os << event->GetLine();
    }
};

class NewLineFormatItem : public LogFormatter::FormatItem {
public:
    NewLineFormatItem(const std::string& str = "") {}
    void format(std::ostream& os, Logger::ptr logger, LOG_LEVEL::LEVEL level, LogEvent::ptr event) override {
        os << std::endl;
    }
};


class StringFormatItem : public LogFormatter::FormatItem {
public:
    StringFormatItem(const std::string& str)
        :m_string(str) {}
    void format(std::ostream& os, Logger::ptr logger, LOG_LEVEL::LEVEL level, LogEvent::ptr event) override {
        os << m_string;
    }
private:
    std::string m_string;
};

class TabFormatItem : public LogFormatter::FormatItem {
public:
    TabFormatItem(const std::string& str = "") {}
    void format(std::ostream& os, Logger::ptr logger, LOG_LEVEL::LEVEL level, LogEvent::ptr event) override {
        os << "\t";
    }
private:
    std::string m_string;
};

// %d{%Y-%m-%d %H:%M:%S}%T%t%T%N%T%F%T[%p]%T[%c]%T%f:%l%T%m%n
void LogFormatter::Init()
{
    //str, format, type  1: 解析的formatter   0: 字符串
    std::vector<std::tuple<std::string, std::string, int>> vec;
    std::string nstr;
    for (size_t i = 0; i < mPattern.size(); ++i)
    {
        if (mPattern[i] != '%')
        {
            nstr.append(1, mPattern[i]);
            continue;
        }

        if ( i + 1 < mPattern.size() )
        {
            if (mPattern[i + 1] == '%')
            {
                nstr.append(1, '%');
                continue;
            }
        }

        size_t n = i + 1;
        
        //状态位
        int iFmtStatus = 0;
        size_t iFmtBegin = 0;

        std::string str, fmt;

        while (n < mPattern.size())
        {
            if (!iFmtStatus && !isalpha(mPattern[n]) && mPattern[n] != '{' 
                && mPattern[n] != '}')
            {
                str = mPattern.substr(i + 1, n - i - 1);
                break;
            }

            if (iFmtStatus == 0 && mPattern[n] == '{')
            {                
                str = mPattern.substr(i + 1, n - i - 1);
                iFmtStatus = 1;
                iFmtBegin = n;
                ++n;
                continue;
            }

            else if (iFmtStatus == 1 && mPattern[n] == '}')
            {
                fmt = mPattern.substr(iFmtBegin + 1, n - iFmtBegin - 1);
                iFmtStatus = 0;
                ++n;
                break;
            }

            ++n;
            if (n == mPattern.size() && str.empty())
            {
                str = mPattern.substr(i + 1);
            }
        }

        if (iFmtStatus == 0)
        {
            if (!nstr.empty())
            {
                vec.push_back(std::make_tuple(nstr, "", 0));
                nstr.clear();
            }
            vec.push_back(std::make_tuple(str, fmt, 1));
            i = n - 1;
        }
        else if (iFmtStatus == 1)
        {
            std::cout << "pattern parse error: " << mPattern << " - " << mPattern.substr(i) << std::endl;
            bError = true;
            vec.push_back(std::make_tuple("<<pattern_error>>", fmt, 0));
        }
    }

    if (!nstr.empty())
    {
        vec.push_back(std::make_tuple(nstr, "", 0));
    }

    static std::map<std::string, std::function< FormatItem::ptr(const std::string& str)> > S_FormatItems = 
    {
#define XX(str, C)\
        {#str, [](const std::string& fmt) { return FormatItem::ptr(new C(fmt)); } }

        XX(m, MessageFormatItem),           //m:消息
        XX(p, LevelFormatItem),             //p:日志级别
        XX(r, ElapseFormatItem),            //r:累计毫秒数
        XX(c, NameFormatItem),              //c:日志名称
        XX(t, ThreadIdFormatItem),          //t:线程id
        XX(n, NewLineFormatItem),           //n:换行
        XX(d, DateTimeFormatItem),          //d:时间
        XX(f, FilenameFormatItem),          //f:文件名
        XX(l, LineFormatItem),              //l:行号
        XX(T, TabFormatItem),               //T:Tab
        XX(F, FiberIdFormatItem),           //F:协程id
        XX(N, ThreadNameFormatItem),        //N:线程名称
#undef XX
    };

    for(auto& i : vec) 
    {
        if(std::get<2>(i) == 0)
        {
            mItems.push_back(FormatItem::ptr(new StringFormatItem(std::get<0>(i))));
        }
        else 
        {
            auto it = S_FormatItems.find(std::get<0>(i));
            if(it == S_FormatItems.end()) 
            {
                mItems.push_back(FormatItem::ptr(new StringFormatItem("<<error_format %" + std::get<0>(i) + ">>")));
                bError = true;
            }
            else 
            {
                mItems.push_back(it->second(std::get<1>(i)));
            }
        }
        //std::cout << "(" << std::get<0>(i) << ") - (" << std::get<1>(i) << ") - (" << std::get<2>(i) << ")" << std::endl;
    }
    //std::cout << m_items.size() << std::endl;
}


void LoggerMgr::Init()
{
}

Logger::ptr LoggerMgr::GetRoot() const
{
    return mRoot;
}

Logger::ptr LoggerMgr::GetLogger(const std::string& name)
{
    MutexType::Lock lock(mMutex);
    auto itr = mLoggers.find(name);
    if (itr != mLoggers.end())
    {
        return itr->second;
    }

    Logger::ptr logger(new Logger(name));
    logger->mRoot = mRoot;
    mLoggers[name] = logger;
    return logger;
}

std::string LoggerMgr::ToYamlString()
{
    MutexType::Lock lock(mMutex);
    YAML::Node node;
    for(auto& i : mLoggers)
    {
        node.push_back(YAML::Load(i.second->ToYamlString()));
    }
    std::stringstream ss;
    ss << node;
    return ss.str();
}

struct LogAppenderDefine
{
    int type = 0; //1: file,  2: stdout
    LOG_LEVEL::LEVEL level = LOG_LEVEL::UNKNOW;
    std::string formatter;
    std::string file;
    bool operator==(const LogAppenderDefine& oth) const
    {
        return type == oth.type
            && level == oth.level
            && formatter == oth.formatter
            && file == oth.file;
    }
};

struct LogDefine
{
    std::string name;
    LOG_LEVEL::LEVEL level = LOG_LEVEL::UNKNOW;
    std::string formatter;
    std::vector<LogAppenderDefine> appenders;
    bool operator==(const LogDefine& oth) const
    {
        return name == oth.name
            && level == oth.level
            && formatter == oth.formatter
            && appenders == oth.appenders;
    }

    bool operator<(const LogDefine& oth) const
    {
        return name < oth.name;
    }

    bool IsValid() const
    {
        return !name.empty();
    }
};


//类型转换模板类偏特化, YAML::string转化为LogDefine
template <>
class LexicalCast<std::string, LogDefine>
{
public:
    LogDefine operator()(const std::string& str)
    {
        YAML::Node n = YAML::Load(str);
        LogDefine lg;
        if (!n["name"].IsDefined())
        {
            std::cout << "Log config error: name is null, " << n << std::endl;
            throw std::logic_error("log config name is null");
        }
        lg.name = n["name"].as<std::string>();
        lg.level = LOG_LEVEL::FromString(n["level"].IsDefined()?n["level"].as<std::string>() : "");

        if (n["formatter"].IsDefined())
        {
            lg.formatter = n["formatter"].as<std::string>();
        }

        if (n["appenders"].IsDefined())
        {
            for (size_t i = 0; i < n["appenders"].size(); ++i)
            {
                auto apd = n["appenders"][i];
                if (!apd["type"].IsDefined())
                {
                    std::cout << "log config error: appender type is null, " << apd
                              << std::endl;
                    continue;
                }
                LogAppenderDefine log_apd;
                std::string type = apd["type"].as<std::string>();
                if (type == "FileAppender")
                {
                    log_apd.type = 1;
                    if (!apd["file"].IsDefined())
                    {
                        std::cout << "log config error: fileappender file is null, " << apd
                              << std::endl;
                        continue;
                    }
                    log_apd.file = apd["file"].as<std::string>();
                    if (apd["formatter"].IsDefined())
                    {
                        log_apd.formatter = apd["file"].as<std::string>();
                    }

                }
                else if (type == "StdOutAppender")
                {
                    log_apd.type = 2;
                    if(apd["formatter"].IsDefined()) {
                        log_apd.formatter = apd["formatter"].as<std::string>();
                    }
                }
                else 
                {
                    std::cout << "log config error: appender type is invalid, " << apd
                              << std::endl;
                    continue;
                }
                lg.appenders.push_back(log_apd);
            }
        }
        return lg;
    }
};


//模板类型偏特化：LogDefine -> string
template<>
class LexicalCast<LogDefine, std::string>
{
public:
    std::string operator()(const LogDefine& i) 
    {
        YAML::Node n;
        n["name"] = i.name;
        if(i.level != LOG_LEVEL::UNKNOW) 
        {
            n["level"] = LOG_LEVEL::ToString(i.level);
        }
        if(!i.formatter.empty()) 
        {
            n["formatter"] = i.formatter;
        }

        for(auto& a : i.appenders) 
        {
            YAML::Node na;
            if(a.type == 1) 
            {
                na["type"] = "FileAppender";
                na["file"] = a.file;
            } else if(a.type == 2) 
            {
                na["type"] = "StdOutAppender";
            }
            if(a.level != LOG_LEVEL::UNKNOW) 
            {
                na["level"] = LOG_LEVEL::ToString(a.level);
            }

            if(!a.formatter.empty()) 
            {
                na["formatter"] = a.formatter;
            }

            n["appenders"].push_back(na);
        }
        std::stringstream ss;
        ss << n;
        return ss.str();
    }
};

primo::CfgVar<std::set<LogDefine> >::ptr g_log_defines =
    primo::Config::LookUp("logs", std::set<LogDefine>(), "logs config");


struct LogIniter 
{
    LogIniter() 
    {
        g_log_defines->AddListener([](const std::set<LogDefine>& old_value,
                    const std::set<LogDefine>& new_value)
        {
            P_LOG_INFO(P_LOG_ROOT()) << "on_logger_conf_changed";
            for(auto& i : new_value)
            {
                auto it = old_value.find(i);
                primo::Logger::ptr logger;
                if(it == old_value.end()) 
                {
                    //新增logger
                    logger = P_LOG_NAME(i.name);
                } 
                else 
                {
                    if(!(i == *it)) 
                    {
                        //修改的logger
                        logger = P_LOG_NAME(i.name);
                    } 
                    else 
                    {
                        continue;
                    }
                }
                logger->SetLevel(i.level);
                //std::cout << "** " << i.name << " level=" << i.level
                //<< "  " << logger << std::endl;
                if(!i.formatter.empty())
                {
                    logger->SetFormatter(i.formatter);
                }

                logger->ClearAppenders();
                for (auto& a : i.appenders) 
                {
                    primo::LogAppender::ptr ap;
                    if (a.type == 1) 
                    {
                        ap.reset(new FileAppender(a.file));
                    } 
                    else if (a.type == 2) 
                    {
                        /*
                        if(!primo::EnvMgr::GetInstance()->has("d")) 
                        {
                            ap.reset(new StdOutAppender);
                        } 
                        else 
                        {
                            continue;
                        }
                        */
                        ap.reset(new StdOutAppender);
                    }
                    ap->SetLevel(a.level);
                    if (!a.formatter.empty()) 
                    {
                        LogFormatter::ptr fmt(new LogFormatter(a.formatter));
                        if (!fmt->IsError()) 
                        {
                            ap->SetFormatter(fmt);
                        } 
                        else 
                        {
                            std::cout << "log.name=" << i.name << " appender type=" << a.type
                                      << " formatter=" << a.formatter << " is invalid" << std::endl;
                        }
                    }
                    logger->AddAppender(ap);
                }
            }

            for(auto& i : old_value) 
            {
                auto it = new_value.find(i);
                if(it == new_value.end()) 
                {
                    //删除logger
                    auto logger = P_LOG_NAME(i.name);
                    logger->SetLevel((LOG_LEVEL::LEVEL)0);
                    logger->ClearAppenders();
                }
            }
        });
    }
};

static LogIniter __LOG_INITER;

}
