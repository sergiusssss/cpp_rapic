//
// Created by sergi on 9/12/2023.
//

#pragma once

#include <functional>
#include <string>

#include <boost/beast/ssl.hpp>

#include <rapic/request.hpp>
#include <rapic/response.hpp>

namespace rapic::tests::utils {

class HttpsRestServer {
public:
    struct Configuration {
        std::string address;
        std::uint32_t service;

        std::string certificate_chain_file_path;
        std::string private_key_file_path;
    };

    using Callback = std::function<Response(Request)>;

    explicit HttpsRestServer(const Configuration& configuration);
    ~HttpsRestServer() = default;

    void AcceptSingleConnection(const Callback& callback);

private:
    void ProcessConnection(boost::asio::ip::tcp::socket socket, const Callback& callback);

    boost::asio::io_context io_context_;
    boost::asio::ssl::context ssl_context_;
    boost::asio::ip::tcp::endpoint end_point_;
    boost::asio::ip::tcp::acceptor acceptor_;
};

}  // namespace rapic::tests::utils
