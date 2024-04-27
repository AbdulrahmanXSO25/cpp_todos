#ifndef HTTPRESPONSE_H
#define HTTPRESPONSE_H

#include <string>
#include <unordered_map>
#include <sstream>
#include "nlohmann/json.hpp"
#include "ContentType.h"

enum class HttpStatusCode {
    OK = 200,
    Created = 201,
    NoContent = 204,
    BadRequest = 400,
    Unauthorized = 401,
    PaymentRequired = 402,
    Forbidden = 403,
    NotFound = 404,
    MethodNotAllowed = 405,
    InternalServerError = 500,
    NotImplemented = 501,
    BadGateway = 502,
    ServiceUnavailable = 503,
    GatewayTimeout = 504,
};

class HttpResponse {
public:
    HttpStatusCode status;
    std::unordered_map<std::string, std::string> headers;
    nlohmann::json body; 

    explicit HttpResponse(HttpStatusCode status = HttpStatusCode::OK);

    void setBody(const nlohmann::json& content, ContentType type = ContentType::JSON);

    void addHeader(const std::string& key, const std::string& value);

    std::string generateResponse() const;

private:
    std::string statusCodeToString(HttpStatusCode code) const;
    std::string formatStatusLine() const;
    std::string assembleHeaders() const;
    void addDefaultHeaders();
};

#endif // HTTPRESPONSE_H