#include <Logger.h>

#include <spdlog/sinks/stdout_color_sinks.h>

std::shared_ptr<spdlog::logger> Logger::m_logger = {};

void Logger::init()
{
    spdlog::set_pattern("%^[%T] %n: %v%$");
    m_logger = spdlog::stdout_color_st("Engine");
    m_logger->set_level(spdlog::level::trace);
}
