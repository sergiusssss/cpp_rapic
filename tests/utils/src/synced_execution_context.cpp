//
// Created by sergi on 9/12/2023.
//

#include <rapic/tests/utils/synced_execution_context.hpp>

namespace rapic::tests::utils {

void SyncedExecutionContext::PostTask(std::packaged_task<void(void)> task) {
    auto f = task.get_future();
    task();
    f.get();
}

std::string SyncedExecutionContext::GetName() { return "Synced"; }

}  // namespace rapic::tests::utils