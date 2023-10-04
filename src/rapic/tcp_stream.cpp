//
// Created by sergi on 9/28/2023.
//

#include "tcp_stream.hpp"

#include "logger.hpp"

namespace rapic {

TcpStream::TcpStream(const boost::asio::ip::tcp::resolver::results_type& end_points)
    : io_context_()
    , stream_(io_context_) {
    RAPIC_TRACE("Tcp connection establishing. Started");

    boost::system::error_code error_code;
    stream_.connect(end_points, error_code);

    if (error_code) {
        RAPIC_ERROR("Tcp connection establishing. Socket connection failed with reason: {}", error_code.what());
        throw boost::beast::system_error{error_code};
    }

    RAPIC_TRACE("Tcp connection establishing. Succeed");
}

TcpStream::~TcpStream() = default;

void TcpStream::Shutdown() {
    RAPIC_TRACE("Tcp connection shutting downing. Started");

    boost::system::error_code error_code;
    stream_.socket().shutdown(boost::asio::ip::tcp::socket::shutdown_both, error_code);

    if (error_code) {
        RAPIC_ERROR("Tcp connection shutting downing. Failed with reason: {}", error_code.what());
        throw boost::beast::system_error{error_code};
    }

    RAPIC_TRACE("Tcp connection shutting downing. Succeed");
}

void TcpStream::Write(const boost::beast::http::request<boost::beast::http::string_body>& request) {
    RAPIC_TRACE("Tcp stream writing. Started");

    boost::system::error_code error_code;
    boost::beast::http::write(stream_, request, error_code);

    if (error_code) {
        RAPIC_ERROR("Tcp stream writing. Failed with reason: {}", error_code.what());
        throw boost::beast::system_error{error_code};
    }

    RAPIC_TRACE("Tcp stream writing. Succeed");
}

boost::beast::http::response<boost::beast::http::string_body> TcpStream::Read() {
    RAPIC_TRACE("Tcp stream reading. Started");

    boost::beast::flat_buffer buffer;

    boost::beast::http::response<boost::beast::http::string_body> response;

    boost::system::error_code error_code;
    boost::beast::http::read(stream_, buffer, response, error_code);
    if (error_code) {
        RAPIC_ERROR("Tcp stream reading. Failed with reason: {}", error_code.what());
        throw boost::beast::system_error{error_code};
    }

    RAPIC_TRACE("Tcp stream reading. Succeed");
    return response;
}

}  // namespace rapic