//
// Created by sergi on 9/12/2023.
//

#include <rapic/tests/utils/http_rest_server.hpp>

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
    }

    return HttpMethod::kGet;
}

}  // namespace

HttpRestServer::HttpRestServer(const Configuration& configuration)
    : io_context_()
    , end_point_(boost::asio::ip::make_address(configuration.address), static_cast<boost::asio::ip::port_type>(configuration.service))
    , acceptor_(io_context_, end_point_) {}

void HttpRestServer::AcceptSingleConnection(const Callback& callback) {
    RAPIC_UTILS_INFO("Accepting single tcp connection. Started");

    boost::system::error_code error_code;

    auto socket = acceptor_.accept(error_code);

    if (error_code) {
        RAPIC_UTILS_ERROR("Accepting single tcp connection. Socket accepting failed with reason: {}", error_code.what());
        return;
    }

    RAPIC_UTILS_INFO("Accepting single tcp connection. Connection accepted");

    ProcessConnection(std::move(socket), callback);

    RAPIC_UTILS_INFO("Accepting single tcp connection. Succeed");
}

void HttpRestServer::ProcessConnection(boost::asio::ip::tcp::socket socket, const Callback& callback) {
    RAPIC_UTILS_INFO("Processing tcp connection. Started");

    boost::system::error_code error_code;

    while (!error_code) {
        boost::beast::flat_buffer buffer;

        boost::beast::http::request<boost::beast::http::string_body> req;
        boost::beast::http::read(socket, buffer, req, error_code);

        if (error_code == boost::beast::http::error::end_of_stream) {
            break;
        }

        if (error_code) {
            RAPIC_UTILS_ERROR("Processing tcp connection. Request reading failed with reason: {}", error_code.what());
            break;
        }

        RAPIC_UTILS_INFO("Processing tcp connection. Request read.");

        rapic::Request request;
        request.path = req.target();
        request.body = req.body();
        request.method = Convert(req.method());

        const auto& headers = req.base();
        for (const auto& header : headers) {
            request.headers.emplace(header.name_string(), header.value());
        }

        RAPIC_UTILS_INFO("Processing tcp connection. Request prepared");

        auto response = callback(request);

        RAPIC_UTILS_INFO("Processing tcp connection. Callback executed");

        boost::beast::http::response<boost::beast::http::string_body> res{boost::beast::http::status::ok, req.version()};

        for (const auto& header : response.headers) {
            res.set(header.first, header.second);
        }

        res.body() = response.body;
        res.prepare_payload();

        RAPIC_UTILS_INFO("Processing tcp connection. Response prepared");

        boost::beast::http::write(socket, res, error_code);

        if (error_code) {
            RAPIC_UTILS_ERROR("Processing tcp connection. Response writing failed with reason: {}", error_code.what());
            break;
        }

        RAPIC_UTILS_INFO("Processing tcp connection. Response sent");
    }

    socket.shutdown(boost::asio::ip::tcp::socket::shutdown_send, error_code);
    if (error_code) {
        RAPIC_UTILS_ERROR("Processing tcp connection. Shutting downing failed with reason: {}", error_code.what());
    }

    RAPIC_UTILS_INFO("Processing tcp connection. Succeed");
}

}  // namespace rapic::tests::utils