#ifndef PHONEBOOK_H  // ������ �� ���������� ���������
#define PHONEBOOK_H

#include <stdbool.h>  // ��� ���� bool

// ��������� ��� ������ �������� � �������� ������
typedef struct {
    unsigned long long number : 60;  // 60 ��� ��� ������ (���������� ��� 18 ����)
    unsigned int hasPlus : 1;        // 1 ��� ��� �������� �����
    unsigned int isValid : 1;        // 1 ��� ��� ���������� ������
} PhoneNumber;

// ��������� ��� ��������
typedef struct {
    char name[50];       // ��� ��������
    PhoneNumber number;  // ����� ��������
} Contact;

// ���������� �������
PhoneNumber processPhoneNumber(const char* phoneStr);
void printPhonebookTable(Contact* phonebook, int count);
void numberToBinaryString(unsigned long long number, int bits, char* buffer, int bufferSize);

#endif