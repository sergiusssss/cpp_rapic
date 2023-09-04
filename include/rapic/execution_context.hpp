//
// Created by sergi on 9/1/2023.
//

#pragma once

#include <memory>

class Task;

// ExecutionContext interface
class ExecutionContext {
public:
    virtual ~ExecutionContext() = default;
    virtual void PostTask(std::unique_ptr<Task> task) = 0;
    virtual void Run() = 0;
    virtual void Stop() = 0;
    virtual bool IsRunning() const = 0;
};