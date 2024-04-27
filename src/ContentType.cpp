#include "ContentType.h"

const char* ContentTypeToString(ContentType type) {
    switch (type) {
        case ContentType::HTML: return "text/html";
        case ContentType::TEXT: return "text/plain";
        case ContentType::JSON: return "application/json";
        case ContentType::XML:  return "application/xml";
        case ContentType::JPEG: return "image/jpeg";
        case ContentType::PNG:  return "image/png";
        default:                return "application/octet-stream";
    }
}