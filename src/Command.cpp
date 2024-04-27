#include "Command.h"
#include <iostream>
#include <mutex>
#include <exception>

std::mutex dbMutex;  // Mutex for database access if needed

Command::Command(Database& db) : db(db) {}

bool Command::addTask(const TodoItem& item) {
    try {
        std::lock_guard<std::mutex> guard(dbMutex);
        std::string sql = "INSERT INTO tasks (title, description) VALUES (?, ?);";
        std::vector<std::string> params = {item.title, item.description};
        return db.execute(sql, params);
    } catch (const std::exception& e) {
        std::cerr << "Exception in addTask: " << e.what() << std::endl;
        return false;
    }
}

bool Command::updateTask(const TodoItem& item) {
    try {
        std::lock_guard<std::mutex> guard(dbMutex);
        std::string sql = "UPDATE tasks SET title = ?, description = ? WHERE id = ?;";
        std::vector<std::string> params = {item.title, item.description, std::to_string(item.id)};
        return db.execute(sql, params);
    } catch (const std::exception& e) {
        std::cerr << "Exception in updateTask: " << e.what() << std::endl;
        return false;
    }
}

bool Command::deleteTask(int taskId) {
    try {
        std::lock_guard<std::mutex> guard(dbMutex);
        std::string sql = "DELETE FROM tasks WHERE id = ?;";
        std::vector<std::string> params = {std::to_string(taskId)};
        return db.execute(sql, params);
    } catch (const std::exception& e) {
        std::cerr << "Exception in deleteTask: " << e.what() << std::endl;
        return false;
    }
}

std::vector<TodoItem> Command::getTasks() {
    try {
        std::lock_guard<std::mutex> guard(dbMutex);
        std::vector<TodoItem> tasks;
        std::string sql = "SELECT id, title, description FROM tasks;";
        db.execute(sql, [&tasks](int argc, char** argv, char** azColName) {
            if (argc == 3) {
                int id = std::stoi(argv[0]);
                std::string title = argv[1] ? argv[1] : "";
                std::string description = argv[2] ? argv[2] : "";
                tasks.emplace_back(id, title, description);
            }
        }, {});
        return tasks;
    } catch (const std::exception& e) {
        std::cerr << "Exception in getTasks: " << e.what() << std::endl;
        return {};
    }
}