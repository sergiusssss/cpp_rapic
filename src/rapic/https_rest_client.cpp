//
// Created by sergi on 8/26/2023.
//

#include <rapic/https_rest_client.hpp>

#include "https_rest_client_impl.hpp"

namespace rapic {

HttpsRestClient::HttpsRestClient(std::string base_url, rapic::ExecutionContext& context)
    : impl_(std::make_unique<Impl>(std::move(base_url), context)) {}

HttpsRestClient::~HttpsRestClient() = default;

void HttpsRestClient::SendRequest(const Request& request, Callback callback, std::chrono::milliseconds timeout) {
    impl_->SendRequest(request, callback, timeout);
}

const std::string& HttpsRestClient::GetBaseUrl() const { return impl_->GetBaseUrl(); }

void HttpsRestClient::SetBaseUrl(const std::string& base_url) { impl_->SetBaseUrl(base_url); }

ExecutionContext& HttpsRestClient::Context() { return impl_->Context(); }

}  // namespace rapic
