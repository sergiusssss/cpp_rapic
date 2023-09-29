//
// Created by sergi on 9/12/2023.
//

#pragma once

#include "test_execution_context.hpp"

namespace rapic::tests::utils {

class SyncedExecutionContext : public TestExecutionContext {
public:
    explicit SyncedExecutionContext() = default;
    ~SyncedExecutionContext() override = default;

    /// ExecutionContext interface
    void PostTask(std::packaged_task<void(void)> task) override;

    /// TestExecutionContext interface
    std::string GetName() override;
};

}  // namespace rapic::tests::utils
