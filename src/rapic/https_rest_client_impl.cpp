//
// Created by sergi on 9/12/2023.
//

#include "https_rest_client_impl.hpp"

#include <utility>

#include "ssl_stream.hpp"

namespace rapic {

HttpsRestClient::Impl::Impl(HttpsRestClient::Configuration configuration, ExecutionContext& context)
    : context_(context)
    , configuration_(std::move(configuration))
    , io_context_()
    , ssl_context_(boost::asio::ssl::context::sslv23_client) {
    RAPIC_TRACE("HttpsRestClient initialised");
}

void HttpsRestClient::Impl::SendRequest(const Request& request, const RestClient::Callback& callback) {
    context_.PostTask(CreateRequestTask(request, callback));
}

const std::string& HttpsRestClient::Impl::GetAddress() { return configuration_.address; }

const std::string& HttpsRestClient::Impl::GetService() { return configuration_.service; }

std::unique_ptr<rapic::Stream>
HttpsRestClient::Impl::Connect(const boost::asio::ip::basic_resolver<boost::asio::ip::tcp, boost::asio::any_io_executor>::results_type& end_points) {
    return std::make_unique<rapic::SslStream>(end_points, GetAddress(), ssl_context_);
}

}  // namespace rapic
