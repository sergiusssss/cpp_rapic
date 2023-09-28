//
// Created by sergi on 9/12/2023.
//

#include <rapic/tests/utils/https_rest_server.hpp>

#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/strand.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>

#include <spdlog/spdlog.h>

#include <rapic/tests/utils/logger.hpp>

namespace rapic::tests::utils {

HttpsRestServer::HttpsRestServer(const Configuration& configuration)
    : io_context_()
    , ssl_context_(boost::asio::ssl::context::sslv23_server)
    , end_point_(boost::asio::ip::make_address(configuration.address), static_cast<boost::asio::ip::port_type>(configuration.service))
    , acceptor_(io_context_, end_point_) {
    if (!configuration.certificate_chain_file_path.empty()) {
        RAPIC_UTILS_INFO("Used certificate chain file: {}", configuration.certificate_chain_file_path);
        ssl_context_.use_certificate_chain_file(configuration.certificate_chain_file_path);
    }
    if (!configuration.private_key_file_path.empty()) {
        RAPIC_UTILS_INFO("Used private key file: {}", configuration.private_key_file_path);
        ssl_context_.use_private_key_file(configuration.private_key_file_path, boost::asio::ssl::context::pem);
    }
}

void HttpsRestServer::AcceptSingleConnection(const Callback& callback) {
    RAPIC_UTILS_INFO("Accepting single connection. Started");

    boost::system::error_code error_code;

    auto socket = acceptor_.accept(error_code);

    if (error_code) {
        RAPIC_UTILS_ERROR("Accepting single connection. Socket accepting failed with reason: {}", error_code.what());
        return;
    }

    RAPIC_UTILS_INFO("Accepting single connection. Connection accepted");

    ProcessConnection(std::move(socket), callback);

    RAPIC_UTILS_INFO("Accepting single connection. Succeed");
}

void HttpsRestServer::ProcessConnection(boost::asio::ip::tcp::socket socket, const Callback& callback) {
    boost::asio::ssl::stream<boost::asio::ip::tcp::socket> stream{std::move(socket), ssl_context_};

    RAPIC_UTILS_INFO("Processing connection. Started");

    boost::system::error_code error_code;
    stream.handshake(boost::asio::ssl::stream_base::server);
    if (error_code) {
        RAPIC_UTILS_ERROR("Processing connection. Sll handshake failed with reason: {}", error_code.what());
        return;
    }

    RAPIC_UTILS_INFO("Processing connection. Sll handshake succeed");

    while (!error_code) {
        boost::beast::flat_buffer buffer;

        boost::beast::http::request<boost::beast::http::string_body> req;
        boost::beast::http::read(stream, buffer, req, error_code);

        if (error_code == boost::beast::http::error::end_of_stream) {
            break;
        }

        if (error_code) {
            RAPIC_UTILS_ERROR("Processing connection. Request reading failed with reason: {}", error_code.what());
            break;
        }

        RAPIC_UTILS_INFO("Processing connection. Request read.");

        rapic::Request request;
        request.path = req.target();
        request.body = req.body();
        request.method = rapic::HttpMethod::kGet;

        const auto& headers = req.base();
        for (const auto& header : headers) {
            request.headers.emplace(header.name_string(), header.value());
        }

        RAPIC_UTILS_INFO("Processing connection. Request prepared");

        auto response = callback(request);

        RAPIC_UTILS_INFO("Processing connection. Callback executed");

        boost::beast::http::response<boost::beast::http::string_body> res{boost::beast::http::status::ok, req.version()};

        for (const auto& header : response.headers) {
            res.set(header.first, header.second);
        }

        res.body() = response.body;
        res.prepare_payload();

        RAPIC_UTILS_INFO("Processing connection. Response prepared");

        boost::beast::http::write(stream, res, error_code);

        if (error_code) {
            RAPIC_UTILS_ERROR("Processing connection. Response writing failed with reason: {}", error_code.what());
            break;
        }

        RAPIC_UTILS_INFO("Processing connection. Response sent");
    }

    stream.next_layer().shutdown(boost::asio::ip::tcp::socket::shutdown_both, error_code);
    if (error_code) {
        RAPIC_UTILS_ERROR("Processing connection. Shutting downing failed with reason: {}", error_code.what());
    }

    RAPIC_UTILS_INFO("Processing connection. Succeed");
}

}  // namespace rapic::tests::utils