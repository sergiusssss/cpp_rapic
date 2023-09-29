//
// Created by sergi on 9/12/2023.
//

#pragma once

#include <rapic/http_rest_client.hpp>

#include "request_task_producer.hpp"

namespace rapic {

class HttpRestClient::Impl : public RequestTaskProducer {
public:
    // Accept an ExecutionContext upon construction
    explicit Impl(HttpRestClient::Configuration configuration, ExecutionContext& context);
    ~Impl();

    // Send request
    void SendRequest(const Request& request, const RestClient::Callback& callback);

protected:
    const std::string& GetAddress() override;

    const std::string& GetService() override;

    std::unique_ptr<rapic::Stream>
    Connect(const boost::asio::ip::basic_resolver<boost::asio::ip::tcp, boost::asio::any_io_executor>::results_type& end_points) override;

private:
    ExecutionContext& context_;
    HttpRestClient::Configuration configuration_;
};

}  // namespace rapic