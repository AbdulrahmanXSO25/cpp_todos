#include "Database.h"
#include <sstream>
#include <stdexcept>

Database::Database(const std::string& filename) {
    if (sqlite3_open(filename.c_str(), &db) != SQLITE_OK) {
        throw std::runtime_error("Failed to open database: " + std::string(sqlite3_errmsg(db)));
    }
}

Database::~Database() {
    sqlite3_close(db);
}

bool Database::execute(const std::string& sql, const std::vector<std::string>& params) {
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        throw std::runtime_error("Failed to prepare statement: " + std::string(sqlite3_errmsg(db)));
    }

    // Binding parameters
    int index = 1;
    for (const auto& param : params) {
        if (sqlite3_bind_text(stmt, index++, param.c_str(), -1, SQLITE_STATIC) != SQLITE_OK) {
            sqlite3_finalize(stmt);
            throw std::runtime_error("Failed to bind parameter: " + std::string(sqlite3_errmsg(db)));
        }
    }

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        sqlite3_finalize(stmt);
        throw std::runtime_error("Execution failed: " + std::string(sqlite3_errmsg(db)));
    }

    sqlite3_finalize(stmt);
    return true;
}

void Database::execute(const std::string& sql, std::function<void(int, char**, char**)> callback, const std::vector<std::string>& params) {
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        throw std::runtime_error("Failed to prepare statement: " + std::string(sqlite3_errmsg(db)));
    }

    // Binding parameters
    int index = 1;
    for (const auto& param : params) {
        if (sqlite3_bind_text(stmt, index++, param.c_str(), -1, SQLITE_STATIC) != SQLITE_OK) {
            sqlite3_finalize(stmt);
            throw std::runtime_error("Failed to bind parameter: " + std::string(sqlite3_errmsg(db)));
        }
    }

    int colCount = sqlite3_column_count(stmt);
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        std::vector<char*> columnTexts(colCount);
        std::vector<char*> columnNames(colCount);

        for (int i = 0; i < colCount; ++i) {
            const char* text = reinterpret_cast<const char*>(sqlite3_column_text(stmt, i));
            const char* name = sqlite3_column_name(stmt, i);

            columnTexts[i] = const_cast<char*>(text);
            columnNames[i] = const_cast<char*>(name);
        }

        callback(colCount, columnTexts.data(), columnNames.data());
    }

    sqlite3_finalize(stmt);
}

void Database::createTable() {
    std::string sql = "CREATE TABLE IF NOT EXISTS tasks ("
                "ID INTEGER PRIMARY KEY AUTOINCREMENT, "
                "TITLE NVARCHAR(50) NOT NULL, "
                "DESCRIPTION TEXT NOT NULL);";
    execute(sql, {}); // No parameters needed for table creation
}
