//
// Created by sergi on 8/26/2023.
//

#include <gtest/gtest.h>

#include <rapic/rest_api_client.hpp>

TEST(ClientTest, BaseRequestExecution) {
    rapic::RestApiClient client;

    client.execute(rapic::Request{});

    EXPECT_TRUE(true);
}
