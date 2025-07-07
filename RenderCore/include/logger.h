#pragma once

#include <memory>
#include <spdlog/spdlog.h>

class Logger
{
public:
    static void init();
    inline static std::shared_ptr<spdlog::logger>& get_logger() { return m_logger; }
private:
    static std::shared_ptr<spdlog::logger> m_logger;
};

#define LOG_TRACE(...)		Logger::get_logger()->trace(__VA_ARGS__)
#define LOG_INFO(...)		Logger::get_logger()->info(__VA_ARGS__)
#define LOG_WARN(...)		Logger::get_logger()->warn(__VA_ARGS__)
#define LOG_ERROR(...)		Logger::get_logger()->error(__VA_ARGS__)
#define LOG_CRITICAL(...)	Logger::get_logger()->critical(__VA_ARGS__)
#define LOG_DEBUG(...)		Logger::get_logger()->debug(__VA_ARGS__)