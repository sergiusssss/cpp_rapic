//
// Created by sergi on 8/26/2023.
//

#pragma once

#include <chrono>
#include <functional>
#include <utility>

#include <rapic/execution_context.hpp>
#include <rapic/request.hpp>
#include <rapic/response.hpp>

namespace rapic {

// Callback for handling the Response
using RestClientCallback = std::function<void(const Response&)>;

// REST API client interface
class RestClient {
public:
    virtual ~RestClient() = default;

    virtual void SendRequest(const Request& request, RestClientCallback callback, std::chrono::milliseconds timeout) = 0;

    [[nodiscard]] virtual const std::string& GetBaseUrl() const = 0;

    virtual void SetBaseUrl(const std::string& base_url) = 0;

    [[nodiscard]] virtual ExecutionContext& Context() = 0;
};

}  // namespace rapic