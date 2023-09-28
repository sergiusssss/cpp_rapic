//
// Created by sergi on 8/26/2023.
//

#pragma once

#include <map>
#include <string>

namespace rapic {

/**
 * @brief The HttpMethod enum class represents the possible HTTP methods.
 *
 * The HttpMethod class provides the following HTTP methods:
 * - GET: Represents the HTTP GET method for retrieving data.
 * - POST: Represents the HTTP POST method for sending data.
 * - PUT: Represents the HTTP PUT method for updating data.
 * - DELETE: Represents the HTTP DELETE method for deleting data.
 */
enum class HttpMethod { kGet, kPost, kPut, kDelete };

struct Request {
    HttpMethod method;
    std::string path;
    std::map<std::string, std::string> headers;
    std::string body;
};

}  // namespace rapic