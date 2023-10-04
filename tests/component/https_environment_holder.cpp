//
// Created by sergi on 9/29/2023.
//

#include "https_environment_holder.hpp"

namespace rapic::tests {

HttpsEnvironmentHolder::HttpsEnvironmentHolder(const std::shared_ptr<rapic::ExecutionContext>& execution_context,
                                               const utils::HttpsRestServer::Configuration& configuration)
    : execution_context_(execution_context)
    , client_(std::make_shared<rapic::HttpsRestClient>(HttpsRestClient::Configuration{configuration.address, std::to_string(configuration.service)},
                                                       *execution_context_))
    , server_(std::make_shared<utils::HttpsRestServer>(configuration)) {}

std::shared_ptr<rapic::ExecutionContext> HttpsEnvironmentHolder::GetExecutionContext() { return execution_context_; }

std::shared_ptr<rapic::RestClient> HttpsEnvironmentHolder::GetClient() { return client_; }

std::shared_ptr<utils::RestServer> HttpsEnvironmentHolder::GetServer() { return server_; }

std::string HttpsEnvironmentHolder::GetName() { return "Https"; }

}  // namespace rapic::tests
