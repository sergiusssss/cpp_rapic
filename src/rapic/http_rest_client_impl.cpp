//
// Created by sergi on 9/12/2023.
//

#include "http_rest_client_impl.hpp"

#include "http_request_task.hpp"

namespace rapic {

HttpRestClient::Impl::Impl(std::string base_url, ExecutionContext& context)
    : context_(context)
    , base_url_(std::move(base_url)) {}

HttpRestClient::Impl::~Impl() = default;

void HttpRestClient::Impl::SendRequest(const Request& request, RestClient::Callback callback, std::chrono::milliseconds timeout) {
    context_.PostTask(CreateHttpRequestTask(base_url_, request, callback, timeout));
}

}  // namespace rapic
