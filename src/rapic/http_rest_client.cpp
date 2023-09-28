//
// Created by sergi on 8/26/2023.
//

#include <rapic/http_rest_client.hpp>

#include "http_rest_client_impl.hpp"

namespace rapic {

HttpRestClient::HttpRestClient(std::string base_url, rapic::ExecutionContext& context)
    : impl_(std::make_unique<Impl>(std::move(base_url), context)) {}

HttpRestClient::~HttpRestClient() = default;

void HttpRestClient::SendRequest(const Request& request, Callback callback) { impl_->SendRequest(request, callback); }

const std::string& HttpRestClient::GetAddress() const { return impl_->GetAddress(); }

ExecutionContext& HttpRestClient::Context() { return impl_->Context(); }

}  // namespace rapic
