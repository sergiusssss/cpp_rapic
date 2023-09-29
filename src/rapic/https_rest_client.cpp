//
// Created by sergi on 8/26/2023.
//

#include <rapic/https_rest_client.hpp>

#include "https_rest_client_impl.hpp"

#include "logger.hpp"

namespace rapic {

HttpsRestClient::Configuration::Configuration(const std::string& address, const std::string& service)
    : address(address)
    , service(service) {}

HttpsRestClient::HttpsRestClient(const Configuration& configuration, rapic::ExecutionContext& context)
    : impl_(std::make_unique<Impl>(configuration, context)) {}

HttpsRestClient::~HttpsRestClient() = default;

void HttpsRestClient::SendRequest(const Request& request, const Callback& callback) {
    impl_->SendRequest(request, callback);
}

}  // namespace rapic
