// 23.10.2025
// Version 1.0
// Shuravin Macsim

// Подключаем необходимые библиотеки
#include <stdio.h>      // Стандартный ввод-вывод
#include <string.h>     // Функции для работы со строками
#include <ctype.h>      // Функции для работы с символами
#include "phonebook.h"  // Заголовочный файл для работы с телефонной книгой
#include "table.h"      // Функции для работы с таблицами
#include <locale.h>     // Локализация (поддержка русского языка)
#include "export.h"     // Функции для экспорта данных
#include <windows.h>    // Функции Windows API

// Константы программы
#define MAX_CONTACTS 100  // Максимальное количество контактов в телефонной книге

/**
 * Главная функция программы - телефонная книга
 *
 * Программа позволяет:
 * - Вводить контакты (имя и телефон)
 * - Проверять корректность номеров телефонов
 * - Выводить все контакты в виде таблицы
 * - Экспортировать контакты в файл
 */
int main() {
    // Настройка локализации для поддержки русского языка
    SetConsoleOutputCP(1251);  // Установка кодовой страницы вывода
    SetConsoleCP(1251);        // Установка кодовой страницы ввода
    setlocale(LC_ALL, "Russian");  // Установка русской локали

    // Объявление переменных
    Contact phonebook[MAX_CONTACTS];  // Массив для хранения контактов
    int contactCount = 0;             // Текущее количество контактов
    char inputName[100];              // Буфер для ввода имени
    char inputPhone[20];              // Буфер для ввода номера телефона

    // Приветственное сообщение
    printf("=== ТЕЛЕФОННАЯ КНИГА ===\n");
    printf("Введите 0 в номере или имени для завершения ввода\n\n");

    // Главный цикл ввода контактов
    do {
        // Заголовок для текущего контакта
        printf("Контакт #%d:\n", contactCount + 1);

        // Ввод имени контакта
        printf("Vvedite imya: ");
        fgets(inputName, sizeof(inputName), stdin);  // Чтение строки с именем
        inputName[strcspn(inputName, "\n")] = '\0';  // Удаление символа новой строки

        // Проверка команды выхода (пользователь ввел "0")
        if (strcmp(inputName, "0") == 0) {
            break;  // Выход из цикла ввода
        }

        // Ввод номера телефона
        printf("Введите номер телефона: ");
        fgets(inputPhone, sizeof(inputPhone), stdin);  // Чтение строки с номером
        inputPhone[strcspn(inputPhone, "\n")] = '\0';  // Удаление символа новой строки

        // Проверка команды выхода (пользователь ввел "0")
        if (strcmp(inputPhone, "0") == 0) {
            break;  // Выход из цикла ввода
        }

        // Обработка и валидация номера телефона
        PhoneNumber processedNumber = processPhoneNumber(inputPhone);

        // Проверка корректности номера
        if (processedNumber.isValid) {
            // Сохранение корректного контакта в телефонную книгу
            strcpy_s(phonebook[contactCount].name, sizeof(phonebook[contactCount].name), inputName);
            phonebook[contactCount].number = processedNumber;
            contactCount++;  // Увеличение счетчика контактов

            printf("Контакт добавлен!!\n\n");
        }
        else {
            // Сообщение об ошибке при некорректном номере
            printf("Ошибка,неверный формат номера\n\n");
        }

    } while (contactCount < MAX_CONTACTS);  // Продолжаем пока есть место для контактов

    // Вывод всей телефонной книги в виде таблицы
    printf("\n=== ВАША ТЕЛЕФОННАЯ КНИГА ===\n");
    printPhonebookTable(phonebook, contactCount);

    // Экспорт контактов в файл (если есть что экспортировать)
    if (contactCount > 0) {
        printf("\n=== ЭКСПОРТ КОНТАКТОВ ===\n");
        exportToVCard(phonebook, contactCount);  // Экспорт в формате vCard
    }
    else {
        printf("\nНет контактов для экспорта.\n");
    }

    // Завершение программы
    printf("Для выхода нажмите энтер...");
    getchar();  // Ожидание нажатия клавиши

    return 0;  // Успешное завершение программы
}