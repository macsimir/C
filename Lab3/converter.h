#ifndef CONVERTER_H
#define CONVERTER_H

// Функция для перевода числа в двоичную систему с разделением на тетрады
void decimalToBinaryWithTetrads(int num, char* binary, int binarySize);

// Функция для перевода числа в шестнадцатеричную систему без 0x
void decimalToHexadecimalNoPrefix(int num, char* hex, int hexSize);

#endif