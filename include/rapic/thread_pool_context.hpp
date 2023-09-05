//
// Created by sergi on 9/1/2023.
//

#pragma once

#include <functional>

#include <rapic/execution_context.hpp>

namespace rapic {

// ExecutionContext interface
class ThreadPoolContext : public ExecutionContext {
public:
    explicit ThreadPoolContext(std::uint32_t thread_number);
    ~ThreadPoolContext() override;

    /// ExecutionContext interface
    void PostTask(std::unique_ptr<Task> task) override;

    void Start();
    void Stop();
    bool IsRunning();

private:
    class Impl;
    std::unique_ptr<Impl> impl_;
};

}  // namespace rapic