//
// Created by sergi on 9/12/2023.
//

#pragma once

#include <rapic/https_rest_client.hpp>

namespace rapic {

class HttpsRestClient::Impl {
public:
    // Accept an ExecutionContext upon construction
    explicit Impl(std::string base_url, ExecutionContext& context);
    ~Impl();

    // Send request
    void SendRequest(const Request& request, Callback callback, std::chrono::milliseconds timeout);

    // Get the base URL
    [[nodiscard]] const std::string& GetBaseUrl() const { return base_url_; }

    // Set the base URL
    void SetBaseUrl(const std::string& base_url) { base_url_ = base_url; }

    [[nodiscard]] ExecutionContext& Context() { return context_; }

private:
    ExecutionContext& context_;
    std::string base_url_;
};

}  // namespace rapic