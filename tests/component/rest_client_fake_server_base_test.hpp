//
// Created by sergi on /26/2023.
//

#pragma once

#include <iostream>
#include <thread>
#include <utility>

#include <gtest/gtest.h>

#include <rapic/https_rest_client.hpp>
#include <rapic/tests/utils/rest_server.hpp>

namespace rapic::tests {

class RestClientFakeServerBaseTest {
public:
    explicit RestClientFakeServerBaseTest(std::shared_ptr<rapic::ExecutionContext> execution_context, std::shared_ptr<RestClient> client,
                                          std::shared_ptr<utils::RestServer> server);

    void ValidateSequence(const Request& request, const Response& response);

protected:
    std::shared_ptr<rapic::ExecutionContext> execution_context_;

    std::shared_ptr<rapic::RestClient> client_;
    std::shared_ptr<utils::RestServer> server_;

    std::thread server_thread_;
};

}  // namespace rapic::tests