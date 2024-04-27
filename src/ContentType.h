#ifndef CONTENTTYPE_H
#define CONTENTTYPE_H

enum class ContentType {
    HTML,
    TEXT,
    JSON,
    XML,
    JPEG,
    PNG
};

const char* ContentTypeToString(ContentType type);

#endif // CONTENTTYPE_H