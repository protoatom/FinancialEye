#include "database.hpp"

#include <stdexcept>

WorkWithDatabase::WorkWithDatabase(const std::string& constructor_address_database)
    : address_database(constructor_address_database)
{
    int result = sqlite3_open(address_database.c_str(), &db_ptr);

    if (result != SQLITE_OK) {
        std::string error_message = sqlite3_errmsg(db_ptr);
        sqlite3_close(db_ptr);
        db_ptr = nullptr;
        throw std::runtime_error(error_message);
    }
}

WorkWithDatabase::~WorkWithDatabase()
{
    if (db_ptr != nullptr) {
        sqlite3_close(db_ptr);
    }
}

void WorkWithDatabase::initializationDatabase()
{
    const char* sql = R"(
        CREATE TABLE IF NOT EXISTS financial_data (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            name TEXT NOT NULL,
            category_product TEXT NOT NULL,
            action TEXT NOT NULL,
            price REAL NOT NULL CHECK(price > 0)
        );
    )";

    char* error_message = nullptr;

    int result = sqlite3_exec(
        db_ptr,
        sql,
        nullptr,
        nullptr,
        &error_message
    );

    if (result != SQLITE_OK) {
        std::string message = error_message != nullptr
            ? error_message
            : sqlite3_errmsg(db_ptr);
        sqlite3_free(error_message);
        throw std::runtime_error(message);
    }
}

void WorkWithDatabase::addRecordToDatabase(const RowNamesDB& row)
{
    const char* sql =
        "INSERT INTO financial_data "
        "(name, category_product, action, price) "
        "VALUES (?, ?, ?, ?);";

    sqlite3_stmt* statement_ptr = nullptr;
    int result = sqlite3_prepare_v2(db_ptr, sql, -1, &statement_ptr, nullptr);

    if (result != SQLITE_OK) {
        throw std::runtime_error(sqlite3_errmsg(db_ptr));
    }

    result = sqlite3_bind_text(
        statement_ptr,
        1,
        row.name.c_str(),
        -1,
        SQLITE_TRANSIENT
    );
    if (result == SQLITE_OK) {
        result = sqlite3_bind_text(
        statement_ptr,
        2,
        row.category_product.c_str(),
        -1,
        SQLITE_TRANSIENT
        );
    }
    if (result == SQLITE_OK) {
        result = sqlite3_bind_text(
        statement_ptr,
        3,
        row.action.c_str(),
        -1,
        SQLITE_TRANSIENT
        );
    }
    if (result == SQLITE_OK) {
        result = sqlite3_bind_double(statement_ptr, 4, row.price);
    }

    if (result != SQLITE_OK) {
        std::string error_message = sqlite3_errmsg(db_ptr);
        sqlite3_finalize(statement_ptr);
        throw std::runtime_error(error_message);
    }

    result = sqlite3_step(statement_ptr);

    if (result != SQLITE_DONE) {
        std::string error_message = sqlite3_errmsg(db_ptr);
        sqlite3_finalize(statement_ptr);
        throw std::runtime_error(error_message);
    }

    sqlite3_finalize(statement_ptr);
}

double WorkWithDatabase::getTotal(const std::string& action) const
{
    const char* sql =
        "SELECT COALESCE(SUM(price), 0) "
        "FROM financial_data WHERE action = ?;";
    sqlite3_stmt* statement_ptr = nullptr;

    int result = sqlite3_prepare_v2(db_ptr, sql, -1, &statement_ptr, nullptr);
    if (result != SQLITE_OK) {
        throw std::runtime_error(sqlite3_errmsg(db_ptr));
    }

    result = sqlite3_bind_text(
        statement_ptr,
        1,
        action.c_str(),
        -1,
        SQLITE_TRANSIENT
    );
    if (result != SQLITE_OK) {
        std::string error_message = sqlite3_errmsg(db_ptr);
        sqlite3_finalize(statement_ptr);
        throw std::runtime_error(error_message);
    }

    result = sqlite3_step(statement_ptr);
    if (result != SQLITE_ROW) {
        std::string error_message = sqlite3_errmsg(db_ptr);
        sqlite3_finalize(statement_ptr);
        throw std::runtime_error(error_message);
    }

    double total = sqlite3_column_double(statement_ptr, 0);
    sqlite3_finalize(statement_ptr);
    return total;
}

bool WorkWithDatabase::deleteLastRecord()
{
    const char* sql = R"(
        DELETE FROM financial_data
        WHERE id = (
            SELECT id FROM financial_data ORDER BY id DESC LIMIT 1
        );
    )";

    char* error_message = nullptr;
    int result = sqlite3_exec(db_ptr, sql, nullptr, nullptr, &error_message);

    if (result != SQLITE_OK) {
        std::string message = error_message != nullptr
            ? error_message
            : sqlite3_errmsg(db_ptr);
        sqlite3_free(error_message);
        throw std::runtime_error(message);
    }

    return sqlite3_changes(db_ptr) > 0;
}
