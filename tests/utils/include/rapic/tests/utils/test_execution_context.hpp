//
// Created by sergi on 9/12/2023.
//

#pragma once

#include <rapic/execution_context.hpp>

namespace rapic::tests::utils {

class TestExecutionContext : public ExecutionContext {
public:
    virtual std::string GetName() = 0;
};

}  // namespace rapic::tests::utils
