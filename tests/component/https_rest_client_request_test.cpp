//
// Created by sergi on 9/26/2023.
//

#include <rapic/request.hpp>
#include <rapic/response.hpp>

#include <gtest/gtest.h>

#include <rapic/tests/utils/synced_execution_context.hpp>

#include "https_rest_client_fake_server_base_test.hpp"

struct TestParameters {
    rapic::Request request;
    rapic::Response response;
};

const rapic::tests::utils::HttpsRestServer::Configuration kConfiguration{
  .address = "127.0.0.1",
  .service = 1234,

  .certificate_chain_file_path = "../resources/certificate.crt",
  .private_key_file_path = "../resources/privatekey.key",
};

const std::vector<TestParameters> kParams{
  {
    {rapic::HttpMethod::kGet,
     "/api/test",
     {
       {"Host", "127.0.0.1"},
       {"Content-Length", "36"},
       {"User-Agent", "Boost.Beast/347"},
       {"TestReqHeaderKey", "TestReqHeaderValue"},
     },
     R"({"test_req_name" : "test_req_value"})"},
    {200,
     "OK",
     {
       {"Content-Length", "36"},
       {"Content-Type", "application/json"},
       {"Server", "Boost.Beast/347"},
       {"TestResHeaderKey", "TestResHeaderValue"},
     },
     R"({"test_res_name" : "test_res_value"})"},
  },
};

class HttpsRestClientRequestTest
    : public testing::TestWithParam<TestParameters>
    , public rapic::tests::HttpsRestClientFakeServerBaseTest {
public:
    HttpsRestClientRequestTest()
        : HttpsRestClientFakeServerBaseTest(std::make_shared<rapic::tests::utils::SyncedExecutionContext>(), kConfiguration) {}
};

TEST_P(HttpsRestClientRequestTest, SequenceValidation) {
    auto& params = GetParam();

    ValidateSequence(params.request, params.response);
}

INSTANTIATE_TEST_SUITE_P(InstantiationName, HttpsRestClientRequestTest, testing::ValuesIn(kParams));
