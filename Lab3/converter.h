#ifndef CONVERTER_H
#define CONVERTER_H

// ������� ��� �������� ����� � �������� ������� � ����������� �� �������
void decimalToBinaryWithTetrads(int num, char* binary, int binarySize);

// ������� ��� �������� ����� � ����������������� ������� ��� 0x
void decimalToHexadecimalNoPrefix(int num, char* hex, int hexSize);

#endif