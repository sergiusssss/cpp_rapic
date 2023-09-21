//
// Created by sergi on 9/13/2023.
//

#include "http_request_task.hpp"

#include <spdlog/spdlog.h>

#include <boost/asio/connect.hpp>
#include <boost/asio/spawn.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>

#include <iostream>

namespace rapic {

namespace {

void HttpRequest(const std::string& base_url, const Request& request, const RestClient::Callback& callback, std::chrono::milliseconds timeout) {
    using tcp = boost::asio::ip::tcp;
    namespace http = boost::beast::http;
    namespace beast = boost::beast;

    std::stringstream ss;
    ss << std::this_thread::get_id();
    spdlog::error("task: {}", ss.str());

    spdlog::error("1");

    // We need an io_context
    boost::asio::io_context ioc;
    ioc.run();

    spdlog::error("1.5");

    // We will resolve the destination and communicate using this socket
    tcp::resolver resolver(ioc);
    // tcp::socket socket(ioc);
    beast::tcp_stream stream(ioc);

    std::string full_url = base_url + request.url;

    // Look up the domain name
    spdlog::info("base url: {}; path: {}; full url: {};", base_url, request.url, full_url);
    auto const results = resolver.resolve(base_url, "http");

    spdlog::error("2");

    // Connect to the server
    // boost::asio::connect(socket, results.begin(), results.end());
    stream.connect(results);

    spdlog::error("3");

    // Define timeout
    //    boost::asio::steady_timer timer(ioc);
    //    timer.expires_after(timeout);

    // Create request string from the Request instance
    // Since Beast does not understand our Request structure, we must manually
    // construct the HTTP message.

    // Assumed: request.method is HttpMethod::GET
    // Time and version are hard coded for simplicity
    http::request<http::empty_body> req{http::verb::get, request.url, 11};
    req.set(http::field::host, base_url);
    req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);

    // Adding headers to HTTP request
    for (const auto& header : request.headers) {
        req.set(header.first, header.second);
    }

    // Send HTTP request
    http::write(stream, req);

    // This buffer will be used to hold the response
    boost::beast::flat_buffer buffer;

    // Response container
    http::response<http::dynamic_body> res;

    // Receive HTTP response
    http::read(stream, buffer, res);

    // Prepare our custom Response
    Response response;
    response.status_code = res.result_int();
    response.status_message = res.reason();

    for (const auto& header : res.base()) {
        response.headers.insert(std::make_pair(std::to_string(static_cast<std::uint32_t>(header.name())), header.value()));
    }

    response.body = boost::beast::buffers_to_string(res.body().data());

    spdlog::error("before callback");

    // shutdown
    stream.socket().shutdown(tcp::socket::shutdown_both);
    {
        callback(response);
    }

    spdlog::error("final");
}

}  // namespace

std::packaged_task<void(void)>
CreateHttpRequestTask(const std::string& base_url, const Request& request, const RestClient::Callback& callback, std::chrono::milliseconds timeout) {
    return std::packaged_task<void(void)>{[&request, &callback, &timeout, &base_url] { return HttpRequest(base_url, request, callback, timeout); }};
}
}  // namespace rapic
