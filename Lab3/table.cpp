#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "table.h"

/**
 * ����������� ����� �� ������ ���� � ���� �������� ������
 * ������� ������:
 * - result: ����� ��� ���������� (�������� ��������)
 * - resultSize: ������ ������ ����������
 * - text: �������� ����� ��� ������������
 * - width: ������ ���� ��� ������������
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
 * ����������� ����� �� ������� ���� � ���� �������� ������
 * ������� ������:
 * - result: ����� ��� ���������� (�������� ��������)
 * - resultSize: ������ ������ ����������
 * - text: �������� ����� ��� ������������
 * - width: ������ ���� ��� ������������
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
 * �������� �������������� �������������� ����� �������
 * ������� ������:
 * - cols: ���������� ������� � �������
 * - colWidth: ������ ������ ������� � ��������
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
 * �������� ������ ������� � �������� �������������
 * ������� ������:
 * - cells: ������ ����� - ����� �������
 * - cols: ���������� ������� (�����) � ������
 * - colWidth: ������ ������ ������� � ��������
 * - align: ��� ������������ (0 - �����, ������ �������� - ������)
 */
void printRow(const char* cells[], int cols, int colWidth, int align) {
    printf("|");
    for (int i = 0; i < cols; i++) {
        char aligned[50];

        if (align == 0) { // ����� ������������
            alignLeft(aligned, sizeof(aligned), cells[i], colWidth);
        }
        else { // ������ ������������
            alignRight(aligned, sizeof(aligned), cells[i], colWidth);
        }

        printf("%s|", aligned);
    }
    printf("\n");
}

/**
 * ��������� ����������� ����� ����� � ������
 * ������� ������:
 * - num: ����� ��� ��������������
 * - buffer: ����� ��� ������ ���������� (�������� ��������)
 * - bufferSize: ������ ������
 */
void intToString(int num, char* buffer, int bufferSize) {
    // ��������� �������� ��������
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

    // ��������� ����� ����� � �������� �������
    while (num > 0 && index < 19) {
        temp[index++] = (num % 10) + '0';
        num /= 10;
    }

    // ��������� ���� ����� ��� ������������� �����
    if (isNegative && index < 19) {
        temp[index++] = '-';
    }

    // �������������� ����� � ���������� �������
    int j = 0;
    for (int i = index - 1; i >= 0 && j < bufferSize - 1; i--, j++) {
        buffer[j] = temp[i];
    }
    buffer[j] = '\0';
}