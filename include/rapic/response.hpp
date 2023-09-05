//
// Created by sergi on 8/26/2023.
//

#pragma once

#include <map>
#include <string>

namespace rapic {

// Response structure
struct Response {
    int status_code;
    std::string status_message;
    std::map<std::string, std::string> headers;
    std::string body;
};

}  // namespace rapic