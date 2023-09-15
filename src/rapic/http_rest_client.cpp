//
// Created by sergi on 8/26/2023.
//

#include <rapic/http_rest_client.hpp>

#include "http_rest_client_impl.hpp"

namespace rapic {

HttpRestClient::HttpRestClient(std::string base_url, rapic::ExecutionContext& context)
    : impl_(std::make_unique<Impl>(std::move(base_url), context)) {}

HttpRestClient::~HttpRestClient() = default;

void HttpRestClient::SendRequest(const Request& request, Callback callback, std::chrono::milliseconds timeout) {
    impl_->SendRequest(request, callback, timeout);
}

const std::string& HttpRestClient::GetBaseUrl() const { return impl_->GetBaseUrl(); }

void HttpRestClient::SetBaseUrl(const std::string& base_url) { impl_->SetBaseUrl(base_url); }

ExecutionContext& HttpRestClient::Context() { return impl_->Context(); }

}  // namespace rapic
