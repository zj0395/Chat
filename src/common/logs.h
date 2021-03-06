//
// Created by zj on 12/26/18.
//

#ifndef RPC_LOGS_H
#define RPC_LOGS_H

// SPDLOG_LEVEL_TRACE,
// SPDLOG_LEVEL_DEBUG,
// SPDLOG_LEVEL_INFO,
// SPDLOG_LEVEL_WARN,
// SPDLOG_LEVEL_ERROR,
// SPDLOG_LEVEL_CRITICAL,
// SPDLOG_LEVEL_OFF
#ifdef DEBUG
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
#else
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_DEBUG
#endif

#include "singleton.h"
#include "spdlog/spdlog.h"
#include "spdlog/async.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include <memory>
#include <string>
//#include <atomic>

#define _LOGGER zj::Logger::get_instance().get_logger()

#define LOG_TRACE(...)    SPDLOG_LOGGER_TRACE(_LOGGER, __VA_ARGS__)
#define LOG_DEBUG(...)    SPDLOG_LOGGER_DEBUG(_LOGGER, __VA_ARGS__)
#define LOG_INFO(...)     SPDLOG_LOGGER_INFO(_LOGGER, __VA_ARGS__)
#define LOG_WARN(...)     SPDLOG_LOGGER_WARN(_LOGGER, __VA_ARGS__)
#define LOG_ERROR(...)    SPDLOG_LOGGER_ERROR(_LOGGER, __VA_ARGS__)
#define LOG_CRITICAL(...) SPDLOG_LOGGER_CRITICAL(_LOGGER, __VA_ARGS__)

namespace zj{
class Logger : public Singleton<Logger>{
    friend class Singleton<Logger>;
    typedef std::shared_ptr<spdlog::async_logger> LogPtrType;
public:
    static void init(const std::string& name, const std::string& file_name) {
        _name = name;
        _file_name = file_name;
        // make sure the instance of Logger is initialized first,
        // so will also be destroyed last.
        get_instance();
    }

    inline LogPtrType get_logger() { return _logger;}
private:
    Logger() {
        spdlog::init_thread_pool(8192, 1);
        auto stdout_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt >();
        stdout_sink->set_level(spdlog::level::debug);
        stdout_sink->set_pattern("[%Y-%m-%d %H.%M.%S.%e] [%n] [%^%l%$] [%@] [thread %t] %v");
        auto rotating_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(_file_name, 1024*1024*10, 3);
        rotating_sink->set_level(spdlog::level::trace);
        rotating_sink->set_pattern("[%Y-%m-%d %H.%M.%S.%e] [%n] [%^%l%$] [%@] [thread %t] %v");
        std::vector<spdlog::sink_ptr> sinks {stdout_sink, rotating_sink};
        _logger = std::make_shared<spdlog::async_logger>(_name, sinks.begin(), sinks.end(), spdlog::thread_pool(), spdlog::async_overflow_policy::block);
        // spdlog::register_logger(_logger);
    }
    LogPtrType _logger;
    static std::string _name;
    static std::string _file_name;
    //std::atomic<bool> _init_flag;
};
}

#endif
