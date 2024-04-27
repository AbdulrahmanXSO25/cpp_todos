#ifndef COMMAND_H
#define COMMAND_H

#include "Database.h"
#include "TodoItem.h"
#include <vector>

class Command {
public:
    explicit Command(Database& db);
    std::vector<TodoItem> getTasks();
    bool addTask(const TodoItem& item);
    bool updateTask(const TodoItem& item);
    bool deleteTask(int taskId);

private:
    Database& db;
};

#endif // COMMAND_H