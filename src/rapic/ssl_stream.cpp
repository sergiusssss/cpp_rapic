//
// Created by sergi on 9/28/2023.
//

#include "ssl_stream.hpp"

#include <boost/asio/connect.hpp>

#include "logger.hpp"

namespace rapic {

SslStream::SslStream(const boost::asio::ip::tcp::resolver::results_type& end_points, const std::string& address,
                     boost::asio::ssl::context& ssl_context)
    : ssl_context_(ssl_context)
    , io_context_()
    , stream_(io_context_, ssl_context_) {
    RAPIC_TRACE("Ssl connection establishing. Started");

    boost::system::error_code error_code;
    boost::beast::get_lowest_layer(stream_).connect(end_points, error_code);

    if (error_code) {
        RAPIC_ERROR("Ssl connection establishing. Socket connection failed with reason: {}", error_code.what());
        throw boost::beast::system_error{error_code};
    }

    RAPIC_TRACE("Ssl connection establishing. Socket connected");

    if (!SSL_set_tlsext_host_name(stream_.native_handle(), address.c_str())) {
        error_code = {static_cast<int>(::ERR_get_error()), boost::asio::error::get_ssl_category()};
        RAPIC_ERROR("Ssl connection establishing. Ssl host name configuration failed with reason: {}", error_code.what());
        throw boost::beast::system_error{error_code};
    }

    RAPIC_TRACE("Ssl connection establishing. Ssl host name configured");

    stream_.handshake(boost::asio::ssl::stream_base::client, error_code);
    if (error_code) {
        RAPIC_ERROR("Ssl connection establishing. Ssl handshake failed with reason: {}", error_code.what());
        throw boost::beast::system_error{error_code};
    }

    RAPIC_TRACE("Ssl connection establishing. Ssl handshake succeed");
    RAPIC_TRACE("Ssl connection establishing. Succeed");
}

SslStream::~SslStream() = default;

void SslStream::Shutdown() {
    RAPIC_TRACE("Ssl connection shutting downing. Started");

    boost::system::error_code error_code;
    stream_.shutdown(error_code);

    if (error_code && error_code.category() == boost::asio::error::get_ssl_category()) {
        RAPIC_ERROR("Ssl connection shutting downing. Failed with reason: {}", error_code.what());
        throw boost::beast::system_error{error_code};
    }

    RAPIC_TRACE("Ssl connection shutting downing. Succeed");
}

void SslStream::Write(const boost::beast::http::request<boost::beast::http::string_body>& request) {
    RAPIC_TRACE("Ssl stream writing. Started");

    boost::system::error_code error_code;
    boost::beast::http::write(stream_, request, error_code);

    if (error_code) {
        RAPIC_ERROR("Ssl stream writing. Failed with reason: {}", error_code.what());
        throw boost::beast::system_error{error_code};
    }

    RAPIC_TRACE("Ssl stream writing. Succeed");
}

boost::beast::http::response<boost::beast::http::string_body> SslStream::Read() {
    RAPIC_TRACE("Ssl stream reading. Started");

    boost::beast::flat_buffer buffer;

    boost::beast::http::response<boost::beast::http::string_body> response;

    boost::system::error_code error_code;
    boost::beast::http::read(stream_, buffer, response, error_code);
    if (error_code) {
        RAPIC_ERROR("Ssl stream reading. Failed with reason: {}", error_code.what());
        throw boost::beast::system_error{error_code};
    }

    RAPIC_TRACE("Ssl stream reading. Succeed");
    return response;
}

}  // namespace rapic