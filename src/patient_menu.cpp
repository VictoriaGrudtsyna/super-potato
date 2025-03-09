#include "patient_menu.hpp"
#include "input_utils.hpp"
#include "common.hpp"
#include <iostream>
#include <string>

void edit_Profile(Database_Handler &db, int patient_id) {
    std::cout << "Для отмены обновления введите 'cancel' на любом этапе.\n";
    std::string new_last_name = get_Validated_Name("Введите новую фамилию:", true);
    if (new_last_name == CANCEL_COMMAND) {
        std::cout << "Обновление отменено.\n";
        return;
    }
    std::string new_first_name = get_Validated_Name("Введите новое имя:", true);
    if (new_first_name == CANCEL_COMMAND) {
        std::cout << "Обновление отменено.\n";
        return;
    }
    std::string new_patronymic = get_Input("Введите новое отчество (необязательно): ");
    if (new_patronymic == CANCEL_COMMAND) {
        std::cout << "Обновление отменено.\n";
        return;
    }
    if (!new_patronymic.empty()) {
        while (!isCyrillic(new_patronymic)) {
            std::cout << "Ошибка: допускается только кириллица!\n";
            new_patronymic = get_Input("Введите новое отчество (или 'cancel'): ");
            if (new_patronymic == CANCEL_COMMAND) {
                std::cout << "Обновление отменено.\n";
                return;
            }
        }
    }
    std::string new_phone = get_Validated_Phone();
    if (new_phone == CANCEL_COMMAND) {
        std::cout << "Обновление отменено.\n";
        return;
    }
    while (!db.update_User_Info(patient_id, new_last_name, new_first_name, new_patronymic, new_phone)) {
        std::cout << "Ошибка: выбранный телефон уже используется другим пользователем. Введите другой номер.\n";
        new_phone = get_Validated_Phone();
        if (new_phone == CANCEL_COMMAND) {
            std::cout << "Обновление отменено.\n";
            return;
        }
    }
    std::cout << "Личная информация успешно обновлена.\n";
}

void patient_Menu(Database_Handler &db, int patient_id) {
    int choice = 0;
    do {
        choice = get_Menu_Choice("\n=== Личный кабинет ===\n1. Просмотр записей\n2. Редактировать личную информацию\n3. Выход\nВыберите действие: ");
        switch (choice) {
            case 1: {
                std::cout << "\n=== Просмотр записей ===\n";
                std::string records = db.get_Patient_Records(patient_id);
                if (records.empty())
                    std::cout << "У вас пока нет записей\n";
                else
                    std::cout << records << std::endl;
            } break;
            case 2:
                edit_Profile(db, patient_id);
                break;
            case 3:
                std::cout << "Выход из личного кабинета.\n";
                break;
            default:
                std::cout << "Неверный выбор!\n";
                break;
        }
    } while (choice != 3);
}

void handle_Registration(Database_Handler &db) {
    std::cout << "Для отмены регистрации введите 'cancel' на любом этапе.\n";
    std::string last_name = get_Validated_Name("Фамилия:", true);
    if (last_name == CANCEL_COMMAND) {
        std::cout << "Регистрация отменена.\n";
        return;
    }
    std::string first_name = get_Validated_Name("Имя:", true);
    if (first_name == CANCEL_COMMAND) {
        std::cout << "Регистрация отменена.\n";
        return;
    }
    std::string patronymic = get_Input("Отчество (необязательно): ");
    if (patronymic == CANCEL_COMMAND) {
        std::cout << "Регистрация отменена.\n";
        return;
    }
    if (!patronymic.empty()) {
        while (!isCyrillic(patronymic)) {
            std::cout << "Ошибка: допускается только кириллица!\n";
            patronymic = get_Input("Отчество (или 'cancel'): ");
            if (patronymic == CANCEL_COMMAND) {
                std::cout << "Регистрация отменена.\n";
                return;
            }
        }
    }
    std::string phone = get_Validated_Phone();
    if (phone == CANCEL_COMMAND) {
        std::cout << "Регистрация отменена.\n";
        return;
    }
    while (db.user_Exists(phone)) {
        std::cout << "Ошибка: телефон уже зарегистрирован!\n";
        phone = get_Validated_Phone();
        if (phone == CANCEL_COMMAND) {
            std::cout << "Регистрация отменена.\n";
            return;
        }
    }
    std::string password = get_Validated_Password();
    if (password == CANCEL_COMMAND) {
        std::cout << "Регистрация отменена.\n";
        return;
    }
    if (db.register_User(last_name, first_name, patronymic, phone, password))
        std::cout << "Регистрация прошла успешно!\n";
    else
        std::cout << "Ошибка регистрации!\n";
}

std::string handle_Login(Database_Handler &db) {
    std::cout << "Для отмены входа введите 'cancel' на любом этапе.\n";
    std::string phone = get_Validated_Phone();
    if (phone == CANCEL_COMMAND) {
        std::cout << "Вход отменен.\n";
        return CANCEL_COMMAND;
    }
    std::string password = get_Validated_Input("Пароль:", false);
    if (password == CANCEL_COMMAND) {
        std::cout << "Вход отменен.\n";
        return CANCEL_COMMAND;
    }
    return db.login_User(phone, password);
}
