//
// Created by sergi on 9/1/2023.
//

#pragma once

#include <memory>

namespace rapic {

class Task;

// ExecutionContext interface
class ExecutionContext {
public:
    virtual ~ExecutionContext() = default;
    virtual void PostTask(std::unique_ptr<Task> task) = 0;
};

}  // namespace rapic