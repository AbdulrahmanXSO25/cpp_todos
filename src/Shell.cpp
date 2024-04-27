#include "Shell.h"
#include <bits/stdc++.h>

Shell::Shell(Command &cmd) : command(cmd) {
    initializeMenu();
}

void Shell::initializeMenu() {
    initializeMenuOptionsNames();
    initializeMenuOptionsMethods();
}

void Shell::initializeMenuOptionsMethods() {
    menuOptionsMethods.push_back({ ADD_TASK, [this]() { addTask(); } });
    menuOptionsMethods.push_back({ UPDATE_TASK, [this]() { updateTask(); } });
    menuOptionsMethods.push_back({ DELETE_TASK, [this]() { deleteTask(); } });
    menuOptionsMethods.push_back({ SHOW_TASKS, [this]() { viewTasks(); } });
    menuOptionsMethods.push_back({ EXIT, [this]() { } });
}

void Shell::initializeMenuOptionsNames() {
    menuOptionsNames.push_back({ADD_TASK, "Add Task" });
    menuOptionsNames.push_back({UPDATE_TASK, "Update Task" });
    menuOptionsNames.push_back({DELETE_TASK, "Delete Task" });
    menuOptionsNames.push_back({SHOW_TASKS, "Show Tasks" });
    menuOptionsNames.push_back({EXIT, "Exit" });
}

void Shell::run() {
    std::string input;
    while (true) {
        std::cout << "Options:\n";
        for (size_t i = 0; i < MENU_OPTION_COUNT; ++i) {
            std::cout << i + 1 << ". " << menuOptionsNames[i].second << "\n";
        }
        std::cout << "Enter choice: ";
        std::getline(std::cin, input);

        int choice = std::stoi(input);

        if (choice > 0 && choice <= static_cast<int>(MENU_OPTION_COUNT)) {
            if (choice - 1 == EXIT) {
                std::cout << "You are welcome!\n";
                break;
            } else {
                menuOptionsMethods[choice - 1].second();
            }
        } else {
            std::cout << "Invalid choice. Please enter a number from 1 to " << MENU_OPTION_COUNT << ".\n";
        }
    }
}

void Shell::viewTasks() const {
    std::vector<TodoItem> tasks = command.getTasks();
    

    const int maxIntWidth = std::log10(std::numeric_limits<int>::max()) + 1;

    std::cout << std::left
            << std::setw(maxIntWidth) << "ID"
            << std::setw(TITLE_MAX) << "Title"
            << "Description" << std::endl;

    for (const auto& task : tasks) {
        std::cout << std::left
            << std::setw(maxIntWidth) << task.id
            << std::setw(TITLE_MAX) << task.title
            << task.description << std::endl;
    }
}

void Shell::addTask() {
    TodoItem item = promptForTaskDetails();
    command.addTask(item) ? std::cout << "Task added.\n" : std::cout << "Problem Adding.\n";
}

void Shell::updateTask() {
    int id;
    std::cout << "Enter task ID to update: ";
    std::cin >> id;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    TodoItem item = promptForTaskDetails();
    item.id = id;

    command.updateTask(item) ? std::cout << "Task " << item.id << " updated.\n" : std::cout << "Problem Adding.\n";
}

void Shell::deleteTask() {
    int id;
    std::cout << "Enter task ID to delete: ";
    std::cin >> id;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    command.deleteTask(id) ? std::cout << "Task " << id << " deleted.\n" : std::cout << "Problem deleting.\n";
}

TodoItem Shell::promptForTaskDetails() {
    std::string title, description;
    std::cout << "Enter task title: ";
    std::getline(std::cin, title);
    std::cout << "Enter task description: ";
    std::getline(std::cin, description);
    return {0, title, description};
}