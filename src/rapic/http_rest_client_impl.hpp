//
// Created by sergi on 9/12/2023.
//

#pragma once

#include <rapic/http_rest_client.hpp>

namespace rapic {

class HttpRestClient::Impl {
public:
    // Accept an ExecutionContext upon construction
    explicit Impl(std::string base_url, ExecutionContext& context);
    ~Impl();

    // Send request
    void SendRequest(const Request& request, Callback callback);

    // Get the base URL
    [[nodiscard]] const std::string& GetAddress() const { return base_url_; }

    [[nodiscard]] ExecutionContext& Context() { return context_; }

private:
    ExecutionContext& context_;
    std::string base_url_;
};

}  // namespace rapic