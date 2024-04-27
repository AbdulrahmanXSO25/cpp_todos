#ifndef DATABASE_H
#define DATABASE_H

#include <string>
#include <functional>
#include "lib/sqlite3.h"

class Database {
public:
    Database(const std::string& filename);
    ~Database();

    void execute(const std::string& sql);
    void execute(const std::string& sql, std::function<void(int, char**, char**)> callback);
    void createTable();

private:
    sqlite3* db;
};

#endif // DATABASE_H
