#ifndef TABLE_H
#define TABLE_H

// ������� ��� ������������ ������ �� ������ ����
void alignLeft(char* result, int resultSize, const char* text, int width);

// ������� ��� ������������ ������ �� ������� ����
void alignRight(char* result, int resultSize, const char* text, int width);

// ������� ��� ������ �������������� �����
void printLine(int cols, int colWidth);

// ������� ��� ������ ������ ������� � �������������
void printRow(const char* cells[], int cols, int colWidth, int align);

// ���������� ������� ��� �������������� ����� � ������
void intToString(int num, char* buffer, int bufferSize);

#endif