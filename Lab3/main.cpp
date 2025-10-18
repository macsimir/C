#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>
#include <limits.h>
#include "table.h"
#include "converter.h"
#include "input.h"

int main() {
    setlocale(LC_ALL, "Russian");

    char** binaryHistory = NULL;
    char** hexHistory = NULL;
    int* decimalHistory = NULL;
    const int MAX_NUMBERS = 100; // Лимит на количество чисел
    int capacity = 10;
    int count = 0;
    int number;

    // Увеличиваем размеры массивов для нового формата
    binaryHistory = (char**)malloc(capacity * sizeof(char*));
    hexHistory = (char**)malloc(capacity * sizeof(char*));
    decimalHistory = (int*)malloc(capacity * sizeof(int));

    if (binaryHistory == NULL || hexHistory == NULL || decimalHistory == NULL) {
        printf("Ошибка выделения памяти!\n");
        return 1;
    }

    for (int i = 0; i < capacity; i++) {
        binaryHistory[i] = (char*)malloc(40 * sizeof(char)); // Больше места для пробелов
        hexHistory[i] = (char*)malloc(9 * sizeof(char)); // Только 8 символов + '\0'

        if (binaryHistory[i] == NULL || hexHistory[i] == NULL) {
            printf("Ошибка выделения памяти для строки!\n");
            return 1;
        }
    }

    printf("+----------------------------------------------------------------------+\n");
    printf("|                        КОНВЕРТЕР СИСТЕМ СЧИСЛЕНИЯ                    |\n");
    printf("+----------------------------------------------------------------------+\n");
    printf("            Введите числа (0 для вывода таблицы)                              \n");
    printf("            Диапазон: от %d до %d                                             \n", INT_MIN, INT_MAX);
    printf("            Максимальное количество чисел: %d                                 \n", MAX_NUMBERS);

    while (1) {
        // Проверка лимита
        if (count >= MAX_NUMBERS) {
            printf("\nДостигнут лимит в %d чисел! Вывод таблицы...\n", MAX_NUMBERS);
            break;
        }

        printf("Введите число: ");

        if (!safeInputWithDigits(&number)) {
            continue; // Продолжаем цикл при ошибке ввода
        }

        if (number == 0) {
            break;
        }

        if (count >= capacity) {
            capacity *= 2;

            char** tempBinary = (char**)realloc(binaryHistory, capacity * sizeof(char*));
            char** tempHex = (char**)realloc(hexHistory, capacity * sizeof(char*));
            int* tempDecimal = (int*)realloc(decimalHistory, capacity * sizeof(int));

            if (tempBinary == NULL || tempHex == NULL || tempDecimal == NULL) {
                printf("Ошибка перевыделения памяти!\n");
                break;
            }

            binaryHistory = tempBinary;
            hexHistory = tempHex;
            decimalHistory = tempDecimal;

            for (int i = count; i < capacity; i++) {
                binaryHistory[i] = (char*)malloc(40 * sizeof(char));
                hexHistory[i] = (char*)malloc(9 * sizeof(char));
                if (binaryHistory[i] == NULL || hexHistory[i] == NULL) {
                    printf("Ошибка выделения памяти для новой строки!\n");
                    break;
                }
            }
        }

        decimalHistory[count] = number;
        decimalToBinaryWithTetrads(number, binaryHistory[count], 40);
        decimalToHexadecimalNoPrefix(number, hexHistory[count], 9);

        printf("Добавлено: %d -> %s -> %s\n", number, binaryHistory[count], hexHistory[count]);
        printf("Осталось чисел для ввода: %d\n", MAX_NUMBERS - count - 1);
        count++;
    }

    if (count > 0) {
        const int COLS = 4;
        const int COL_WIDTH = 35; // Увеличиваем ширину для двоичного представления

        printf("\n");
        printf("+----------------------------------------------------------------------+\n");
        printf("|                          ИСТОРИЯ ОПЕРАЦИЙ                           |\n");
        printf("+----------------------------------------------------------------------+\n");

        // Заголовки таблицы
        const char* headers[] = { "N операции", "Десятичная", "Двоичная", "Шестнадцатеричная" };
        printLine(COLS, COL_WIDTH);
        printRow(headers, COLS, COL_WIDTH, 0);
        printLine(COLS, COL_WIDTH);

        // Данные операций
        for (int i = 0; i < count; i++) {
            // Подготавливаем данные для строки
            char operationNum[20];
            char decimalStr[20];
            char binaryStr[40];
            char hexStr[9];

            // Форматируем номер операции с помощью безопасной функции
            intToString(i + 1, operationNum, sizeof(operationNum));

            // Форматируем десятичное число с помощью безопасной функции
            intToString(decimalHistory[i], decimalStr, sizeof(decimalStr));

            // Копируем двоичное представление
            strcpy_s(binaryStr, sizeof(binaryStr), binaryHistory[i]);

            // Копируем шестнадцатеричное представление
            strcpy_s(hexStr, sizeof(hexStr), hexHistory[i]);

            // Создаем массив ячеек для текущей строки
            const char* cells[] = { operationNum, decimalStr, binaryStr, hexStr };

            // Печатаем строку с разным выравниванием
            printRow(cells, COLS, COL_WIDTH, 1);
            printLine(COLS, COL_WIDTH);
        }

        printf("Всего операций: %d\n", count);
    }
    else {
        printf("\nИстория операций пуста!\n");
    }

    // Освобождаем память
    for (int i = 0; i < capacity; i++) {
        free(binaryHistory[i]);
        free(hexHistory[i]);
    }
    free(binaryHistory);
    free(hexHistory);
    free(decimalHistory);

    return 0;
}