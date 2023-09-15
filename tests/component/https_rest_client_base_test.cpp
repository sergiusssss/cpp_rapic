//
// Created by sergi on 8/26/2023.
//

#include <gtest/gtest.h>

#include <rapic/https_rest_client.hpp>

#include <rapic/tests/utils/synced_execution_context.hpp>

class HttpsRestClientBaseTest : public testing::TestWithParam<std::shared_ptr<rapic::ExecutionContext>> {
public:
    explicit HttpsRestClientBaseTest(std::string base_url)
        : context_(GetParam())
        , client_(std::move(base_url), *context_) {}

protected:
    std::shared_ptr<rapic::ExecutionContext> context_;
    rapic::HttpsRestClient client_;
};

class HttpsRestClientEmptyUrlTest : public HttpsRestClientBaseTest {
public:
    explicit HttpsRestClientEmptyUrlTest()
        : HttpsRestClientBaseTest("") {}
};

class HttpsRestClientReqResUrlTest : public HttpsRestClientBaseTest {
public:
    explicit HttpsRestClientReqResUrlTest()
        : HttpsRestClientBaseTest("reqres.in") {}
};

TEST_P(HttpsRestClientEmptyUrlTest, BaseRequestExecution) {
    client_.SendRequest(
      rapic::Request{}, [](const rapic::Response&) {}, std::chrono::milliseconds{10});

    EXPECT_TRUE(true);
}

TEST_P(HttpsRestClientReqResUrlTest, BaseRequestExecution) {
    rapic::Request request{
      .method = rapic::HttpMethod::kGet,
      .url = "/api/users",
      .headers = {},
      .body = "",
    };

    std::promise<rapic::Response> response_promise;

    auto callback = [&response_promise](const rapic::Response& res) { response_promise.set_value(res); };
    auto response_future = response_promise.get_future();

    std::cout << "main:" << std::this_thread::get_id() << std::endl;

    client_.SendRequest(request, callback, std::chrono::milliseconds{10});

    std::cout << "shit" << std::endl;

    const auto response = response_future.get();
    EXPECT_EQ(response.status_code, 200);
    EXPECT_EQ(response.status_message, "");
}

INSTANTIATE_TEST_SUITE_P(ExecutionContexts, HttpsRestClientEmptyUrlTest,
                         testing::Values(std::make_shared<rapic::tests::utils::SyncedExecutionContext>()));
INSTANTIATE_TEST_SUITE_P(ExecutionContexts, HttpsRestClientReqResUrlTest,
                         testing::Values(std::make_shared<rapic::tests::utils::SyncedExecutionContext>()));