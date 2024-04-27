#ifndef TODOITEM_H
#define TODOITEM_H

#include <string>

class TodoItem {
public:
    int id;
    std::string title;
    std::string description;

    TodoItem(int id, std::string title, std::string description)
        : id(id), title(std::move(title)), description(std::move(description)) {}
};

#endif // TODOITEM_H
