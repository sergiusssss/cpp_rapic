//
// Created by sergi on 8/26/2023.
//

#ifndef RAPIC_REST_API_CLIENT_HPP
#define RAPIC_REST_API_CLIENT_HPP

#include <rapic/request.hpp>
#include <rapic/response.hpp>

namespace rapic {

class RestApiClient{

public:
    Response Execute(const Request& request);

};

}

#endif  // RAPIC_REST_API_CLIENT_HPP
