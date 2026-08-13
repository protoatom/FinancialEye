#pragma once

#include "database.hpp"

class WorkWithUser {
public:
    void menuOutput() const;
    int userChoice() const;
    RowNamesDB addUserCosts() const;
    RowNamesDB addUserIncome() const;

private:
    RowNamesDB readRecord(const std::string& action) const;
};
