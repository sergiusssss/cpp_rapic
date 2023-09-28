//
// Created by sergi on 9/28/2023.
//

#include <rapic/tests/utils/logger.hpp>

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

namespace rapic::tests::utils {

class LoggerWrapper {
public:
    LoggerWrapper()
        : logger_(spdlog::stdout_color_mt("rapic-test-utils")) {
        logger_->set_level(spdlog::level::trace);
        logger_->set_pattern("[%Y-%m-%d %T.%e] [%^%l%$] [%n] [%s:%#] %v");
    }

    std::shared_ptr<spdlog::logger> GetLogger() { return logger_; }

private:
    std::shared_ptr<spdlog::logger> logger_;
};

spdlog::logger& GetLogger() { return *GetLoggerPtr(); }

std::shared_ptr<spdlog::logger> GetLoggerPtr() {
    static auto logger_wrapper = LoggerWrapper();
    return logger_wrapper.GetLogger();
}

}  // namespace rapic::tests::utils
