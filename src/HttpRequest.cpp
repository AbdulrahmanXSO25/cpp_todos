#include "HttpRequest.h"

HttpRequest::HttpRequest(const std::string& request) {
    body = nlohmann::json({});
    parse(request);
}

void HttpRequest::Display() const {
    std::cout << "HTTP Request:" << std::endl;
    std::cout << "Method: " << HttpMethodToString(method) << std::endl;
    std::cout << "Path: " << path << std::endl;

    if (!queryParameters.empty()) {
        std::cout << "Query Parameters:" << std::endl;
        for (const auto& [key, value] : queryParameters) {
            std::cout << "  " << key << ": " << value << std::endl;
        }
    }

    if (!headers.empty()) {
        std::cout << "Headers:" << std::endl;
        for (const auto& [key, value] : headers) {
            std::cout << "  " << key << ": " << value << std::endl;
        }
    }

    std::cout << "Body: ";
    if (body.empty()) {
        std::cout << "None" << std::endl;
    } else {
        std::cout << std::endl << body.dump(2) << std::endl; // Using dump(2) for pretty-printing JSON
    }

    if (!errorMessage.empty()) {
        std::cout << "Error: " << errorMessage << std::endl;
    }
}

void HttpRequest::parse(const std::string& request) {
    std::istringstream requestStream(request);
    std::string methodStr, url, protocol, line;

    if (!std::getline(requestStream, line)) {
        method = HttpMethod::UNKNOWN;
        return;
    }
    std::istringstream lineStream(line);
    if (!(lineStream >> methodStr >> url >> protocol)) {
        method = HttpMethod::UNKNOWN;
        return;
    }

    method = stringToHttpMethod(methodStr);

    auto queryPos = url.find('?');
    path = url.substr(0, queryPos);
    if (queryPos != std::string::npos) {
        parseQuery(url.substr(queryPos + 1));
    }

    if (!parseHeaders(requestStream)) {
        // Log and set error state
        std::cerr << "Failed to parse headers for path: " << path << std::endl;
        errorMessage = "Header parsing failed";
    }

    if(method != HttpMethod::GET && method != HttpMethod::DELETE && !parseBody(requestStream)) {
        // Log and set error state
        std::cerr << "Failed to parse body for path: " << path << std::endl;
        errorMessage = "Body parsing failed";
    }
}

void HttpRequest::parseQuery(const std::string& queryString) {
    std::istringstream queryStream(queryString);
    std::string pair;
    while (std::getline(queryStream, pair, '&')) {
        auto eqPos = pair.find('=');
        if (eqPos != std::string::npos) {
            std::string key = pair.substr(0, eqPos);
            std::string value = pair.substr(eqPos + 1);
            queryParameters[key] = value;
        } else {
            queryParameters[pair] = "";
        }
    }
}

bool HttpRequest::parseHeaders(std::istringstream& stream) {
    std::string line;
    while (std::getline(stream, line) && !line.empty() && line != "\r") {
        auto colonPos = line.find(':');
        if (colonPos == std::string::npos) {
            continue;
        }
        std::string headerName = line.substr(0, colonPos);
        std::string headerValue = line.substr(colonPos + 1);
        headers[headerName] = headerValue;
    }
    return true;
}

bool HttpRequest::parseBody(std::istringstream& stream) {
    std::string rawBody;
    std::string line;
    while (std::getline(stream, line)) {
        rawBody += line + "\n";
    }
    if (!rawBody.empty() && rawBody.back() == '\n') {
        rawBody.pop_back();
    }

    try {
        body = nlohmann::json::parse(rawBody);
    } catch (nlohmann::json::parse_error& e) {
        body = nlohmann::json({});
        return false;
    }
    return true;
}

HttpMethod HttpRequest::stringToHttpMethod(const std::string& method) {
    if (method == "GET") return HttpMethod::GET;
    if (method == "POST") return HttpMethod::POST;
    if (method == "PUT") return HttpMethod::PUT;
    if (method == "DELETE") return HttpMethod::DELETE;
    return HttpMethod::UNKNOWN;
}

std::string HttpRequest::HttpMethodToString(HttpMethod method) const {
    switch (method) {
        case HttpMethod::GET: return "GET";
        case HttpMethod::POST: return "POST";
        case HttpMethod::PUT: return "PUT";
        case HttpMethod::DELETE: return "DELETE";
        case HttpMethod::UNKNOWN: default: return "UNKNOWN";
    }
}