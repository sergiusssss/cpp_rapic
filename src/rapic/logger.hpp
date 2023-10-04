//
// Created by sergi on 9/28/2023.
//

#pragma once

#include <spdlog/spdlog.h>

namespace rapic {

spdlog::logger& GetLogger();
std::shared_ptr<spdlog::logger> GetLoggerPtr();

#define RAPIC_TRACE(...) SPDLOG_LOGGER_TRACE(GetLoggerPtr(), __VA_ARGS__)
#define RAPIC_DEBUG(...) SPDLOG_LOGGER_DEBUG(GetLoggerPtr(), __VA_ARGS__)
#define RAPIC_INFO(...) SPDLOG_LOGGER_INFO(GetLoggerPtr(), __VA_ARGS__)
#define RAPIC_WARN(...) SPDLOG_LOGGER_WARN(GetLoggerPtr(), __VA_ARGS__)
#define RAPIC_ERROR(...) SPDLOG_LOGGER_ERROR(GetLoggerPtr(), __VA_ARGS__)
#define RAPIC_CRITICAL(...) SPDLOG_LOGGER_CRITICAL(GetLoggerPtr(), __VA_ARGS__)

}  // namespace rapic
