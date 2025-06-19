#include <logger.h>

#include <spdlog/sinks/stdout_color_sinks.h>

std::shared_ptr<spdlog::logger> logger::m_logger = {};

void logger::init()
{
    spdlog::set_pattern("%^[%T] %n: %v%$");
    m_logger = spdlog::stdout_color_st("Engine");
    m_logger->set_level(spdlog::level::trace);
}
