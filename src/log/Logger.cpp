#include "Logger.h"

namespace primo{

Logger::Logger(const std::string& name) : mName(name) {}

void Logger::AddAppender(LogAppender::ptr appender) {
    mAppenderList.push_back(ptr);
}

void Logger::DelAppender(LogAppender::ptr appender) {
    for(auto itr : mAppenderList) {
        if(appender == itr) {
            mAppenderList.erase(itr);
            break;
        }
    
}F

void Logger::Log(LOG_LEVEL::LEVEL::LEVEL level, LogEvent event) {
    if(level >= mLevel) {
        for(auto itr : mAppenderList) {
            ptr -> Log(level, event);
        }
    }
}

void Logger::Log(LOG_LEVEL::LEVEL level, LogEvent::ptr) {

}

void Logger::debug(LogEvent::ptr event) {
    Log(mLevel, event);
}
void Logger::info(LogEvent::ptr event) {

}
void Logger::warn(LogEvent::ptr event) {

}

void Logger::error(LogEvent::ptr event) {

}

void StdOutAppender::Log(LOG_LEVEL::LEVEL level, LogEvent::ptr event) {
    if(level >= mLevel) {
        std::cout << mFormatter->format(event) << std::endl;
    }
}


FileAppender::FileAppender(const std::string& name) : mFileName(name) {}


void FileAppender::Log(LOG_LEVEL::LEVEL level, LogEvent::ptr event) {
    if(level >= mLevel) {
        mFormatter->format(event);
    }
}


bool FileAppender::ReOpen() {
    if(mFileStream) {
        mFileStream.close();
    }
    mFileStream.open(mFileName);
    return mFileStream;
}
    
}
