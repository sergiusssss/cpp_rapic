//
// Created by sergi on 9/12/2023.
//

#pragma once

#include <rapic/http_rest_client.hpp>

namespace rapic {

class HttpRestClient::Impl : public RestClient {
public:
    // Accept an ExecutionContext upon construction
    explicit Impl(std::string base_url, ExecutionContext& context);
    ~Impl() override;

    // Send request
    void SendRequest(const Request& request, Callback callback, std::chrono::milliseconds timeout) override;

    // Get the base URL
    [[nodiscard]] const std::string& GetBaseUrl() const override { return base_url_; }

    // Set the base URL
    void SetBaseUrl(const std::string& base_url) override { base_url_ = base_url; }

    [[nodiscard]] ExecutionContext& Context() override { return context_; }

private:
    ExecutionContext& context_;
    std::string base_url_;
};

}  // namespace rapic