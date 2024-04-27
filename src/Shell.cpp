#include "Shell.h"
#include <bits/stdc++.h>

Shell::Shell(Database& db) : db(db) {
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

        int choice ;
        std::cin>>choice;


        if (choice > 0 && choice <= static_cast<int>(MENU_OPTION_COUNT)) {
            if (choice - 1 == EXIT) {
                break;  // Exit the loop
            } else {
                menuOptionsMethods[choice - 1].second();  // Execute the selected option's action
            }
        } else {
            std::cout << "Invalid choice. Please enter a number from 1 to " << MENU_OPTION_COUNT << ".\n";
        }
    }
}

void Shell::addTask() {
    TodoItem item = promptForTaskDetails();
    std::string sql = "INSERT INTO tasks (title, description) VALUES ('" + item.title + "', '" + item.description + "');";
    db.execute(sql);
    std::cout << "Task added.\n";
}

void Shell::viewTasks() const {
    std::string sql = "SELECT * FROM tasks;";
    std::vector<std::vector<std::string>> table;

    db.execute(sql, [&table](int argc, char** argv, char** azColName) {
        std::vector<std::string> row;
        for (int i = 0; i < argc; i++) {
            std::string value = argv[i] ? argv[i] : "NULL";
            row.push_back(value);
        }
        table.push_back(row);
    });

    for (const auto& row : table) {
        for (const auto& cell : row) {
            std::cout << std::setw(12) << std::left << cell << "| ";
        }
        std::cout << std::endl;
    }
}

void Shell::updateTask() {
    int id;
    std::cout << "Enter task ID to update: ";
    std::cin >> id;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    TodoItem item = promptForTaskDetails();
    std::string sql = "UPDATE tasks SET title = '" + item.title + "', description = '" + item.description + "' WHERE id = " + std::to_string(id) + ";";
    db.execute(sql);
    std::cout << "Task updated.\n";
}

void Shell::deleteTask() {
    int id;
    std::cout << "Enter task ID to delete: ";
    std::cin >> id;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::string sql = "DELETE FROM tasks WHERE id = " + std::to_string(id) + ";";
    db.execute(sql);
    std::cout << "Task deleted.\n";
}

void Shell::loadTasks() {
    std::string sql = "SELECT * FROM tasks;";
    db.execute(sql, [this](int argc, char** argv, char** azColName) {
        // Assuming that the callback gives us a row at a time with id, title, and description in that order.
        if (argc == 3) {
            int id = std::stoi(argv[0]);
            std::string title = argv[1] ? argv[1] : "";
            std::string description = argv[2] ? argv[2] : "";
            tasks.emplace_back(id, title, description);
        }
    });
}

TodoItem Shell::promptForTaskDetails() {
    std::string title, description;
    std::cout << "Enter task title: ";
    std::getline(std::cin, title);
    std::cout << "Enter task description: ";
    std::getline(std::cin, description);
    // ID will be set by the database upon insertion, so we use a placeholder value here.
    return {0, title, description};
}

void Shell::saveTask(const TodoItem& item) {
    std::string sql = "INSERT INTO tasks (title, description) VALUES ('" + item.title + "', '" + item.description + "');";
    db.execute(sql);
    std::cout << "Task saved.\n";
}