//
// Created by sergi on 9/29/2023.
//

#pragma once

#include <rapic/execution_context.hpp>
#include <rapic/https_rest_client.hpp>

#include <rapic/tests/utils/https_rest_server.hpp>

#include "environment_holder.hpp"

namespace rapic::tests {

class HttpsEnvironmentHolder : public EnvironmentHolder {
public:
    explicit HttpsEnvironmentHolder(const std::shared_ptr<rapic::ExecutionContext>& execution_context,
                                    const utils::HttpsRestServer::Configuration& configuration);

    std::shared_ptr<rapic::ExecutionContext> GetExecutionContext() override;

    std::shared_ptr<rapic::RestClient> GetClient() override;

    std::shared_ptr<utils::RestServer> GetServer() override;

    std::string GetName() override;

private:
    std::shared_ptr<rapic::ExecutionContext> execution_context_;

    std::shared_ptr<rapic::HttpsRestClient> client_;
    std::shared_ptr<utils::HttpsRestServer> server_;
};

}  // namespace rapic::tests
