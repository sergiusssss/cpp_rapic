//
// Created by sergi on /26/2023.
//

#include <iostream>
#include <thread>
#include <utility>

#include <gtest/gtest.h>

#include <rapic/https_rest_client.hpp>
#include <rapic/tests/utils/https_rest_server.hpp>

namespace rapic::tests {

class HttpsRestClientFakeServerBaseTest {
public:
    explicit HttpsRestClientFakeServerBaseTest(std::shared_ptr<rapic::ExecutionContext> execution_context,
                                               const utils::HttpsRestServer::Configuration& configuration);

    void ValidateSequence(const Request& request, const Response& response);

protected:
    std::shared_ptr<rapic::ExecutionContext> execution_context_;

    rapic::HttpsRestClient client_;
    utils::HttpsRestServer server_;

    std::thread server_thread_;
};

}  // namespace rapic::tests