//
// Created by sergi on 8/26/2023.
//

#include <gtest/gtest.h>

#include <rapic/http_rest_client.hpp>
#include <rapic/thread_pool_context.hpp>

TEST(ClientTest, BaseRequestExecution) {
    rapic::ThreadPoolContext context;

    rapic::HttpRestClient client{context, ""};

    client.SendRequest(
      rapic::Request{}, [](const rapic::Response&) {}, std::chrono::milliseconds{10});

    EXPECT_TRUE(true);
}
