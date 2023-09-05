//
// Created by sergi on 9/5/2023.
//

#include <gtest/gtest.h>

#include <rapic/thread_pool_context.hpp>

TEST(ThreadPoolContextTest, BaseTaskExecution) {
    rapic::ThreadPoolContext context{1};

    bool executed = false;
    auto task = [&]() { executed = true; };

    context.PostTask(std::make_unique<std::function<void()>>(task));

    context.Start();

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    EXPECT_TRUE(executed);
}