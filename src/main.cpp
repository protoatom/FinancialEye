#include "database.hpp"
#include "userlogic.hpp"

#include <exception>
#include <iomanip>
#include <iostream>

int main()
{
    try {
        WorkWithDatabase database("/home/sharky/FinancialEye/databases/database.db");
        WorkWithUser user_logic;

        database.initializationDatabase();

        bool is_running = true;
        while (is_running) {
            user_logic.menuOutput();
            std::cout << "Выберите действие: ";

            switch (user_logic.userChoice()) {
            case 1:
                database.addRecordToDatabase(user_logic.addUserCosts());
                std::cout << "Расход добавлен.\n";
                break;
            case 2:
                database.addRecordToDatabase(user_logic.addUserIncome());
                std::cout << "Доход добавлен.\n";
                break;
            case 3:
                std::cout << std::fixed << std::setprecision(2)
                          << "Общая сумма расходов: "
                          << database.getTotal("Расходы") << '\n';
                break;
            case 4:
                std::cout << std::fixed << std::setprecision(2)
                          << "Общая сумма доходов: "
                          << database.getTotal("Доходы") << '\n';
                break;
            case 5:
                std::cout << (database.deleteLastRecord()
                    ? "Последняя запись удалена.\n"
                    : "База данных пуста.\n");
                break;
            case 0:
                is_running = false;
                break;
            default:
                std::cout << "Неизвестный пункт меню.\n";
            }
        }
    } catch (const std::exception& error) {
        std::cerr << "Ошибка: " << error.what() << '\n';
        return 1;
    }

    return 0;
}
