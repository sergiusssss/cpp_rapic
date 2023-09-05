//
// Created by sergi on 8/26/2023.
//

#include "thread_pool_context_impl.hpp"

#include <spdlog/spdlog.h>

#include <rapic/thread_pool_context.hpp>

namespace rapic {

class Task {};

ThreadPoolContext::Impl::Impl(std::uint32_t thread_number)
    : thread_number_(thread_number) {}

ThreadPoolContext::Impl::~Impl() { Stop(); }

void ThreadPoolContext::Impl::PostTask(std::unique_ptr<Task> task) {
    std::lock_guard<std::mutex> lock(tasks_queue_mutex_);

    tasks_queue_.push(std::move(task));

    running_flag_cv_.notify_one();
}

bool ThreadPoolContext::Impl::Start() {
    if (!threads_.empty()) {
        spdlog::error("ThreadPool isn't stopped correctly.");
        return false;
    }

    running_flag_.store(true);

    threads_.reserve(thread_number_);
    for (std::size_t i = 0; i < thread_number_; ++i) {
        threads_.emplace_back(&ThreadPoolContext::Impl::ThreadLoop, this);
    }

    return true;
}

bool ThreadPoolContext::Impl::Stop() {
    running_flag_.store(false);
    running_flag_cv_.notify_all();

    threads_.clear();

    std::lock_guard<std::mutex> lock(tasks_queue_mutex_);

    decltype(tasks_queue_) empty_queue;
    tasks_queue_.swap(empty_queue);

    running_flag_cv_.notify_all();

    return true;
}

bool ThreadPoolContext::Impl::IsRunning() { return running_flag_.load(); }

void ThreadPoolContext::Impl::ThreadLoop() {
    while (running_flag_.load()) {
        std::unique_ptr<Task> task;
        {
            std::lock_guard<std::mutex> queue_lock(tasks_queue_mutex_);
            if (!tasks_queue_.empty()) {
                task = std::move(tasks_queue_.front());
                tasks_queue_.pop();
            }
        }

        if (task) {
            (*task)();
        }

        std::unique_lock<std::mutex> lock(running_flag_mutex_);
        running_flag_cv_.wait(lock, [&]() { return !running_flag_.load(); });
    }
}

}  // namespace rapic
