//
// Created by sergi on 9/28/2023.
//

#pragma once

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>

#include "stream.hpp"

namespace rapic {

class TcpStream : public Stream {
public:
    TcpStream(const boost::asio::ip::tcp::resolver::results_type& end_points);

    ~TcpStream() override;

    void Shutdown() override;

    void Write(const boost::beast::http::request<boost::beast::http::string_body>& request) override;

    boost::beast::http::response<boost::beast::http::string_body> Read() override;

private:
    boost::asio::io_context io_context_;
    boost::beast::tcp_stream stream_;
};

}  // namespace rapic
