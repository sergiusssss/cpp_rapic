//
// Created by sergi on 9/29/2023.
//

#include "http_environment_holder.hpp"

namespace rapic::tests {

HttpEnvironmentHolder::HttpEnvironmentHolder(const std::shared_ptr<rapic::ExecutionContext>& execution_context,
                                             const utils::HttpRestServer::Configuration& configuration)
    : execution_context_(execution_context)
    , client_(std::make_shared<rapic::HttpRestClient>(HttpRestClient::Configuration{configuration.address, std::to_string(configuration.service)},
                                                      *execution_context_))
    , server_(std::make_shared<utils::HttpRestServer>(configuration)) {}

std::shared_ptr<rapic::ExecutionContext> HttpEnvironmentHolder::GetExecutionContext() { return execution_context_; }

std::shared_ptr<rapic::RestClient> HttpEnvironmentHolder::GetClient() { return client_; }

std::shared_ptr<utils::RestServer> HttpEnvironmentHolder::GetServer() { return server_; }

std::string HttpEnvironmentHolder::GetName() { return "Http"; }

}  // namespace rapic::tests
