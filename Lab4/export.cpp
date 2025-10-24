// Подключаем необходимые библиотеки
#include <stdio.h>       // Стандартный ввод-вывод для работы с файлами
#include <string.h>      // Функции для работы со строками
#include <windows.h>     // Windows API для работы с кодировками
#include "phonebook.h"   // Структуры данных телефонной книги
#include "export.h"      // Заголовочный файл для экспорта

/**
 * Конвертирует строку из CP-1251 в UTF-8 для использования в vCard
 *
 * @param source - исходная строка в кодировке CP-1251
 * @return указатель на строку в UTF-8 (требует освобождения через delete[])
 *         или NULL в случае ошибки
 */
char* ConvertToUTF8ForVCard(const char* source) {
    if (source == NULL) return NULL;

    // Первый этап: определяем размер буфера для UTF-16
    int wlen = MultiByteToWideChar(1251, 0, source, -1, NULL, 0);
    if (wlen == 0) return NULL;

    // Выделяем память для UTF-16 строки
    wchar_t* wstr = new wchar_t[wlen];
    // Конвертируем из CP-1251 в UTF-16
    MultiByteToWideChar(1251, 0, source, -1, wstr, wlen);

    // Второй этап: определяем размер буфера для UTF-8
    int utf8len = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, NULL, 0, NULL, NULL);
    // Выделяем память для UTF-8 строки
    char* utf8str = new char[utf8len];
    // Конвертируем из UTF-16 в UTF-8
    WideCharToMultiByte(CP_UTF8, 0, wstr, -1, utf8str, utf8len, NULL, NULL);

    // Освобождаем промежуточный буфер UTF-16
    delete[] wstr;

    return utf8str;  // Возвращаем строку в UTF-8
}

/**
 * Экспортирует контакты телефонной книги в файл формата vCard (.vcf)
 *
 * vCard - стандартный формат для обмена контактной информацией
 * Поддерживается большинством почтовых клиентов и телефонных книг
 *
 * @param phonebook - массив контактов для экспорта
 * @param count - количество контактов в массиве
 */
void exportToVCard(Contact* phonebook, int count) {
    // Проверка входных параметров
    if (phonebook == NULL || count <= 0) {
        printf("Ошибка: нет данных для экспорта!\n");
        return;
    }

    FILE* file = NULL;
    // Безопасное открытие файла для записи (текстовый режим)
    errno_t err = fopen_s(&file, "phonebook.vcf", "w");

    if (err != 0 || file == NULL) {
        printf("Ошибка создания файла для экспорта!\n");
        return;
    }

    // Специально НЕ записываем BOM (Byte Order Mark)
    // Для vCard стандарт рекомендует использовать UTF-8 без BOM

    // Проходим по всем контактам и экспортируем их
    for (int i = 0; i < count; i++) {
        // Конвертируем имя в UTF-8 для корректного отображения русских символов
        char* utf8Name = ConvertToUTF8ForVCard(phonebook[i].name);

        // Записываем заголовок vCard для текущего контакта
        fprintf(file, "BEGIN:VCARD\n");
        fprintf(file, "VERSION:3.0\n");  // Указываем версию формата vCard

        // Записываем поле с именем (FN - Formatted Name)
        // Указываем кодировку UTF-8 для поддержки кириллицы
        if (utf8Name) {
            fprintf(file, "FN;CHARSET=UTF-8:%s\n", utf8Name);
        }
        else {
            // Резервный вариант на случай ошибки конвертации
            fprintf(file, "FN:Unknown\n");
        }

        // Записываем номер телефона
        // TYPE=CELL указывает, что это мобильный номер
        if (phonebook[i].number.hasPlus) {
            // Номер с международным префиксом +
            fprintf(file, "TEL;TYPE=CELL:+%llu\n", phonebook[i].number.number);
        }
        else {
            // Номер без международного префикса
            fprintf(file, "TEL;TYPE=CELL:%llu\n", phonebook[i].number.number);
        }

        // Завершаем запись контакта
        fprintf(file, "END:VCARD\n");

        // Добавляем пустую строку между контактами (кроме последнего)
        // для лучшей читаемости файла
        if (i < count - 1) {
            fprintf(file, "\n");
        }

        // Освобождаем память, выделенную для UTF-8 строки
        if (utf8Name) {
            delete[] utf8Name;
        }
    }

    // Закрываем файл
    fclose(file);

    // Выводим отчет об экспорте
    printf("Экспорт завершен! Файл: phonebook.vcf\n");
    printf("Количество экспортированных контактов: %d\n", count);
}