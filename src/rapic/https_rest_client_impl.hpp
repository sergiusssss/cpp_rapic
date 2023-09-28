//
// Created by sergi on 9/12/2023.
//

#pragma once

#include <rapic/https_rest_client.hpp>

#include <boost/asio/ssl.hpp>

#include "logger.hpp"
#include "request_task_producer.hpp"

namespace rapic {

class HttpsRestClient::Impl : RequestTaskProducer {
public:
    // Accept an ExecutionContext upon construction
    explicit Impl(HttpsRestClient::Configuration configuration, ExecutionContext& context);
    ~Impl() override = default;

    // Send request
    void SendRequest(const Request& request, const Callback& callback);

private:
    const std::string& GetAddress() override;

    const std::string& GetService() override;

    std::unique_ptr<rapic::Stream>
    Connect(const boost::asio::ip::basic_resolver<boost::asio::ip::tcp, boost::asio::any_io_executor>::results_type& end_points) override;

private:
    ExecutionContext& context_;
    HttpsRestClient::Configuration configuration_;

    boost::asio::io_context io_context_;
    boost::asio::ssl::context ssl_context_;
};

}  // namespace rapic