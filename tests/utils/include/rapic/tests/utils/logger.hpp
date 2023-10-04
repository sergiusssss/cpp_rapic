//
// Created by sergi on 9/28/2023.
//

#pragma once

#include <spdlog/spdlog.h>

namespace rapic::tests::utils {

spdlog::logger& GetLogger();
std::shared_ptr<spdlog::logger> GetLoggerPtr();

#define RAPIC_UTILS_TRACE(...) SPDLOG_LOGGER_TRACE(rapic::tests::utils::GetLoggerPtr(), __VA_ARGS__)
#define RAPIC_UTILS_DEBUG(...) SPDLOG_LOGGER_DEBUG(rapic::tests::utils::GetLoggerPtr(), __VA_ARGS__)
#define RAPIC_UTILS_INFO(...) SPDLOG_LOGGER_INFO(rapic::tests::utils::GetLoggerPtr(), __VA_ARGS__)
#define RAPIC_UTILS_WARN(...) SPDLOG_LOGGER_WARN(rapic::tests::utils::GetLoggerPtr(), __VA_ARGS__)
#define RAPIC_UTILS_ERROR(...) SPDLOG_LOGGER_ERROR(rapic::tests::utils::GetLoggerPtr(), __VA_ARGS__)
#define RAPIC_UTILS_CRITICAL(...) SPDLOG_LOGGER_CRITICAL(rapic::tests::utils::GetLoggerPtr(), __VA_ARGS__)

}  // namespace rapic::tests::utils
