//
// Created by sergi on 9/1/2023.
//

#pragma once

#include <boost/asio/thread_pool.hpp>

#include "test_execution_context.hpp"

namespace rapic::tests::utils {

class ThreadPoolExecutionContext : public TestExecutionContext {
public:
    explicit ThreadPoolExecutionContext(std::uint32_t thread_number);
    ~ThreadPoolExecutionContext() override;

    /// ExecutionContext interface
    void PostTask(std::packaged_task<void(void)> task) override;

    bool Stop();

    /// TestExecutionContext interface
    std::string GetName() override;

private:
    boost::asio::thread_pool thread_pool_;
};

}  // namespace rapic::tests::utils
