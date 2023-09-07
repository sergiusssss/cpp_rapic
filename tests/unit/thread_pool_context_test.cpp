//
// Created by sergi on 9/5/2023.
//

#include <thread>

#include <gtest/gtest.h>

#include <rapic/thread_pool_context.hpp>

class ThreadPoolContextTest : public testing::TestWithParam<std::uint16_t> {
public:
    ThreadPoolContextTest()
        : context_(GetParam()) {}

protected:
    rapic::ThreadPoolContext context_;
};

TEST_P(ThreadPoolContextTest, BaseTaskExecution) {
    bool executed = false;
    auto task = [&]() { executed = true; };

    context_.PostTask(std::make_unique<std::function<void()>>(task));

    context_.Start();

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    EXPECT_TRUE(executed);

    context_.Stop();
}

TEST_P(ThreadPoolContextTest, StartStopTest) {
    ASSERT_TRUE(context_.Start());
    ASSERT_FALSE(context_.Start());
    ASSERT_TRUE(context_.Stop());
    ASSERT_TRUE(context_.Start());
}

TEST_P(ThreadPoolContextTest, RunningFlagTest) {
    ASSERT_FALSE(context_.IsRunning());
    context_.Start();
    ASSERT_TRUE(context_.IsRunning());
    context_.Stop();
    ASSERT_FALSE(context_.IsRunning());
}

TEST_P(ThreadPoolContextTest, LoadedExecution) {
    std::atomic<std::uint32_t> executions = 0;

    constexpr std::uint32_t kNumberOfTasks = 1024;

    for (std::uint32_t i = 0; i < kNumberOfTasks; ++i) {
        auto task = [&]() { ++executions; };
        context_.PostTask(std::make_unique<std::function<void()>>(task));
    }

    context_.Start();

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    EXPECT_EQ(executions, kNumberOfTasks);
}

TEST_P(ThreadPoolContextTest, DifferentThreads) {
    const auto kNumberOfTasks = 4 * GetParam();

    std::set<std::thread::id> thread_ids;
    std::mutex thread_id_mutex;

    for (std::uint32_t i = 0; i < kNumberOfTasks; ++i) {
        auto task = [&]() {
            std::this_thread::sleep_for(std::chrono::milliseconds(5));

            std::lock_guard<std::mutex> lock(thread_id_mutex);
            thread_ids.insert(std::this_thread::get_id());
        };
        context_.PostTask(std::make_unique<std::function<void()>>(task));
    }

    context_.Start();

    std::this_thread::sleep_for(std::chrono::milliseconds(100 + (kNumberOfTasks * (5 + 5) / GetParam())));

    EXPECT_FALSE(thread_ids.empty());
    EXPECT_EQ(thread_ids.size(), GetParam());
}

INSTANTIATE_TEST_SUITE_P(NumberOfThreads, ThreadPoolContextTest, testing::Values(2, 8, 1024));