//
// Created by sergi on 8/26/2023.
//

#include <rapic/thread_pool_context.hpp>

namespace rapic {
struct Task {};
}  // namespace rapic

void rapic::ThreadPoolContext::PostTask(std::unique_ptr<Task> task) {}
