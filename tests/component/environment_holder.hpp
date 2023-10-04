//
// Created by sergi on 9/29/2023.
//

#pragma once

#include <rapic/execution_context.hpp>
#include <rapic/rest_client.hpp>

#include <rapic/tests/utils/rest_server.hpp>

namespace rapic::tests {

class EnvironmentHolder {
public:
    virtual std::shared_ptr<rapic::ExecutionContext> GetExecutionContext() = 0;

    virtual std::shared_ptr<rapic::RestClient> GetClient() = 0;

    virtual std::shared_ptr<utils::RestServer> GetServer() = 0;

    virtual std::string GetName() = 0;
};

}  // namespace rapic::tests
