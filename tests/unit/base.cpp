//
// Created by sergi on 9/5/2023.
//

#include <deque>
#include <future>
#include <thread>

#include <gtest/gtest.h>

#include <rapic/thread_pool_execution_context.hpp>

class ThreadPoolExecutionContextTest : public testing::TestWithParam<std::uint16_t> {
public:
    ThreadPoolExecutionContextTest()
        : context_(GetParam()) {}

protected:
    rapic::ThreadPoolExecutionContext context_;
};

TEST_P(ThreadPoolExecutionContextTest, TEST) {
    std::deque<std::packaged_task<int(void)>> deque;

    std::packaged_task<int(void)> t1([]() {return 1;});
    std::packaged_task<int(void)> t2([]() {return 2;});
    std::packaged_task<int(void)> t3([]() {return 3;});

    auto f1 = t1.get_future();
    auto f2 = t2.get_future();
    auto f3 = t3.get_future();

    deque.push_back(std::move(t1));
    deque.push_back(std::move(t2));
    deque.push_back(std::move(t3));

    auto t1c = std::move(deque.front());
    deque.pop_front();
    auto t2c = std::move(deque.front());
    deque.pop_front();
    auto t3c = std::move(deque.front());
    deque.pop_front();

    t1c();
    t2c();
    t3c();

    EXPECT_EQ(f1.get(), 1);
    EXPECT_EQ(f2.get(), 2);
    EXPECT_EQ(f3.get(), 3);
}

TEST_P(ThreadPoolExecutionContextTest, BaseTaskExecution) {
    bool executed = false;
    auto task = [&]() { executed = true; };

    context_.PostTask(std::make_unique<std::function<void()>>(task));

    context_.Start();

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    EXPECT_TRUE(executed);

    context_.Stop();
}

TEST_P(ThreadPoolExecutionContextTest, StartStopTest) {
    ASSERT_TRUE(context_.Start());
    ASSERT_FALSE(context_.Start());
    ASSERT_TRUE(context_.Stop());
    ASSERT_TRUE(context_.Start());
}

TEST_P(ThreadPoolExecutionContextTest, RunningFlagTest) {
    ASSERT_FALSE(context_.IsRunning());
    context_.Start();
    ASSERT_TRUE(context_.IsRunning());
    context_.Stop();
    ASSERT_FALSE(context_.IsRunning());
}

TEST_P(ThreadPoolExecutionContextTest, LoadedExecution) {
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

TEST_P(ThreadPoolExecutionContextTest, DifferentThreads) {
    const auto kNumberOfTasks = 4 * GetParam();

    std::set<std::thread::id> thread_ids;
    std::mutex thread_id_mutex;

    for (std::uint32_t i = 0; i < kNumberOfTasks; ++i) {
        auto task = [&]() {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));

            std::lock_guard<std::mutex> lock(thread_id_mutex);
            thread_ids.insert(std::this_thread::get_id());
        };
        context_.PostTask(std::make_unique<std::function<void()>>(task));
    }

    context_.Start();

    std::this_thread::sleep_for(std::chrono::milliseconds(100 + (kNumberOfTasks * (10 + 15) / GetParam())));

    EXPECT_FALSE(thread_ids.empty());
    EXPECT_EQ(thread_ids.size(), GetParam());
}

INSTANTIATE_TEST_SUITE_P(NumberOfThreads, ThreadPoolExecutionContextTest, testing::Values(2, 8, 1024));