//
// Created by sergi on 9/12/2023.
//

#include "https_rest_client_impl.hpp"

#include "https_request_task.hpp"

namespace rapic {

HttpsRestClient::Impl::Impl(std::string base_url, ExecutionContext& context)
    : context_(context)
    , base_url_(std::move(base_url)) {}

HttpsRestClient::Impl::~Impl() = default;

void HttpsRestClient::Impl::SendRequest(const Request& request, RestClient::Callback callback, std::chrono::milliseconds timeout) {
    context_.PostTask(CreateHttpsRequestTask(base_url_, request, callback, timeout));
}

}  // namespace rapic
