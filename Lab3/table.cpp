#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "table.h"

/**
 * Выравнивает текст по левому краю в поле заданной ширины
 * Входные данные:
 * - result: буфер для результата (выходной параметр)
 * - resultSize: размер буфера результата
 * - text: исходный текст для выравнивания
 * - width: ширина поля для выравнивания
 */
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

/**
 * Выравнивает текст по правому краю в поле заданной ширины
 * Входные данные:
 * - result: буфер для результата (выходной параметр)
 * - resultSize: размер буфера результата
 * - text: исходный текст для выравнивания
 * - width: ширина поля для выравнивания
 */
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

/**
 * Печатает горизонтальную разделительную линию таблицы
 * Входные данные:
 * - cols: количество колонок в таблице
 * - colWidth: ширина каждой колонки в символах
 */
void printLine(int cols, int colWidth) {
    for (int i = 0; i < cols; i++) {
        printf("+");
        for (int j = 0; j < colWidth; j++) {
            printf("-");
        }
    }
    printf("+\n");
}

/**
 * Печатает строку таблицы с заданным выравниванием
 * Входные данные:
 * - cells: массив строк - ячеек таблицы
 * - cols: количество колонок (ячеек) в строке
 * - colWidth: ширина каждой колонки в символах
 * - align: тип выравнивания (0 - левое, другое значение - правое)
 */
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

/**
 * Безопасно преобразует целое число в строку
 * Входные данные:
 * - num: число для преобразования
 * - buffer: буфер для записи результата (выходной параметр)
 * - bufferSize: размер буфера
 */
void intToString(int num, char* buffer, int bufferSize) {
    // Обработка нулевого значения
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

    // Извлекаем цифры числа в обратном порядке
    while (num > 0 && index < 19) {
        temp[index++] = (num % 10) + '0';
        num /= 10;
    }

    // Добавляем знак минус для отрицательных чисел
    if (isNegative && index < 19) {
        temp[index++] = '-';
    }

    // Переворачиваем цифры в правильном порядке
    int j = 0;
    for (int i = index - 1; i >= 0 && j < bufferSize - 1; i--, j++) {
        buffer[j] = temp[i];
    }
    buffer[j] = '\0';
}