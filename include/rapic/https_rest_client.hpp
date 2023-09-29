//
// Created by sergi on 8/26/2023.
//

#pragma once

#include <rapic/rest_client.hpp>

namespace rapic {

class HttpsRestClient : public RestClient {
public:
    struct Configuration {
        Configuration(const std::string& address, const std::string& service);
        
        std::string address;
        std::string service;
    };

    explicit HttpsRestClient(const Configuration& configuration, ExecutionContext& context);
    ~HttpsRestClient() override;

    // Send request
    void SendRequest(const Request& request, const Callback& callback) override;

private:
    class Impl;
    std::unique_ptr<Impl> impl_;
};

}  // namespace rapic