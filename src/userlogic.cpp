#include "userlogic.hpp"

#include <iostream>
#include <limits>

void WorkWithUser::menuOutput() const
{
    std::cout << "\n1. Добавить расходы\n"
              << "2. Добавить доходы\n"
              << "3. Общая сумма расходов\n"
              << "4. Общая сумма доходов\n"
              << "5. Удалить последнюю запись\n"
              << "0. Выход\n";
}

int WorkWithUser::userChoice() const
{
    int number_choice = 0;

    while (!(std::cin >> number_choice)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Ошибка: введите номер пункта: ";
    }

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return number_choice;
}

RowNamesDB WorkWithUser::addUserCosts() const
{
    return readRecord("Расходы");
}

RowNamesDB WorkWithUser::addUserIncome() const
{
    return readRecord("Доходы");
}

RowNamesDB WorkWithUser::readRecord(const std::string& action) const
{
    RowNamesDB row;
    row.action = action;

    std::cout << "Введите наименование: ";
    std::getline(std::cin, row.name);

    std::cout << "Введите название категории: ";
    std::getline(std::cin, row.category_product);

    std::cout << "Введите сумму: ";
    while (!(std::cin >> row.price) || row.price <= 0.0) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Ошибка: введите число больше нуля: ";
    }

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return row;
}
