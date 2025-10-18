#ifndef TABLE_H
#define TABLE_H

// Функция для выравнивания текста по левому краю
void alignLeft(char* result, int resultSize, const char* text, int width);

// Функция для выравнивания текста по правому краю
void alignRight(char* result, int resultSize, const char* text, int width);

// Функция для печати разделительной линии
void printLine(int cols, int colWidth);

// Функция для печати строки таблицы с выравниванием
void printRow(const char* cells[], int cols, int colWidth, int align);

// Безопасная функция для преобразования числа в строку
void intToString(int num, char* buffer, int bufferSize);

#endif