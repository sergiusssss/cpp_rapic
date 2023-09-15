//
// Created by sergi on 8/26/2023.
//

#include <rapic/tests/utils/thread_pool_execution_context.hpp>

namespace rapic::tests::utils {

ThreadPoolExecutionContext::ThreadPoolExecutionContext(std::uint32_t thread_number)
    : thread_number_(thread_number) {}

ThreadPoolExecutionContext::~ThreadPoolExecutionContext() { Stop(); }

void ThreadPoolExecutionContext::PostTask(std::packaged_task<void(void)> task) {
    std::lock_guard<std::mutex> lock(tasks_queue_mutex_);

    tasks_queue_.push(std::move(task));

    tasks_queue_cv_.notify_one();
}

bool ThreadPoolExecutionContext::Start() {
    if (!threads_.empty()) {
        return false;
    }

    running_flag_.store(true);

    threads_.reserve(thread_number_);
    for (std::size_t i = 0; i < thread_number_; ++i) {
        threads_.emplace_back(&ThreadPoolExecutionContext::ThreadLoop, this);
    }

    return true;
}

bool ThreadPoolExecutionContext::Stop() {
    running_flag_.store(false);
    tasks_queue_cv_.notify_all();

    threads_.clear();

    std::lock_guard<std::mutex> lock(tasks_queue_mutex_);

    decltype(tasks_queue_) empty_queue;
    tasks_queue_.swap(empty_queue);

    return true;
}

bool ThreadPoolExecutionContext::IsRunning() { return running_flag_.load(); }

void ThreadPoolExecutionContext::ThreadLoop() {
    while (running_flag_.load()) {
        std::packaged_task<void(void)> task;
        {
            std::lock_guard<std::mutex> queue_lock(tasks_queue_mutex_);
            if (!tasks_queue_.empty()) {
                task = std::move(tasks_queue_.front());
                tasks_queue_.pop();
            }
        }

        if (task.valid()) {
            task();
        }

        std::unique_lock<std::mutex> lock(tasks_queue_mutex_);
        tasks_queue_cv_.wait(lock, [&]() { return !(tasks_queue_.empty() && running_flag_.load()); });
    }
}

}  // namespace rapic::tests::utils
