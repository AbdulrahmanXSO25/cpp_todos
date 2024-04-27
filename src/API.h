#ifndef API_H
#define API_H

#include "Command.h"
#include "HttpRequest.h"
#include "HttpResponse.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string>
#include <iostream>
#include <sstream>
#include <cstring>
#include <nlohmann/json.hpp>


struct RouteKey {
    HttpMethod method;
    std::string path;

    bool operator==(const RouteKey& other) const {
        return method == other.method && path == other.path;
    }
};

namespace std {
    template<>
    struct hash<RouteKey> {
        size_t operator()(const RouteKey& key) const {
            size_t h1 = hash<int>()(static_cast<int>(key.method));
            size_t h2 = hash<string>()(key.path);
            return h1 ^ (h2 << 1);
        }
    };
}

using HandlerFunction = std::function<HttpResponse(const HttpRequest&)>;

class API {
public:
    explicit API(Command& cmd, int port);
    ~API();
    void listenAndServe();

private:
    Command& command;
    int server_fd;
    int port;
    std::unordered_map<RouteKey, HandlerFunction> routes;

    std::string handleRequest(const std::string& request);
    void registerRoutes();
    void setupServer();
    HttpResponse handleGetTasks(const HttpRequest& request);
    HttpResponse handleAddTask(const HttpRequest& request);
    HttpResponse handleUpdateTask(const HttpRequest& request);
    HttpResponse handleDeleteTask(const HttpRequest& request);
};

#endif // API_H
