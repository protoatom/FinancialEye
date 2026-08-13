#pragma once

#include <sqlite3.h>

#include <string>

struct RowNamesDB {
    std::string name;
    std::string category_product;
    std::string action;
    double price = 0.0;
};

class WorkWithDatabase {
private:
    std::string address_database;
    sqlite3* db_ptr = nullptr;

public:
    explicit WorkWithDatabase(const std::string& constructor_address_database);
    ~WorkWithDatabase();

    WorkWithDatabase(const WorkWithDatabase&) = delete;
    WorkWithDatabase& operator=(const WorkWithDatabase&) = delete;

    void initializationDatabase();
    void addRecordToDatabase(const RowNamesDB& row);
    double getTotal(const std::string& action) const;
    bool deleteLastRecord();
};
