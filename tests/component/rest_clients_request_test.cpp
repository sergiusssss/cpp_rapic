//
// Created by sergi on 9/26/2023.
//

#include <tuple>

#include <boost/range/algorithm.hpp>
#include <boost/range/algorithm_ext.hpp>

#include <gtest/gtest.h>

#include <rapic/request.hpp>
#include <rapic/response.hpp>

#include <rapic/tests/utils/synced_execution_context.hpp>
#include <rapic/tests/utils/thread_pool_execution_context.hpp>

#include "http_environment_holder.hpp"
#include "https_environment_holder.hpp"
#include "rest_client_fake_server_base_test.hpp"

struct TestData {
    rapic::Request request;
    rapic::Response response;
};

using EnvironmentFactory = std::function<std::shared_ptr<rapic::tests::EnvironmentHolder>(std::shared_ptr<rapic::ExecutionContext>)>;

using TestParameters = std::tuple<std::shared_ptr<rapic::tests::utils::TestExecutionContext>, EnvironmentFactory, TestData>;

const auto http_environment_factory =
  [](const std::shared_ptr<rapic::ExecutionContext>& execution_context) -> std::shared_ptr<rapic::tests::EnvironmentHolder> {
    const rapic::tests::utils::HttpRestServer::Configuration kHttpConfiguration{
      .address = "127.0.0.1",
      .service = 1234,
    };
    return std::make_shared<rapic::tests::HttpEnvironmentHolder>(execution_context, kHttpConfiguration);
};

const auto https_environment_factory =
  [](const std::shared_ptr<rapic::ExecutionContext>& execution_context) -> std::shared_ptr<rapic::tests::EnvironmentHolder> {
    const rapic::tests::utils::HttpsRestServer::Configuration kHttpsConfiguration{
      .address = "127.0.0.1",
      .service = 1234,

      .certificate_chain_file_path = "../resources/certificate.crt",
      .private_key_file_path = "../resources/privatekey.key",
    };
    return std::make_shared<rapic::tests::HttpsEnvironmentHolder>(execution_context, kHttpsConfiguration);
};

const std::vector<EnvironmentFactory> kEnvironmentFactories = {http_environment_factory, https_environment_factory};

const std::vector<TestData> kParams{
  {
    {rapic::HttpMethod::kGet,
     "/get",
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
  {
    {rapic::HttpMethod::kPost,
     "/post",
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

class RestClientsRequestTest : public testing::TestWithParam<TestParameters> {
public:
    RestClientsRequestTest()
        : execution_context_(std::get<0>(GetParam()))
        , environment_holder_(std::get<1>(GetParam())(execution_context_))
        , base_test(execution_context_, environment_holder_->GetClient(), environment_holder_->GetServer()) {}

    void ValidateSequence(const rapic::Request& request, const rapic::Response& response) { base_test.ValidateSequence(request, response); }

private:
    std::shared_ptr<rapic::ExecutionContext> execution_context_;
    std::shared_ptr<rapic::tests::EnvironmentHolder> environment_holder_;
    rapic::tests::RestClientFakeServerBaseTest base_test;
};

TEST_P(RestClientsRequestTest, SequenceValidation) {
    auto& data = std::get<2>(GetParam());

    ValidateSequence(data.request, data.response);
}

INSTANTIATE_TEST_SUITE_P(InstantiationName, RestClientsRequestTest,
                         testing::Combine(testing::Values(std::make_shared<rapic::tests::utils::SyncedExecutionContext>(),
                                                          std::make_shared<rapic::tests::utils::ThreadPoolExecutionContext>(4)),
                                          testing::ValuesIn(kEnvironmentFactories), testing::ValuesIn(kParams)),
                         [](const testing::TestParamInfo<RestClientsRequestTest::ParamType>& info) {
                             std::stringstream ss;
                             ss << std::get<0>(info.param)->GetName();
                             ss << std::get<1>(info.param)(std::get<0>(info.param))->GetName();
                             auto path = std::get<2>(info.param).request.path;
                             boost::remove_erase(path, '/');
                             path[0] = std::toupper(path[0]);
                             ss << path << info.index;
                             return ss.str();
                         });
