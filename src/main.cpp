#include <bits/stdc++.h>
#include "Database.h"
#include "Shell.h"

int main() {
    try {
        Database db("todos.db");
        db.createTable();

        Shell shell(db);
        shell.run();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
