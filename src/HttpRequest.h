#ifndef HTTPREQUEST_H
#define HTTPREQUEST_H

#include <unordered_map>
#include <string>
#include <sstream>
#include <iostream>
#include "nlohmann/json.hpp"

enum class HttpMethod {
    GET,
    POST,
    PUT,
    DELETE,
    UNKNOWN
};

class HttpRequest {
public:
    HttpMethod method;
    std::string path;
    std::unordered_map<std::string, std::string> queryParameters;
    std::unordered_map<std::string, std::string> headers;
    nlohmann::json body;
    std::string errorMessage;

    explicit HttpRequest(const std::string& request);

    void Display() const;

private:
    void parse(const std::string& request);

    void parseQuery(const std::string& queryString);
    bool parseHeaders(std::istringstream& stream);
    bool parseBody(std::istringstream& stream);

    HttpMethod stringToHttpMethod(const std::string& method);

    std::string HttpMethodToString(HttpMethod method) const;
};

#endif // HTTPREQUEST_H
