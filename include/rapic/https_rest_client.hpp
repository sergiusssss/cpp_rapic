//
// Created by sergi on 8/26/2023.
//

#pragma once

#include <rapic/rest_client.hpp>

namespace rapic {

class HttpsRestClient : public RestClient {
public:
    // Accept an ExecutionContext upon construction
    explicit HttpsRestClient(std::string base_url, ExecutionContext& context);
    ~HttpsRestClient() override;

    // Send request
    void SendRequest(const Request& request, Callback callback, std::chrono::milliseconds timeout) override;

    // Get the base URL
    [[nodiscard]] const std::string& GetBaseUrl() const override;

    // Set the base URL
    void SetBaseUrl(const std::string& base_url) override;

    [[nodiscard]] ExecutionContext& Context() override;

private:
    class Impl;
    std::unique_ptr<Impl> impl_;
};

}  // namespace rapic