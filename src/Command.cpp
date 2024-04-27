#include "Command.h"
#include <bits/stdc++.h>

Command::Command(Database& db) : db(db) { }

bool Command::addTask(const TodoItem& item) {
    std::string sql = "INSERT INTO tasks (title, description) VALUES ('" + item.title + "', '" + item.description + "');";
    return db.execute(sql);
}

bool Command::updateTask(const TodoItem& item) {
    std::string sql = "UPDATE tasks SET title = '" + item.title + "', description = '" + item.description + "' WHERE id = " + std::to_string(item.id) + ";";
    return db.execute(sql);
}

bool Command::deleteTask(int taskId) {
    std::string sql = "DELETE FROM tasks WHERE id = " + std::to_string(taskId) + ";";
    return db.execute(sql);
}

std::vector<TodoItem> Command::getTasks() {
    std::vector<TodoItem> tasks;
    std::string sql = "SELECT * FROM tasks;";
    
    db.execute(sql, [&tasks] (int argc, char** argv, char** azColName) {
        if (argc == 3) {
            int id = std::stoi(argv[0]);
            std::string title = argv[1] ? argv[1] : "";
            std::string description = argv[2] ? argv[2] : "";
            tasks.emplace_back(id, title, description);
        }
    });

    return tasks;
}
