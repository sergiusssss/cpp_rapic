//
// Created by sergi on 9/12/2023.
//

#pragma once

#include <functional>
#include <string>

#include <rapic/request.hpp>
#include <rapic/response.hpp>

namespace rapic::tests::utils {

class RestServer {
public:
    using Callback = std::function<Response(Request)>;

    virtual ~RestServer() = default;

    virtual void AcceptSingleConnection(const Callback& callback) = 0;
};

}  // namespace rapic::tests::utils
