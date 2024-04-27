#ifndef API_H
#define API_H

#include "Command.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string>

class API {
public:
    explicit API(Command& cmd, int port);
    ~API();
    void listenAndServe();

private:
    Command& command;
    int server_fd;
    int port;

    void setupServer();
    std::string handleRequest(const std::string& request);
    std::string parseRequestPath(const std::string& request);
    std::string parseRequestBody(const std::string& request);
};

#endif // API_H
