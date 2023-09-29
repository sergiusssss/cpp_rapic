//
// Created by sergi on 9/12/2023.
//

#pragma once

#include <functional>
#include <string>

#include <boost/asio/ip/tcp.hpp>

#include <rapic/request.hpp>
#include <rapic/response.hpp>

#include "rest_server.hpp"

namespace rapic::tests::utils {

class HttpRestServer : public RestServer {
public:
    struct Configuration {
        std::string address;
        std::uint32_t service;
    };

    explicit HttpRestServer(const Configuration& configuration);
    ~HttpRestServer() override = default;

    void AcceptSingleConnection(const Callback& callback) override;

private:
    void ProcessConnection(boost::asio::ip::tcp::socket socket, const Callback& callback);

    boost::asio::io_context io_context_;
    boost::asio::ip::tcp::endpoint end_point_;
    boost::asio::ip::tcp::acceptor acceptor_;
};

}  // namespace rapic::tests::utils
