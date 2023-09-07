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

#include <rapic/thread_pool_context.hpp>

namespace rapic {

// ExecutionContext interface
class ThreadPoolContext::Impl {
public:
    explicit Impl(std::uint32_t thread_number);
    ~Impl();

    void PostTask(std::unique_ptr<Task> task);

    bool Start();
    bool Stop();
    bool IsRunning();

private:
    void ThreadLoop();

    std::uint32_t thread_number_;

    std::queue<std::unique_ptr<Task>> tasks_queue_;
    std::mutex tasks_queue_mutex_;

    std::vector<std::jthread> threads_;
    std::atomic<bool> running_flag_;

    //std::mutex tasks_queue_mutex_;
    std::condition_variable tasks_queue_cv_;
};

}  // namespace rapic