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

// REST API client interface
class RestClient {
public:
    // Callback for handling the Response
    using Callback = std::function<void(const Response&)>;

    virtual ~RestClient() = default;

    virtual void SendRequest(const Request& request, const Callback& callback) = 0;
};

}  // namespace rapic