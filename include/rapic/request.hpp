//
// Created by sergi on 8/26/2023.
//

#pragma once

#include <map>
#include <string>

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
    /**
     * @brief This method takes in a variable and performs a specific operation on it.
     *
     * The method modifies the value of the variable based on a predefined algorithm.
     * It performs a series of calculations and updates the variable accordingly.
     *
     * @param variable The variable to be modified.
     *
     * @return None
     *
     * @note This method does not return any value. It directly modifies the input variable.
     * Therefore, the original value of the variable will be changed after calling this method.
     *
     * @see related_method()
     */
    HttpMethod method;
    /**
     * @variable url
     * A string variable representing a URL.
     *
     * This variable holds the value of a URL string. It is commonly used to store the address of a webpage or API endpoint.
     *
     * Example usage:
     * ```cpp
     * url = "https://www.example.com";
     * ```
     *
     * @note The URL stored in this variable should be well-formed and include the necessary protocols and domain names.
     *
     * @see https://en.wikipedia.org/wiki/Uniform_Resource_Locator
     */
    std::string url;
    /**
     * @file variable_headers.h
     * @brief Contains the definition of the variable headers used in the code.
     */
    std::map<std::string, std::string> headers;
    /**
     * @brief The body variable holds the content of a document or message body.
     *
     * The body is typically a string that contains the main text of the document or message.
     * It can include any combination of characters, numbers, symbols, and newlines.
     * The body variable is used in various applications, such as email clients, text editors, and chat applications.
     *
     * Usage:
     * - Assign a value to the body variable using a string:
     *     body = "Lorem ipsum dolor sit amet...";
     *
     * - Modify the value of the body variable:
     *     body += "\n\nThis is an additional paragraph.";
     *
     * - Retrieve the value of the body variable and use it in applications:
     *     string messageBody = body;
     *     print(messageBody);
     *
     * @note The body variable does not enforce any specific formatting or structure of the content.
     * You may need to apply additional processing or validation based on the application requirements.
     */
    std::string body;
};