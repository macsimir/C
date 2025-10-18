#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include "input.h"

// Функция для извлечения только цифр из строки и преобразования в число
int extractDigitsAndConvert(const char* input, int* number) {
    char digits[256] = { 0 };
    int digitIndex = 0;
    int hasDigits = 0;
    int isNegative = 0;

    // Пропускаем начальные пробелы
    int i = 0;
    while (input[i] == ' ') i++;

    // Проверяем знак минуса
    if (input[i] == '-') {
        isNegative = 1;
        i++;
    }

    // Извлекаем только цифры
    for (; input[i] != '\0'; i++) {
        if (isdigit((unsigned char)input[i])) {
            digits[digitIndex++] = input[i];
            hasDigits = 1;
        }
    }

    if (!hasDigits) {
        return 0; // Нет цифр в строке
    }

    digits[digitIndex] = '\0';

    // Преобразуем строку цифр в число
    long long result = 0;
    for (int j = 0; digits[j] != '\0'; j++) {
        result = result * 10 + (digits[j] - '0');

        // Проверка переполнения
        if (result > INT_MAX) {
            if (isNegative && result - 1 > INT_MAX) {
                return 0; // Переполнение
            }
            if (!isNegative && result > INT_MAX) {
                return 0; // Переполнение
            }
        }
    }

    if (isNegative) {
        result = -result;
    }

    *number = (int)result;
    return 1;
}

// Функция для безопасного ввода с обработкой смешанного ввода
int safeInputWithDigits(int* number) {
    char input[256];

    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Ошибка ввода!\n");
        return 0;
    }

    // Удаляем символ новой строки
    input[strcspn(input, "\n")] = '\0';

    // Если введен 0, сразу возвращаем 0
    if (strcmp(input, "0") == 0) {
        *number = 0;
        return 1;
    }

    // Извлекаем цифры и преобразуем в число
    if (!extractDigitsAndConvert(input, number)) {
        printf("Ошибка: не удалось извлечь число из ввода!\n");
        return 0;
    }

    return 1;
}