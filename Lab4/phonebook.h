#ifndef PHONEBOOK_H  // Защита от повторного включения
#define PHONEBOOK_H

#include <stdbool.h>  // Для типа bool

// Структура для номера телефона с битовыми полями
typedef struct {
    unsigned long long number : 60;  // 60 бит для номера (достаточно для 18 цифр)
    unsigned int hasPlus : 1;        // 1 бит для признака плюса
    unsigned int isValid : 1;        // 1 бит для валидности номера
} PhoneNumber;

// Структура для контакта
typedef struct {
    char name[50];       // Имя абонента
    PhoneNumber number;  // Номер телефона
} Contact;

// Объявления функций
PhoneNumber processPhoneNumber(const char* phoneStr);
void printPhonebookTable(Contact* phonebook, int count);
void numberToBinaryString(unsigned long long number, int bits, char* buffer, int bufferSize);

#endif