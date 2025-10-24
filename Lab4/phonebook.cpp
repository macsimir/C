#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "phonebook.h"
#include "table.h"

// Функция для обработки и валидации номера телефона
PhoneNumber processPhoneNumber(const char* phoneStr) {
    PhoneNumber result = { 0, 0, 0 };  // Инициализация структуры
    char cleanedNumber[20] = "";     // Буфер для очищенного номера
    int cleanIndex = 0;              // Индекс для очищенного номера

    // Очистка номера от лишних символов
    for (int i = 0; phoneStr[i] != '\0'; i++) {
        if (phoneStr[i] == '+') {
            // Обрабатываем плюс только в начале номера
            if (i == 0) {
                result.hasPlus = 1;  // Устанавливаем флаг плюса
            }
        }
        else if (isdigit(phoneStr[i])) {
            // Добавляем цифры в очищенный номер
            cleanedNumber[cleanIndex++] = phoneStr[i];
        }
    }
    cleanedNumber[cleanIndex] = '\0';  // Завершаем строку

    // Проверка длины номера (от 5 до 15 цифр)
    int length = strlen(cleanedNumber);
    if (length >= 5 && length <= 15) {
        // Преобразование строки в число
        unsigned long long phoneNum = 0;
        for (int i = 0; i < length; i++) {
            phoneNum = phoneNum * 10 + (cleanedNumber[i] - '0');  // Конвертируем цифру
        }

        result.number = phoneNum;  // Сохраняем номер
        result.isValid = 1;        // Отмечаем как валидный
    }

    return result;  // Возвращаем обработанный номер
}

// Функция для преобразования числа в двоичную строку
void numberToBinaryString(unsigned long long number, int bits, char* buffer, int bufferSize) {
    int index = 0;  // Индекс для записи в буфер

    // Записываем биты в строку
    for (int i = bits - 1; i >= 0 && index < bufferSize - 1; i--) {
        buffer[index++] = ((number >> i) & 1) ? '1' : '0';  // Записываем '1' или '0'

        // Добавляем пробел каждые 4 бита для читаемости
        if (i % 4 == 0 && i != 0 && index < bufferSize - 1) {
            buffer[index++] = ' ';  // Разделитель
        }
    }
    buffer[index] = '\0';  // Завершаем строку
}

// Функция для вывода телефонной книги с использованием ваших функций таблицы
void printPhonebookTable(Contact* phonebook, int count) {
    if (count == 0) {
        printf("Телефонная книга пуста!\n");
        return;
    }

    // Параметры таблицы
    const int COLS = 4;           // 4 колонки
    const int COL_WIDTH = 20;     // Ширина колонки
    const int ALIGN_TYPE = 1;     // 0 - левое, 1 - правое (для нечетных студбилетов)

    // Заголовки таблицы
    const char* headers[] = { "№", "Имя", "Телефон", "Двоичный код" };

    // Выводим верхнюю границу таблицы
    printLine(COLS, COL_WIDTH);

    // Выводим заголовок с выравниванием
    printRow(headers, COLS, COL_WIDTH, ALIGN_TYPE);

    // Выводим разделитель после заголовка
    printLine(COLS, COL_WIDTH);

    // Выводим данные контактов
    for (int i = 0; i < count; i++) {
        char indexStr[10];         // Для номера по порядку
        char phoneStr[20];         // Для номера телефона
        char binaryStr[100];       // Для двоичного кода

        // Преобразуем номер контакта в строку
        intToString(i + 1, indexStr, sizeof(indexStr));

        // Формируем строку с номером телефона
        if (phonebook[i].number.hasPlus) {
            sprintf_s(phoneStr, "+%llu", phonebook[i].number.number);  // Номер с плюсом
        }
        else {
            sprintf_s(phoneStr, "%llu", phonebook[i].number.number);   // Номер без плюса
        }

        // Преобразуем номер в двоичную строку
        numberToBinaryString(phonebook[i].number.number, 60, binaryStr, sizeof(binaryStr));

        // Подготавливаем массив ячеек для строки
        const char* cells[] = {
            indexStr,                   // № п/п
            phonebook[i].name,          // Имя
            phoneStr,                   // Телефон
            binaryStr                   // Двоичный код
        };

        // Выводим строку с данными
        printRow(cells, COLS, COL_WIDTH, ALIGN_TYPE);
    }

    // Выводим нижнюю границу таблицы
    printLine(COLS, COL_WIDTH);
}