#ifndef SHELL_H
#define SHELL_H

#include "Database.h"
#include "TodoItem.h"
#include <vector>

class Shell {
public:
    explicit Shell(Database& db);
    void run();

private:
    enum MenuOption {
        ADD_TASK,
        SHOW_TASKS,
        UPDATE_TASK,
        DELETE_TASK,
        EXIT,
        MENU_OPTION_COUNT
    };

    Database& db;
    std::vector<TodoItem> tasks;
    std::vector<std::pair<MenuOption, std::string>> menuOptionsNames;
    std::vector<std::pair<MenuOption, std::function<void()>>> menuOptionsMethods;

    void initializeMenu();
    void initializeMenuOptionsNames();
    void initializeMenuOptionsMethods();
    void addTask();
    void viewTasks() const;
    void updateTask();
    void deleteTask();
    void loadTasks();
    void saveTask(const TodoItem& item);
    TodoItem promptForTaskDetails();
};

#endif // SHELL_H