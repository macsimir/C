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
    const int MAX_NUMBERS = 100; // ����� �� ���������� �����
    int capacity = 10;
    int count = 0;
    int number;

    // ����������� ������� �������� ��� ������ �������
    binaryHistory = (char**)malloc(capacity * sizeof(char*));
    hexHistory = (char**)malloc(capacity * sizeof(char*));
    decimalHistory = (int*)malloc(capacity * sizeof(int));

    if (binaryHistory == NULL || hexHistory == NULL || decimalHistory == NULL) {
        printf("������ ��������� ������!\n");
        return 1;
    }

    for (int i = 0; i < capacity; i++) {
        binaryHistory[i] = (char*)malloc(40 * sizeof(char)); // ������ ����� ��� ��������
        hexHistory[i] = (char*)malloc(9 * sizeof(char)); // ������ 8 �������� + '\0'

        if (binaryHistory[i] == NULL || hexHistory[i] == NULL) {
            printf("������ ��������� ������ ��� ������!\n");
            return 1;
        }
    }

    printf("+----------------------------------------------------------------------+\n");
    printf("|                        ��������� ������ ���������                    |\n");
    printf("+----------------------------------------------------------------------+\n");
    printf("            ������� ����� (0 ��� ������ �������)                              \n");
    printf("            ��������: �� %d �� %d                                             \n", INT_MIN, INT_MAX);
    printf("            ������������ ���������� �����: %d                                 \n", MAX_NUMBERS);

    while (1) {
        // �������� ������
        if (count >= MAX_NUMBERS) {
            printf("\n��������� ����� � %d �����! ����� �������...\n", MAX_NUMBERS);
            break;
        }

        printf("������� �����: ");

        if (!safeInputWithDigits(&number)) {
            continue; // ���������� ���� ��� ������ �����
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
                printf("������ ������������� ������!\n");
                break;
            }

            binaryHistory = tempBinary;
            hexHistory = tempHex;
            decimalHistory = tempDecimal;

            for (int i = count; i < capacity; i++) {
                binaryHistory[i] = (char*)malloc(40 * sizeof(char));
                hexHistory[i] = (char*)malloc(9 * sizeof(char));
                if (binaryHistory[i] == NULL || hexHistory[i] == NULL) {
                    printf("������ ��������� ������ ��� ����� ������!\n");
                    break;
                }
            }
        }

        decimalHistory[count] = number;
        decimalToBinaryWithTetrads(number, binaryHistory[count], 40);
        decimalToHexadecimalNoPrefix(number, hexHistory[count], 9);

        printf("���������: %d -> %s -> %s\n", number, binaryHistory[count], hexHistory[count]);
        printf("�������� ����� ��� �����: %d\n", MAX_NUMBERS - count - 1);
        count++;
    }

    if (count > 0) {
        const int COLS = 4;
        const int COL_WIDTH = 35; // ����������� ������ ��� ��������� �������������

        printf("\n");
        printf("+----------------------------------------------------------------------+\n");
        printf("|                          ������� ��������                           |\n");
        printf("+----------------------------------------------------------------------+\n");

        // ��������� �������
        const char* headers[] = { "N ��������", "����������", "��������", "�����������������" };
        printLine(COLS, COL_WIDTH);
        printRow(headers, COLS, COL_WIDTH, 0);
        printLine(COLS, COL_WIDTH);

        // ������ ��������
        for (int i = 0; i < count; i++) {
            // �������������� ������ ��� ������
            char operationNum[20];
            char decimalStr[20];
            char binaryStr[40];
            char hexStr[9];

            // ����������� ����� �������� � ������� ���������� �������
            intToString(i + 1, operationNum, sizeof(operationNum));

            // ����������� ���������� ����� � ������� ���������� �������
            intToString(decimalHistory[i], decimalStr, sizeof(decimalStr));

            // �������� �������� �������������
            strcpy_s(binaryStr, sizeof(binaryStr), binaryHistory[i]);

            // �������� ����������������� �������������
            strcpy_s(hexStr, sizeof(hexStr), hexHistory[i]);

            // ������� ������ ����� ��� ������� ������
            const char* cells[] = { operationNum, decimalStr, binaryStr, hexStr };

            // �������� ������ � ������ �������������
            printRow(cells, COLS, COL_WIDTH, 1);
            printLine(COLS, COL_WIDTH);
        }

        printf("����� ��������: %d\n", count);
    }
    else {
        printf("\n������� �������� �����!\n");
    }

    // ����������� ������
    for (int i = 0; i < capacity; i++) {
        free(binaryHistory[i]);
        free(hexHistory[i]);
    }
    free(binaryHistory);
    free(hexHistory);
    free(decimalHistory);

    return 0;
}