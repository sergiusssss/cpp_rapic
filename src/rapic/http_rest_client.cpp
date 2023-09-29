//
// Created by sergi on 8/26/2023.
//

#include <rapic/http_rest_client.hpp>

#include "http_rest_client_impl.hpp"

namespace rapic {

HttpRestClient::HttpRestClient(const Configuration& configuration, rapic::ExecutionContext& context)
    : impl_(std::make_unique<Impl>(std::move(configuration), context)) {}

HttpRestClient::~HttpRestClient() = default;

void HttpRestClient::SendRequest(const Request& request, const RestClient::Callback& callback) { impl_->SendRequest(request, callback); }

}  // namespace rapic
