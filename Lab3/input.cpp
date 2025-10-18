#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include "input.h"

// ������� ��� ���������� ������ ���� �� ������ � �������������� � �����
int extractDigitsAndConvert(const char* input, int* number) {
    char digits[256] = { 0 };
    int digitIndex = 0;
    int hasDigits = 0;
    int isNegative = 0;

    // ���������� ��������� �������
    int i = 0;
    while (input[i] == ' ') i++;

    // ��������� ���� ������
    if (input[i] == '-') {
        isNegative = 1;
        i++;
    }

    // ��������� ������ �����
    for (; input[i] != '\0'; i++) {
        if (isdigit((unsigned char)input[i])) {
            digits[digitIndex++] = input[i];
            hasDigits = 1;
        }
    }

    if (!hasDigits) {
        return 0; // ��� ���� � ������
    }

    digits[digitIndex] = '\0';

    // ����������� ������ ���� � �����
    long long result = 0;
    for (int j = 0; digits[j] != '\0'; j++) {
        result = result * 10 + (digits[j] - '0');

        // �������� ������������
        if (result > INT_MAX) {
            if (isNegative && result - 1 > INT_MAX) {
                return 0; // ������������
            }
            if (!isNegative && result > INT_MAX) {
                return 0; // ������������
            }
        }
    }

    if (isNegative) {
        result = -result;
    }

    *number = (int)result;
    return 1;
}

// ������� ��� ����������� ����� � ���������� ���������� �����
int safeInputWithDigits(int* number) {
    char input[256];

    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("������ �����!\n");
        return 0;
    }

    // ������� ������ ����� ������
    input[strcspn(input, "\n")] = '\0';

    // ���� ������ 0, ����� ���������� 0
    if (strcmp(input, "0") == 0) {
        *number = 0;
        return 1;
    }

    // ��������� ����� � ����������� � �����
    if (!extractDigitsAndConvert(input, number)) {
        printf("������: �� ������� ������� ����� �� �����!\n");
        return 0;
    }

    return 1;
}