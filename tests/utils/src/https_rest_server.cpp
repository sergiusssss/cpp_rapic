//
// Created by sergi on 9/12/2023.
//

#include <rapic/tests/utils/https_rest_server.hpp>

#include <boost/asio/ip/tcp.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>

#include <spdlog/spdlog.h>

#include <rapic/tests/utils/logger.hpp>

namespace rapic::tests::utils {

namespace {

HttpMethod Convert(const boost::beast::http::verb& method) {
    switch (method) {
    case boost::beast::http::verb::get:
        return HttpMethod::kGet;
    case boost::beast::http::verb::post:
        return HttpMethod::kPost;
    case boost::beast::http::verb::put:
        return HttpMethod::kPut;
    case boost::beast::http::verb::delete_:
        return HttpMethod::kDelete;
    default:
        return HttpMethod::kGet;
    }

    return HttpMethod::kGet;
}

}  // namespace

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
    RAPIC_UTILS_INFO("Accepting single ssl connection. Started");

    boost::system::error_code error_code;

    auto socket = acceptor_.accept(error_code);

    if (error_code) {
        RAPIC_UTILS_ERROR("Accepting single ssl connection. Socket accepting failed with reason: {}", error_code.what());
        return;
    }

    RAPIC_UTILS_INFO("Accepting single ssl connection. Connection accepted");

    ProcessConnection(std::move(socket), callback);

    RAPIC_UTILS_INFO("Accepting single ssl connection. Succeed");
}

void HttpsRestServer::ProcessConnection(boost::asio::ip::tcp::socket socket, const Callback& callback) {
    boost::asio::ssl::stream<boost::asio::ip::tcp::socket> stream{std::move(socket), ssl_context_};

    RAPIC_UTILS_INFO("Processing ssl connection. Started");

    boost::system::error_code error_code;
    stream.handshake(boost::asio::ssl::stream_base::server);
    if (error_code) {
        RAPIC_UTILS_ERROR("Processing ssl connection. Sll handshake failed with reason: {}", error_code.what());
        return;
    }

    RAPIC_UTILS_INFO("Processing ssl connection. Sll handshake succeed");

    while (!error_code) {
        boost::beast::flat_buffer buffer;

        boost::beast::http::request<boost::beast::http::string_body> req;
        boost::beast::http::read(stream, buffer, req, error_code);

        if (error_code == boost::beast::http::error::end_of_stream) {
            break;
        }

        if (error_code) {
            RAPIC_UTILS_ERROR("Processing ssl connection. Request reading failed with reason: {}", error_code.what());
            break;
        }

        RAPIC_UTILS_INFO("Processing ssl connection. Request read.");

        rapic::Request request;
        request.path = req.target();
        request.body = req.body();
        request.method = Convert(req.method());

        const auto& headers = req.base();
        for (const auto& header : headers) {
            request.headers.emplace(header.name_string(), header.value());
        }

        RAPIC_UTILS_INFO("Processing ssl connection. Request prepared");

        auto response = callback(request);

        RAPIC_UTILS_INFO("Processing ssl connection. Callback executed");

        boost::beast::http::response<boost::beast::http::string_body> res{boost::beast::http::status::ok, req.version()};

        for (const auto& header : response.headers) {
            res.set(header.first, header.second);
        }

        res.body() = response.body;
        res.prepare_payload();

        RAPIC_UTILS_INFO("Processing ssl connection. Response prepared");

        boost::beast::http::write(stream, res, error_code);

        if (error_code) {
            RAPIC_UTILS_ERROR("Processing ssl connection. Response writing failed with reason: {}", error_code.what());
            break;
        }

        RAPIC_UTILS_INFO("Processing ssl connection. Response sent");
    }

    stream.next_layer().shutdown(boost::asio::ip::tcp::socket::shutdown_send, error_code);
    if (error_code) {
        RAPIC_UTILS_ERROR("Processing ssl connection. Shutting downing failed with reason: {}", error_code.what());
    }

    RAPIC_UTILS_INFO("Processing ssl connection. Succeed");
}

}  // namespace rapic::tests::utils