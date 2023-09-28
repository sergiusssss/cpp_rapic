//
// Created by sergi on 9/26/2023.
//

#include "https_rest_client_fake_server_base_test.hpp"

#include <thread>

#include <utility>

#include <gmock/gmock.h>

#include <rapic/https_rest_client.hpp>
#include <rapic/tests/utils/https_rest_server.hpp>

namespace rapic::tests {

HttpsRestClientFakeServerBaseTest::HttpsRestClientFakeServerBaseTest(std::shared_ptr<rapic::ExecutionContext> execution_context,
                                                                     const utils::HttpsRestServer::Configuration& configuration)
    : execution_context_(std::move(execution_context))
    , client_({configuration.address, std::to_string(configuration.service)}, *execution_context_)
    , server_(configuration) {}

void HttpsRestClientFakeServerBaseTest::ValidateSequence(const Request& request, const Response& response) {
    std::promise<rapic::Request> request_promise;

    auto server_execute = [&]() {
        auto server_callback = [&](const rapic::Request& request) {
            request_promise.set_value(request);

            return response;
        };

        server_.AcceptSingleConnection(server_callback);
    };
    server_thread_ = std::thread(server_execute);

    std::promise<rapic::Response> response_promise;
    auto client_callback = [&](const rapic::Response& response) { response_promise.set_value(response); };
    client_.SendRequest(request, client_callback);

    const auto received_request = request_promise.get_future().get();
    const auto received_response = response_promise.get_future().get();

    server_thread_.join();

    EXPECT_EQ(request.path, received_request.path);
    EXPECT_EQ(request.body, received_request.body);
    EXPECT_EQ(request.method, received_request.method);
    EXPECT_THAT(request.headers, testing::ElementsAreArray(received_request.headers));

    EXPECT_EQ(response.body, received_response.body);
    EXPECT_EQ(response.status_message, received_response.status_message);
    EXPECT_EQ(response.status_code, received_response.status_code);
    EXPECT_THAT(response.headers, testing::ElementsAreArray(received_response.headers));
}

}  // namespace rapic::tests