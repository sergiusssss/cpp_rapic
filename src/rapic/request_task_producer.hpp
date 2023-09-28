//
// Created by sergi on 9/28/2023.
//

#pragma once

#include <future>

#include <rapic/rest_client.hpp>

#include <boost/asio/ip/tcp.hpp>

#include "stream.hpp"

namespace rapic {

class RequestTaskProducer {
public:
    virtual ~RequestTaskProducer() = default;

    std::packaged_task<void(void)> CreateRequestTask(const Request& request, const RestClient::Callback& callback);

protected:
    virtual const std::string& GetAddress() = 0;
    virtual const std::string& GetService() = 0;

    virtual std::unique_ptr<rapic::Stream> Connect(const boost::asio::ip::tcp::resolver::results_type& end_points) = 0;

private:
    void RequestTask(const Request& request, const RestClient::Callback& callback);

};

}  // namespace rapic
