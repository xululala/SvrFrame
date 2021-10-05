/*
 * @Descripttion: 日志系统
 * @version: 
 * @Author: primoxu
 * @Date: 2021-08-20 22:25:03
 * @LastEditors: sueRimn
 * @LastEditTime: 2021-10-01 15:55:29
 */

#include "Logger.h"

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
    if (!appender->GetFormatter())
    {
        appender->SetFormatter(mFormatter);
    }
    mAppenderList.push_back(appender);
}

void Logger::DelAppender(LogAppender::ptr appender) 
{
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

void Logger::Log(LOG_LEVEL::LEVEL level, LogEvent::ptr event) 
{
    if (level >= mLevel) 
    {
        for (auto &itr : mAppenderList)
        {
            itr->Log(shared_from_this() ,level, event);
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

void StdOutAppender::Log(Logger::ptr logger, LOG_LEVEL::LEVEL level, LogEvent::ptr event) 
{
    if(level >= mLevel) 
    {
        std::cout << mFormatter->format(logger, level, event) << std::endl;
    }
}

FileAppender::FileAppender(const std::string& name) : mFileName(name) 
{
    ReOpen();
}


void FileAppender::Log(Logger::ptr logger, LOG_LEVEL::LEVEL level, LogEvent::ptr event) 
{
    if(level >= mLevel) 
    {
        mFileStream << mFormatter->format(logger, level, event);
    }
}


std::ofstream& FileAppender::ReOpen() 
{
    if(mFileStream) 
    {
        mFileStream.close();
    }
    mFileStream.open(mFileName);
    return mFileStream;
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

void LogFormatter::Init()
{
    //str, format, type
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

Logger::ptr LoggerMgr::GetLogger(const std::string& name) const
{
    auto itr = mLoggers.find(name);
    return itr == mLoggers.end() ? mRoot : itr->second;
}

}
