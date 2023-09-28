//
// Created by sergi on 9/28/2023.
//

#pragma once

#include <boost/asio/ssl.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/ssl.hpp>

#include "stream.hpp"

namespace rapic {

class SslStream : public Stream {
public:
    SslStream(const boost::asio::ip::tcp::resolver::results_type& end_points, const std::string& address, boost::asio::ssl::context& ssl_context);

    ~SslStream() override;

    void Shutdown() override;

    void Write(const boost::beast::http::request<boost::beast::http::string_body>& request) override;

    boost::beast::http::response<boost::beast::http::string_body> Read() override;

private:
    boost::asio::ssl::context& ssl_context_;

    boost::asio::io_context io_context_;
    boost::beast::ssl_stream<boost::beast::tcp_stream> stream_;
};

}  // namespace rapic
