//
// Created by sergi on 8/26/2023.
//

#pragma once

#include "Request.hpp"
#include "Response.hpp"
#include "execution_context.hpp"

#include <functional>
#include <chrono>

// Callback for handling the Response
using RestClientCallback = std::function<void(const Response&)>;

// REST API client interface
class RestClient {
public:
    // Accept an ExecutionContext upon construction
    explicit RestClient(ExecutionContext& context, const std::string& base_url)
        : context_(context), base_url_(base_url) {}

    virtual ~RestClient() = default;

    // Send request
    virtual void SendRequest(const Request& request, RestClientCallback callback,
                                  std::chrono::milliseconds timeout) = 0;

    // Get the base URL
    const std::string& GetBaseUrl() const { return base_url_; }

    // Set the base URL
    void SetBaseUrl(const std::string& base_url) { base_url_ = base_url; }

    ExecutionContext& context() { return context_; }

private:
    ExecutionContext& context_;
    std::string base_url_;
};