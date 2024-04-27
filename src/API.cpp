#include "API.h"

using json = nlohmann::json;

API::API(Command& cmd, int port) : command(cmd), server_fd(-1), port(port) {
    setupServer();
    registerRoutes();
}

API::~API() {
    if (server_fd != -1) {
        close(server_fd);
    }
}

void API::setupServer() {
    struct sockaddr_in address;
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == 0) {
        std::cerr << "Socket creation failed" << std::endl;
        exit(EXIT_FAILURE);
    }

    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        std::cerr << "Set socket options failed" << std::endl;
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        std::cerr << "Bind failed" << std::endl;
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 10) < 0) {
        std::cerr << "Listen failed" << std::endl;
        exit(EXIT_FAILURE);
    }
}

void API::registerRoutes() {
    routes[{HttpMethod::GET, "/tasks"}] = [this](const HttpRequest& req) { return this->handleGetTasks(req); };
    routes[{HttpMethod::POST, "/tasks"}] = [this](const HttpRequest& req) { return this->handleAddTask(req); };
    routes[{HttpMethod::PUT, "/tasks"}] = [this](const HttpRequest& req) { return this->handleUpdateTask(req); };
    routes[{HttpMethod::DELETE, "/tasks"}] = [this](const HttpRequest& req) { return this->handleDeleteTask(req); };
}

void API::listenAndServe() {
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    int new_socket;
    char buffer[1024] = {0};

    std::cout << "LISTENING ON PORT " << port << "\n";
    while (true) {
        new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
        if (new_socket < 0) {
            std::cerr << "Failed to accept connection" << std::endl;
            continue;
        }
        
        read(new_socket, buffer, sizeof(buffer));
        std::string response = handleRequest(std::string(buffer));
        send(new_socket, response.c_str(), response.length(), 0);
        close(new_socket);
    }
}

std::string API::handleRequest(const std::string& rawRequest) {
    // Parse the incoming HTTP request
    HttpRequest httpRequest(rawRequest);

    // Display the request for debugging
    // httpRequest.Display();

    // Check if parsing was successful
    if (!httpRequest.errorMessage.empty()) {
        HttpResponse errorResponse(HttpStatusCode::BadRequest);
        errorResponse.setBody(nlohmann::json{{"error", httpRequest.errorMessage}}, ContentType::JSON);
        return errorResponse.generateResponse();
    }

    RouteKey routeKey{httpRequest.method, httpRequest.path};

    auto handlerIt = routes.find(routeKey);
    HttpResponse httpResponse;

    if (handlerIt != routes.end()) {
        httpResponse = handlerIt->second(httpRequest);
    } else {
        httpResponse = HttpResponse(HttpStatusCode::NotFound);
    }

    return httpResponse.generateResponse();
}

HttpResponse API::handleGetTasks(const HttpRequest& request) {
    auto tasks = command.getTasks();

    nlohmann::json jsonResponse;
    for (const auto& task : tasks) {
        jsonResponse.push_back(ObjectToJson(task));
    }

    HttpResponse response(HttpStatusCode::OK);
    response.setBody(jsonResponse, ContentType::JSON);
    return response;
}

template<typename T>
typename std::enable_if<std::is_class<T>::value, json>::type
ObjectToJson(const T& obj) {
    json jsonObj;
    // Example: Convert each public member variable to JSON
    for (const auto& [key, value] : obj) {
        jsonObj[key] = value;
    }
    return jsonObj;
}

HttpResponse API::handleAddTask(const HttpRequest& request) {
    if (request.body.is_null() || !request.body.is_object()) {
        nlohmann::json errorResponse = {{"message", "Invalid task data"}};
        HttpResponse response(HttpStatusCode::BadRequest);
        response.setBody(errorResponse, ContentType::JSON);
        return response;
    }

    try {
        TodoItem newTask(
            request.body.value("id", 0),
            request.body.value("title", ""), 
            request.body.value("description", "")
        );

        if (command.addTask(newTask)) {
            nlohmann::json successResponse = {{"message", "Task added successfully"}};
            HttpResponse response(HttpStatusCode::Created);
            response.setBody(successResponse, ContentType::JSON);
            return response;
        } else {
            nlohmann::json failureResponse = {{"message", "Failed to add task"}};
            HttpResponse response(HttpStatusCode::InternalServerError);
            response.setBody(failureResponse, ContentType::JSON);
            return response;
        }
    } catch (const std::exception& e) {
        nlohmann::json errorResponse = {{"message", "Error processing request: " + std::string(e.what())}};
        HttpResponse response(HttpStatusCode::BadRequest);
        response.setBody(errorResponse, ContentType::JSON);
        return response;
    }
}

HttpResponse API::handleUpdateTask(const HttpRequest& request) {
    if (request.body.is_null() || !request.body.is_object()) {
        nlohmann::json errorResponse = {{"message", "Invalid task data"}};
        HttpResponse response(HttpStatusCode::BadRequest);
        response.setBody(errorResponse, ContentType::JSON);
        return response;
    }

    try {

        int task_id = std::stoi(request.queryParameters.find("taskId")->second);

        TodoItem updatdTask(
            request.body.value("id", 0),
            request.body.value("title", ""), 
            request.body.value("description", "")
        );

        updatdTask.id = task_id;

        if (command.updateTask(updatdTask)) {
            nlohmann::json successResponse = {{"message", "Task updated successfully"}};
            HttpResponse response(HttpStatusCode::Created);
            response.setBody(successResponse, ContentType::JSON);
            return response;
        } else {
            nlohmann::json failureResponse = {{"message", "Failed to update task"}};
            HttpResponse response(HttpStatusCode::InternalServerError);
            response.setBody(failureResponse, ContentType::JSON);
            return response;
        }
    } catch (const std::exception& e) {
        nlohmann::json errorResponse = {{"message", "Error processing request: " + std::string(e.what())}};
        HttpResponse response(HttpStatusCode::BadRequest);
        response.setBody(errorResponse, ContentType::JSON);
        return response;
    }
}

HttpResponse API::handleDeleteTask(const HttpRequest& request) {
    if (request.body.is_null() || !request.body.is_object()) {
        nlohmann::json errorResponse = {{"message", "Invalid task data"}};
        HttpResponse response(HttpStatusCode::BadRequest);
        response.setBody(errorResponse, ContentType::JSON);
        return response;
    }

    try {
        int task_id = std::stoi(request.queryParameters.find("taskId")->second);

        if (command.deleteTask(task_id)) {
            nlohmann::json successResponse = {{"message", "Task deleted successfully"}};
            HttpResponse response(HttpStatusCode::Created);
            response.setBody(successResponse, ContentType::JSON);
            return response;
        } else {
            nlohmann::json failureResponse = {{"message", "Failed to delete task"}};
            HttpResponse response(HttpStatusCode::InternalServerError);
            response.setBody(failureResponse, ContentType::JSON);
            return response;
        }
    } catch (const std::exception& e) {
        nlohmann::json errorResponse = {{"message", "Error processing request: " + std::string(e.what())}};
        HttpResponse response(HttpStatusCode::BadRequest);
        response.setBody(errorResponse, ContentType::JSON);
        return response;
    }
}