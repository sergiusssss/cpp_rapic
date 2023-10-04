//
// Created by sergi on 9/12/2023.
//

#include "http_rest_client_impl.hpp"

#include "tcp_stream.hpp"

namespace rapic {

HttpRestClient::Impl::Impl(HttpRestClient::Configuration configuration, ExecutionContext& context)
    : context_(context)
    , configuration_(std::move(configuration)) {}

HttpRestClient::Impl::~Impl() = default;

void HttpRestClient::Impl::SendRequest(const Request& request, const RestClient::Callback& callback) {
    context_.PostTask(CreateRequestTask(request, callback));
}

const std::string& HttpRestClient::Impl::GetAddress() { return configuration_.address; }

const std::string& HttpRestClient::Impl::GetService() { return configuration_.service; }

std::unique_ptr<rapic::Stream>
HttpRestClient::Impl::Connect(const boost::asio::ip::basic_resolver<boost::asio::ip::tcp, boost::asio::any_io_executor>::results_type& end_points) {
    return std::make_unique<rapic::TcpStream>(end_points);
}

}  // namespace rapic
