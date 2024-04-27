#include <iostream>
#include <thread>
#include "Database.h"
#include "Shell.h"
#include "API.h"

void runAPI(API& api) {
    api.listenAndServe();
}

void runShell(Shell& shell) {
    shell.run();
}

int main() {
    try {
        Database db("todos.db");
        db.createTable();

        Command cmd(db);

        API api(cmd, 5000);
        Shell shell(cmd);

        std::thread apiThread(runAPI, std::ref(api));
        std::thread shellThread(runShell, std::ref(shell));

        apiThread.join();
        shellThread.join();

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}