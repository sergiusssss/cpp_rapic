//
// Created by sergi on 9/1/2023.
//

#pragma once

#include <rapic/execution_context.hpp>

namespace rapic {

class Task;

// ExecutionContext interface
class ThreadPoolContext : public ExecutionContext {
public:
    ~ThreadPoolContext() override = default;
    void PostTask(std::unique_ptr<Task> task) override;
};

}  // namespace rapic