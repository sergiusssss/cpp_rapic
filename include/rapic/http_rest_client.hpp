//
// Created by sergi on 8/26/2023.
//

#pragma once

#include <rapic/rest_client.hpp>

namespace rapic {

class HttpRestClient : public RestClient {
public:
    // Accept an ExecutionContext upon construction
    explicit HttpRestClient(ExecutionContext& context, std::string base_url)
        : context_(context)
        , base_url_(std::move(base_url)) {}

    ~HttpRestClient() override = default;

    // Send request
    void SendRequest(const Request& request, RestClientCallback callback, std::chrono::milliseconds timeout) override;

    // Get the base URL
    [[nodiscard]] const std::string& GetBaseUrl() const override { return base_url_; }

    // Set the base URL
    void SetBaseUrl(const std::string& base_url) override { base_url_ = base_url; }

    ExecutionContext& Context() override { return context_; }

private:
    ExecutionContext& context_;
    std::string base_url_;
};

}  // namespace rapic