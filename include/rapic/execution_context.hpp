//
// Created by sergi on 9/1/2023.
//

#pragma once

#include <future>
#include <memory>

namespace rapic {

// ExecutionContext interface
class ExecutionContext {
public:
    virtual ~ExecutionContext() = default;

    virtual void PostTask(std::packaged_task<void(void)> task) = 0;
};

}  // namespace rapic