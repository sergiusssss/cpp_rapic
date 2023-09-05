//
// Created by sergi on 8/26/2023.
//

#include <rapic/thread_pool_context.hpp>
#include "thread_pool_context_impl.hpp"

namespace rapic {

class Task {};

ThreadPoolContext::ThreadPoolContext(std::uint32_t thread_number)
    : impl_(std::make_unique<Impl>(thread_number)) {}

ThreadPoolContext::~ThreadPoolContext() = default;

void ThreadPoolContext::PostTask(std::unique_ptr<Task> task) { impl_->PostTask(std::move(task)); }

void ThreadPoolContext::Start() { impl_->Start(); }

void ThreadPoolContext::Stop() { impl_->Stop(); }

bool ThreadPoolContext::IsRunning() { return impl_->IsRunning(); }

}  // namespace rapic
