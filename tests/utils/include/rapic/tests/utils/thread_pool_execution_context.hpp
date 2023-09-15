//
// Created by sergi on 9/1/2023.
//

#pragma once

#include <atomic>
#include <condition_variable>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>
#include "rapic/execution_context.hpp"

namespace rapic::tests::utils {

class ThreadPoolExecutionContext : public ExecutionContext {
public:
    explicit ThreadPoolExecutionContext(std::uint32_t thread_number);
    ~ThreadPoolExecutionContext() override;

    /// ExecutionContext interface
    void PostTask(std::packaged_task<void(void)> task) override;

    bool Start();
    bool Stop();
    bool IsRunning();

private:
    void ThreadLoop();

    std::uint32_t thread_number_;

    std::queue<std::packaged_task<void(void)>> tasks_queue_;
    std::mutex tasks_queue_mutex_;

    std::vector<std::jthread> threads_;
    std::atomic<bool> running_flag_;

    // std::mutex tasks_queue_mutex_;
    std::condition_variable tasks_queue_cv_;
};

}  // namespace rapic::tests::utils
