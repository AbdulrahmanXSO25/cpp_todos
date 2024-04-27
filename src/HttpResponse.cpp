#include "HttpResponse.h"

HttpResponse::HttpResponse(HttpStatusCode status)
    : status(status) {
}

void HttpResponse::setBody(const nlohmann::json& content, ContentType type) {
    body = content;
    addDefaultHeaders();
    addHeader("Content-Type", ContentTypeToString(type));
}

void HttpResponse::addHeader(const std::string& key, const std::string& value) {
    headers[key] = value;
}

std::string HttpResponse::generateResponse() const {
    std::ostringstream responseStream;
    responseStream << formatStatusLine();
    responseStream << assembleHeaders();
    std::string responseBody = body.dump();
    responseStream << "Content-Length: " << responseBody.length() << "\r\n\r\n";
    responseStream << responseBody;
    return responseStream.str();
}

std::string HttpResponse::formatStatusLine() const {
    return "HTTP/1.1 " + std::to_string(static_cast<int>(status)) + " " + statusCodeToString(status) + "\r\n";
}

std::string HttpResponse::assembleHeaders() const {
    std::ostringstream headersStream;
    for (const auto& header : headers) {
        headersStream << header.first << ": " << header.second << "\r\n";
    }
    return headersStream.str();
}

void HttpResponse::addDefaultHeaders() {
    headers["Server"] = "MyHttpServer/1.0";
}

std::string HttpResponse::statusCodeToString(HttpStatusCode code) const {
    switch (code) {
        case HttpStatusCode::OK: return "OK";
        case HttpStatusCode::Created: return "Created";
        case HttpStatusCode::NoContent: return "No Content";
        case HttpStatusCode::BadRequest: return "Bad Request";
        case HttpStatusCode::Unauthorized: return "Unauthorized";
        case HttpStatusCode::PaymentRequired: return "Payment Required";
        case HttpStatusCode::Forbidden: return "Forbidden";
        case HttpStatusCode::NotFound: return "Not Found";
        case HttpStatusCode::MethodNotAllowed: return "Method Not Allowed";
        case HttpStatusCode::InternalServerError: return "Internal Server Error";
        case HttpStatusCode::NotImplemented: return "Not Implemented";
        case HttpStatusCode::BadGateway: return "Bad Gateway";
        case HttpStatusCode::ServiceUnavailable: return "Service Unavailable";
        case HttpStatusCode::GatewayTimeout: return "Gateway Timeout";
        default: return "Unknown Status";
    }
}
