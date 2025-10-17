#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>
#include <limits.h>

// Функция для выравнивания текста по левому краю
void alignLeft(char* result, int resultSize, const char* text, int width) {
    int len = strlen(text);
    if (len > width) len = width;

    int i;
    for (i = 0; i < len && i < resultSize - 1; i++) {
        result[i] = text[i];
    }

    for (; i < width && i < resultSize - 1; i++) {
        result[i] = ' ';
    }
    result[i] = '\0';
}

// Функция для выравнивания текста по правому краю
void alignRight(char* result, int resultSize, const char* text, int width) {
    int len = strlen(text);
    if (len > width) len = width;

    int spaces = width - len;
    if (spaces < 0) spaces = 0;

    int i;
    for (i = 0; i < spaces && i < resultSize - 1; i++) {
        result[i] = ' ';
    }

    for (int j = 0; j < len && i < resultSize - 1; j++, i++) {
        result[i] = text[j];
    }
    result[i] = '\0';
}

// Функция для печати разделительной линии
void printLine(int cols, int colWidth) {
    for (int i = 0; i < cols; i++) {
        printf("+");
        for (int j = 0; j < colWidth; j++) {
            printf("-");
        }
    }
    printf("+\n");
}

// Функция для печати строки таблицы с выравниванием
void printRow(const char* cells[], int cols, int colWidth, int align) {
    printf("|");
    for (int i = 0; i < cols; i++) {
        char aligned[50];

        if (align == 0) { // левое выравнивание
            alignLeft(aligned, sizeof(aligned), cells[i], colWidth);
        }
        else { // правое выравнивание
            alignRight(aligned, sizeof(aligned), cells[i], colWidth);
        }

        printf("%s|", aligned);
    }
    printf("\n");
}

// Функция для перевода числа в двоичную систему с дополнением нулями до 32 бит
void decimalToBinary32(int num, char* binary, int binarySize) {
    if (binarySize < 33) return;

    unsigned int unsigned_num = (unsigned int)num;

    // Заполняем строку битами, начиная со старшего
    for (int i = 0; i < 32; i++) {
        unsigned int bit = (unsigned_num >> (31 - i)) & 1;
        binary[i] = bit ? '1' : '0';
    }
    binary[32] = '\0';
}

// Функция для перевода числа в шестнадцатеричную систему
void decimalToHexadecimal(int num, char* hex, int hexSize) {
    if (hexSize < 9) return;

    unsigned int unsigned_num = (unsigned int)num;

    hex[0] = '0';
    hex[1] = 'x';

    for (int i = 0; i < 8; i++) {
        unsigned int nibble = (unsigned_num >> (28 - i * 4)) & 0xF;

        if (nibble < 10) {
            hex[i + 2] = '0' + nibble;
        }
        else {
            hex[i + 2] = 'A' + (nibble - 10);
        }
    }
    hex[10] = '\0';
}

// Безопасная функция для преобразования числа в строку
void intToString(int num, char* buffer, int bufferSize) {
    if (num == 0) {
        if (bufferSize > 1) {
            buffer[0] = '0';
            buffer[1] = '\0';
        }
        return;
    }

    int isNegative = 0;
    if (num < 0) {
        isNegative = 1;
        num = -num;
    }

    char temp[20];
    int index = 0;

    while (num > 0 && index < 19) {
        temp[index++] = (num % 10) + '0';
        num /= 10;
    }

    if (isNegative && index < 19) {
        temp[index++] = '-';
    }

    int j = 0;
    for (int i = index - 1; i >= 0 && j < bufferSize - 1; i--, j++) {
        buffer[j] = temp[i];
    }
    buffer[j] = '\0';
}

// Функция для безопасного ввода числа с проверкой диапазона
int safeInput(int* number) {
    long long input;

    // Считываем как long long для проверки диапазона
    if (scanf_s("%lld", &input) != 1) {
        printf("Ошибка ввода! Пожалуйста, введите число.\n");
        while (getchar() != '\n'); // Очистка буфера ввода
        return 0;
    }

    // Проверка диапазона
    if (input < INT_MIN || input > INT_MAX) {
        printf("Ошибка: число должно быть в диапазоне от %d до %d!\n", INT_MIN, INT_MAX);
        while (getchar() != '\n'); // Очистка буфера ввода
        return 0; // Возвращаем 0 при ошибке
    }

    *number = (int)input;
    return 1;
}

int main() {
    setlocale(LC_ALL, "Russian");

    char** binaryHistory = NULL;
    char** hexHistory = NULL;
    int* decimalHistory = NULL;
    int capacity = 10;
    int count = 0;
    int number;

    binaryHistory = (char**)malloc(capacity * sizeof(char*));
    hexHistory = (char**)malloc(capacity * sizeof(char*));
    decimalHistory = (int*)malloc(capacity * sizeof(int));

    if (binaryHistory == NULL || hexHistory == NULL || decimalHistory == NULL) {
        printf("Ошибка выделения памяти!\n");
        return 1;
    }

    for (int i = 0; i < capacity; i++) {
        binaryHistory[i] = (char*)malloc(33 * sizeof(char));
        hexHistory[i] = (char*)malloc(11 * sizeof(char));

        if (binaryHistory[i] == NULL || hexHistory[i] == NULL) {
            printf("Ошибка выделения памяти для строки!\n");
            return 1;
        }
    }

    printf("=================================================\n");
    printf("    КОНВЕРТЕР СИСТЕМ СЧИСЛЕНИЯ (2, 10, 16)      \n");
    printf("        Введите числа (0 для вывода таблицы)    \n");
    printf("    Диапазон: от %d до %d\n", INT_MIN, INT_MAX);
    printf("=================================================\n");

    while (1) {
        printf("Введите число: ");

        if (!safeInput(&number)) {
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
                binaryHistory[i] = (char*)malloc(33 * sizeof(char));
                hexHistory[i] = (char*)malloc(11 * sizeof(char));
                if (binaryHistory[i] == NULL || hexHistory[i] == NULL) {
                    printf("Ошибка выделения памяти для новой строки!\n");
                    break;
                }
            }
        }

        decimalHistory[count] = number;
        decimalToBinary32(number, binaryHistory[count], 33);
        decimalToHexadecimal(number, hexHistory[count], 11);

        printf("Добавлено: %d -> %s -> %s\n", number, binaryHistory[count], hexHistory[count]);
        count++;
    }

    if (count > 0) {
        const int COLS = 4;
        const int COL_WIDTH = 25;

        printf("\n");
        printf("+=====================================================================+\n");
        printf("|                          ИСТОРИЯ ОПЕРАЦИЙ                          |\n");
        printf("+=====================================================================+\n");

        // Заголовки таблицы
        const char* headers[] = { "№ операции", "Десятичная", "Двоичная (32 бита)", "Шестнадцатеричная" };
        printLine(COLS, COL_WIDTH);
        printRow(headers, COLS, COL_WIDTH, 0);
        printLine(COLS, COL_WIDTH);

        // Данные операций
        for (int i = 0; i < count; i++) {
            // Подготавливаем данные для строки
            char operationNum[20];
            char decimalStr[20];
            char binaryStr[33];
            char hexStr[11];

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

    // Ожидание ввода перед закрытием
    printf("\nНажмите Enter для выхода");
    while (getchar() != '\n'); // Очистка буфера, если что-то осталось
    getchar(); // Ожидание нажатия Enter
    return 0
        ;
}