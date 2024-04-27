#include "Database.h"
#include <bits/stdc++.h>

Database::Database(const std::string& filename) {
    if (sqlite3_open(filename.c_str(), &db) != SQLITE_OK) {
        throw std::runtime_error("Failed to open database");
    }
}

Database::~Database() {
    sqlite3_close(db);
}

bool Database::execute(const std::string& sql) {
    char* errMsg = nullptr;
    if (sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK) {
        std::string error = "SQL error: " + std::string(errMsg);
        sqlite3_free(errMsg);
        throw std::runtime_error(error);
    }
    else {
        return true;
    }
}

void Database::execute(const std::string& sql, std::function<void(int, char**, char**)> callback) {
    char* errMsg = nullptr;
    auto wrapper = [](void* param, int argc, char** argv, char** azColName) -> int {
        auto& func = *static_cast<std::function<void(int, char**, char**)>*>(param);
        func(argc, argv, azColName);
        return 0;
    };

    if (sqlite3_exec(db, sql.c_str(), wrapper, &callback, &errMsg) != SQLITE_OK) {
        std::string error = "SQL error: " + std::string(errMsg);
        sqlite3_free(errMsg);
        throw std::runtime_error(error);
    }
}

void Database::createTable() {
    std::ostringstream sql;
    sql << "CREATE TABLE IF NOT EXISTS TASKS(ID INTEGER PRIMARY KEY AUTOINCREMENT, TITLE NVARCHAR(" << TITLE_MAX << ") NOT NULL, DESCRIPTION TEXT NOT NULL);";
    execute(sql.str());
}