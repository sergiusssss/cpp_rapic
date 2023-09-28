//
// Created by sergi on 9/28/2023.
//

#include "tcp_stream.hpp"

namespace rapic {

TcpStream::TcpStream(const boost::asio::ip::tcp::resolver::results_type& end_points)
    : io_context_()
    , stream_(io_context_) {
    stream_.connect(end_points);
}

TcpStream::~TcpStream() { TcpStream::Shutdown(); }

void TcpStream::Shutdown() { stream_.socket().shutdown(boost::asio::ip::tcp::socket::shutdown_both); }

void TcpStream::Write(const boost::beast::http::request<boost::beast::http::string_body>& request) { boost::beast::http::write(stream_, request); }

boost::beast::http::response<boost::beast::http::string_body> TcpStream::Read() {
    // This buffer will be used to hold the response
    boost::beast::flat_buffer buffer;

    // Response container
    boost::beast::http::response<boost::beast::http::string_body> response;

    // Receive HTTP response
    boost::beast::http::read(stream_, buffer, response);

    return response;
}

}  // namespace rapic