#ifndef SHELL_H
#define SHELL_H

#include "Database.h"
#include "Command.h"
#include "TodoItem.h"
#include <vector>

class Shell {
public:
    explicit Shell(Command& cmd);
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

    Command& command;
    std::vector<std::pair<MenuOption, std::string>> menuOptionsNames;
    std::vector<std::pair<MenuOption, std::function<void()>>> menuOptionsMethods;

    void initializeMenu();
    void initializeMenuOptionsNames();
    void initializeMenuOptionsMethods();
    void viewTasks() const;
    void addTask();
    void updateTask();
    void deleteTask();
    TodoItem promptForTaskDetails();
};

#endif // SHELL_H