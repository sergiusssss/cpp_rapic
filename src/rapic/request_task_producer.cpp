//
// Created by sergi on 9/28/2023.
//

#include "request_task_producer.hpp"

#include <chrono>

#include <boost/beast/version.hpp>

#include "logger.hpp"

namespace rapic {

namespace {

boost::beast::http::verb Convert(const HttpMethod& method) {
    switch (method) {
    case HttpMethod::kGet:
        return boost::beast::http::verb::get;
        break;
    case HttpMethod::kPost:
        return boost::beast::http::verb::post;
        break;
    case HttpMethod::kPut:
        return boost::beast::http::verb::put;
        break;
    case HttpMethod::kDelete:
        return boost::beast::http::verb::delete_;
        break;
    }

    return boost::beast::http::verb::unknown;
}

boost::beast::http::request<boost::beast::http::string_body> Convert(const Request& request) {
    const auto path = request.path.empty() ? "/" : request.path;

    boost::beast::http::request<boost::beast::http::string_body> boost_request{Convert(request.method), path, 11};
    boost_request.set(boost::beast::http::field::user_agent, BOOST_BEAST_VERSION_STRING);

    for (const auto& header : request.headers) {
        boost_request.set(header.first, header.second);

        boost_request.insert(header.first, header.second);
    }

    boost_request.body() = request.body;

    boost_request.prepare_payload();

    return boost_request;
}

Response Convert(const boost::beast::http::response<boost::beast::http::string_body>& boost_response) {
    Response response;
    response.status_code = boost_response.result_int();
    response.status_message = boost_response.reason();

    for (const auto& header : boost_response.base()) {
        response.headers.insert(std::make_pair(header.name_string(), header.value()));
    }

    response.body = boost_response.body();

    return response;
}

}  // namespace

std::packaged_task<void(void)> RequestTaskProducer::CreateRequestTask(const Request& request, const RestClient::Callback& callback) {
    return std::packaged_task<void(void)>{[&] { return RequestTask(request, callback); }};
}

void RequestTaskProducer::RequestTask(const Request& request, const RestClient::Callback& callback) {
    const auto execution_start_time = std::chrono::steady_clock::now();

    RAPIC_TRACE("Request execution. Started. Address: {}; Service: {}; Path: {}", GetAddress(), GetService(), request.path);

    boost::asio::io_context ioc;

    boost::asio::ip::tcp::resolver resolver(ioc);

    boost::system::error_code error_code;
    const auto end_points = resolver.resolve(GetAddress(), GetService());

    if (error_code) {
        RAPIC_ERROR("Request execution. End points resolving with reason: {}", error_code.what());
        throw boost::beast::system_error{error_code};
    }
    RAPIC_TRACE("Request execution. End points resolved");

    auto stream = Connect(resolver.resolve(GetAddress(), GetService()));

    const auto connection_established_delta =
      std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - execution_start_time);

    RAPIC_TRACE("Request execution. Connection established. Elapsed time: {}ms", connection_established_delta.count());

    auto boost_request = Convert(request);
    boost_request.set(boost::beast::http::field::host, GetAddress());

    RAPIC_TRACE("Request execution. Request prepared");

    stream->Write(boost_request);

    const auto request_sent_delta =
      std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - execution_start_time);
    RAPIC_TRACE("Request execution. Request sent. Elapsed time: {}ms", request_sent_delta.count());

    const auto& boost_response = stream->Read();

    const auto response_read_delta =
      std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - execution_start_time);
    RAPIC_TRACE("Request execution. Response read. Elapsed time: {}ms", response_read_delta.count());

    const auto response = Convert(boost_response);

    RAPIC_TRACE("Request execution. Response prepared");

    callback(response);

    const auto callback_executed_delta =
      std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - execution_start_time);
    RAPIC_TRACE("Request execution. Callback executed. Elapsed time: {}ms", callback_executed_delta.count());

    stream->Shutdown();

    const auto request_executed_delta =
      std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - execution_start_time);
    RAPIC_INFO("Request execution. Succeed. Elapsed time: {}ms", request_executed_delta.count());
}

}  // namespace rapic
