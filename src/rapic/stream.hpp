//
// Created by sergi on 9/28/2023.
//

#pragma once

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>

namespace rapic {

class Stream {
public:
    virtual ~Stream() = default;

    virtual void Shutdown() = 0;

    virtual void Write(const boost::beast::http::request<boost::beast::http::string_body>& request) = 0;

    virtual boost::beast::http::response<boost::beast::http::string_body> Read() = 0;
};

}  // namespace rapic
