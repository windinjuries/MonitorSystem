
#pragma once
#ifndef EASYLOGGINGHELPER_H
#define EASYLOGGINGHELPER_H
#include "easylogging++.h"
// #include <io.h>
#include <thread>

INITIALIZE_EASYLOGGINGPP

namespace el 
{
    static std::string LogRootPath = "log";
    static el::base::SubsecondPrecision LogSsPrec(3);
    static std::string LoggerToday = el::base::utils::DateTime::getDateTime("%Y%M%d", &LogSsPrec);

    static void ConfigureLogger()
    {       
        std::string datetimeY = el::base::utils::DateTime::getDateTime("%Y", &LogSsPrec);
        std::string datetimeYM = el::base::utils::DateTime::getDateTime("%Y%M", &LogSsPrec);
        std::string datetimeYMd = el::base::utils::DateTime::getDateTime("%Y%M%d", &LogSsPrec);
        
        // std::string filePath = LogRootPath + "/" + datetimeY + "/" + datetimeYM + "/";
        std::string filePath = LogRootPath + "/";
        std::string filename;

        el::Configurations defaultConf;
        defaultConf.setToDefault();
        //建议使用setGlobally
        defaultConf.setGlobally(el::ConfigurationType::Format, "[%datetime{%Y-%M-%d %H:%m:%s.%g}] %msg");
        defaultConf.setGlobally(el::ConfigurationType::Enabled, "true");
        defaultConf.setGlobally(el::ConfigurationType::ToFile, "true");
        defaultConf.setGlobally(el::ConfigurationType::ToStandardOutput, "true");
        defaultConf.setGlobally(el::ConfigurationType::SubsecondPrecision, "3");
        // defaultConf.setGlobally(el::ConfigurationType::PerformanceTracking, "true");
        defaultConf.setGlobally(el::ConfigurationType::LogFlushThreshold, "1");

        //限制文件大小时配置
        //defaultConf.setGlobally(el::ConfigurationType::MaxLogFileSize, "2097152");

        filename = datetimeYMd + "_" + el::LevelHelper::convertToString(el::Level::Global)+".log";
        defaultConf.set(el::Level::Global, el::ConfigurationType::Filename, filePath + filename);

        // filename = datetimeYMd + "_" + el::LevelHelper::convertToString(el::Level::Debug) + ".log";
        // defaultConf.set(el::Level::Debug, el::ConfigurationType::Filename, filePath + filename);

        // filename = datetimeYMd + "_" + el::LevelHelper::convertToString(el::Level::Error) + ".log";
        // defaultConf.set(el::Level::Error, el::ConfigurationType::Filename, filePath + filename);

        // filename = datetimeYMd + "_" + el::LevelHelper::convertToString(el::Level::Fatal) + ".log";
        // defaultConf.set(el::Level::Fatal, el::ConfigurationType::Filename, filePath + filename);

        // filename = datetimeYMd + "_" + el::LevelHelper::convertToString(el::Level::Info) + ".log";
        // defaultConf.set(el::Level::Info, el::ConfigurationType::Filename, filePath + filename);

        // filename = datetimeYMd + "_" + el::LevelHelper::convertToString(el::Level::Trace) + ".log";
        // defaultConf.set(el::Level::Trace, el::ConfigurationType::Filename, filePath + filename);

        // filename = datetimeYMd + "_" + el::LevelHelper::convertToString(el::Level::Warning) + ".log";
        // defaultConf.set(el::Level::Warning, el::ConfigurationType::Filename, filePath + filename);        

        el::Loggers::reconfigureLogger("default", defaultConf);

        //限制文件大小时启用
        //el::Loggers::addFlag(el::LoggingFlag::StrictLogFileSizeCheck);
        el::Loggers::addFlag(el::LoggingFlag::ColoredTerminalOutput);
    }

    class LogDispatcher : public el::LogDispatchCallback
    {
    protected:
        void handle(const el::LogDispatchData* data) noexcept override {
            m_data = data;
            // 使用记录器的默认日志生成器进行调度
            dispatch(m_data->logMessage()->logger()->logBuilder()->build(m_data->logMessage(),
                m_data->dispatchAction() == el::base::DispatchAction::NormalLog));

            //此处也可以写入数据库
        }
    private:
        const el::LogDispatchData* m_data;
        void dispatch(el::base::type::string_t&& logLine) noexcept
        {
            el::base::SubsecondPrecision ssPrec(3);
            static std::string now = el::base::utils::DateTime::getDateTime("%Y%M%d", &ssPrec);
            if (now != LoggerToday)
            {
                LoggerToday = now;
                ConfigureLogger();
            }
        }
    };

    static void InitEasylogging()
    {
        ConfigureLogger();

        el::Helpers::installLogDispatchCallback<LogDispatcher>("LogDispatcher");
        LogDispatcher* dispatcher = el::Helpers::logDispatchCallback<LogDispatcher>("LogDispatcher");
        dispatcher->setEnabled(true);
    }
}
#endif 
