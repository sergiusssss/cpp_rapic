//
// Created by sergi on 8/26/2023.
//

#include <rapic/tests/utils/thread_pool_execution_context.hpp>

#include <boost/asio/post.hpp>

namespace rapic::tests::utils {

ThreadPoolExecutionContext::ThreadPoolExecutionContext(std::uint32_t thread_number)
    : thread_pool_(thread_number) {}

ThreadPoolExecutionContext::~ThreadPoolExecutionContext() { Stop(); }

void ThreadPoolExecutionContext::PostTask(std::packaged_task<void(void)> task) {
    auto f = task.get_future();
    boost::asio::post(thread_pool_, std::move(task));
}

bool ThreadPoolExecutionContext::Stop() {
    thread_pool_.stop();
    thread_pool_.join();

    return true;
}

std::string ThreadPoolExecutionContext::GetName() { return "ThreadPool"; }

}  // namespace rapic::tests::utils
