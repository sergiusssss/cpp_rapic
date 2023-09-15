//
// Created by sergi on 9/13/2023.
//

#pragma once

#include <future>

#include <rapic/request.hpp>
#include <rapic/rest_client.hpp>

namespace rapic {

std::packaged_task<void(void)>
CreateHttpsRequestTask(const std::string& base_url, const Request& request, const RestClient::Callback& callback, std::chrono::milliseconds timeout);

}  // namespace rapic
