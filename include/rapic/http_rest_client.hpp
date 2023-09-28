//
// Created by sergi on 8/26/2023.
//

#pragma once

#include <rapic/rest_client.hpp>

namespace rapic {

class HttpRestClient : public RestClient {
public:
    struct Configuration {
        std::string address;
        std::string service;
    };

    // Accept an ExecutionContext upon construction
    explicit HttpRestClient(const Configuration& configuration, ExecutionContext& context);
    ~HttpRestClient() override;

    // Send request
    void SendRequest(const Request& request, Callback callback) override;

private:
    class Impl;
    std::unique_ptr<Impl> impl_;
};

}  // namespace rapic