#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "converter.h"

// ������� ��� �������� ����� � �������� ������� � ����������� �� �������
void decimalToBinaryWithTetrads(int num, char* binary, int binarySize) {
    if (binarySize < 40) return; // ����� ������ ����� ��� ��������

    unsigned int unsigned_num = (unsigned int)num;
    int index = 0;

    // ��������� ������ ������ � ����������� �� �������
    for (int i = 0; i < 32; i++) {
        unsigned int bit = (unsigned_num >> (31 - i)) & 1;
        binary[index++] = bit ? '1' : '0';

        // ��������� ������ ����� ������ ������� (4 ����)
        if ((i + 1) % 4 == 0 && i != 31) {
            binary[index++] = ' ';
        }
    }
    binary[index] = '\0';
}

// ������� ��� �������� ����� � ����������������� ������� ��� 0x
void decimalToHexadecimalNoPrefix(int num, char* hex, int hexSize) {
    if (hexSize < 9) return;

    unsigned int unsigned_num = (unsigned int)num;

    for (int i = 0; i < 8; i++) {
        unsigned int nibble = (unsigned_num >> (28 - i * 4)) & 0xF;

        if (nibble < 10) {
            hex[i] = '0' + nibble;
        }
        else {
            hex[i] = 'A' + (nibble - 10);
        }
    }
    hex[8] = '\0';
}