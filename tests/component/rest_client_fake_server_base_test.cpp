//
// Created by sergi on 9/26/2023.
//

#include "rest_client_fake_server_base_test.hpp"

#include <thread>

#include <utility>

#include <gmock/gmock.h>

#include <rapic/https_rest_client.hpp>
#include <rapic/tests/utils/https_rest_server.hpp>

namespace rapic::tests {

RestClientFakeServerBaseTest::RestClientFakeServerBaseTest(std::shared_ptr<rapic::ExecutionContext> execution_context,
                                                           std::shared_ptr<RestClient> client, std::shared_ptr<utils::RestServer> server)
    : execution_context_(std::move(execution_context))
    , client_(std::move(client))
    , server_(std::move(server)) {}

void RestClientFakeServerBaseTest::ValidateSequence(const Request& request, const Response& response) {
    std::promise<rapic::Request> request_promise;

    auto server_execute = [&]() {
        auto server_callback = [&](const rapic::Request& request) {
            request_promise.set_value(request);

            return response;
        };

        server_->AcceptSingleConnection(server_callback);
    };
    server_thread_ = std::thread(server_execute);

    std::promise<rapic::Response> response_promise;

    std::function<void(const rapic::Response&)> client_callback = [&](const rapic::Response& response) { response_promise.set_value(response); };
    client_->SendRequest(request, client_callback);

    auto received_request_future = request_promise.get_future();
    auto received_response_future = response_promise.get_future();

    const auto received_request = received_request_future.get();
    const auto received_response = received_response_future.get();

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