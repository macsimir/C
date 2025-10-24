// ���������� ����������� ����������
#include <stdio.h>       // ����������� ����-����� ��� ������ � �������
#include <string.h>      // ������� ��� ������ �� ��������
#include <windows.h>     // Windows API ��� ������ � �����������
#include "phonebook.h"   // ��������� ������ ���������� �����
#include "export.h"      // ������������ ���� ��� ��������

/**
 * ������������ ������ �� CP-1251 � UTF-8 ��� ������������� � vCard
 *
 * @param source - �������� ������ � ��������� CP-1251
 * @return ��������� �� ������ � UTF-8 (������� ������������ ����� delete[])
 *         ��� NULL � ������ ������
 */
char* ConvertToUTF8ForVCard(const char* source) {
    if (source == NULL) return NULL;

    // ������ ����: ���������� ������ ������ ��� UTF-16
    int wlen = MultiByteToWideChar(1251, 0, source, -1, NULL, 0);
    if (wlen == 0) return NULL;

    // �������� ������ ��� UTF-16 ������
    wchar_t* wstr = new wchar_t[wlen];
    // ������������ �� CP-1251 � UTF-16
    MultiByteToWideChar(1251, 0, source, -1, wstr, wlen);

    // ������ ����: ���������� ������ ������ ��� UTF-8
    int utf8len = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, NULL, 0, NULL, NULL);
    // �������� ������ ��� UTF-8 ������
    char* utf8str = new char[utf8len];
    // ������������ �� UTF-16 � UTF-8
    WideCharToMultiByte(CP_UTF8, 0, wstr, -1, utf8str, utf8len, NULL, NULL);

    // ����������� ������������� ����� UTF-16
    delete[] wstr;

    return utf8str;  // ���������� ������ � UTF-8
}

/**
 * ������������ �������� ���������� ����� � ���� ������� vCard (.vcf)
 *
 * vCard - ����������� ������ ��� ������ ���������� �����������
 * �������������� ������������ �������� �������� � ���������� ����
 *
 * @param phonebook - ������ ��������� ��� ��������
 * @param count - ���������� ��������� � �������
 */
void exportToVCard(Contact* phonebook, int count) {
    // �������� ������� ����������
    if (phonebook == NULL || count <= 0) {
        printf("������: ��� ������ ��� ��������!\n");
        return;
    }

    FILE* file = NULL;
    // ���������� �������� ����� ��� ������ (��������� �����)
    errno_t err = fopen_s(&file, "phonebook.vcf", "w");

    if (err != 0 || file == NULL) {
        printf("������ �������� ����� ��� ��������!\n");
        return;
    }

    // ���������� �� ���������� BOM (Byte Order Mark)
    // ��� vCard �������� ����������� ������������ UTF-8 ��� BOM

    // �������� �� ���� ��������� � ������������ ��
    for (int i = 0; i < count; i++) {
        // ������������ ��� � UTF-8 ��� ����������� ����������� ������� ��������
        char* utf8Name = ConvertToUTF8ForVCard(phonebook[i].name);

        // ���������� ��������� vCard ��� �������� ��������
        fprintf(file, "BEGIN:VCARD\n");
        fprintf(file, "VERSION:3.0\n");  // ��������� ������ ������� vCard

        // ���������� ���� � ������ (FN - Formatted Name)
        // ��������� ��������� UTF-8 ��� ��������� ���������
        if (utf8Name) {
            fprintf(file, "FN;CHARSET=UTF-8:%s\n", utf8Name);
        }
        else {
            // ��������� ������� �� ������ ������ �����������
            fprintf(file, "FN:Unknown\n");
        }

        // ���������� ����� ��������
        // TYPE=CELL ���������, ��� ��� ��������� �����
        if (phonebook[i].number.hasPlus) {
            // ����� � ������������� ��������� +
            fprintf(file, "TEL;TYPE=CELL:+%llu\n", phonebook[i].number.number);
        }
        else {
            // ����� ��� �������������� ��������
            fprintf(file, "TEL;TYPE=CELL:%llu\n", phonebook[i].number.number);
        }

        // ��������� ������ ��������
        fprintf(file, "END:VCARD\n");

        // ��������� ������ ������ ����� ���������� (����� ����������)
        // ��� ������ ���������� �����
        if (i < count - 1) {
            fprintf(file, "\n");
        }

        // ����������� ������, ���������� ��� UTF-8 ������
        if (utf8Name) {
            delete[] utf8Name;
        }
    }

    // ��������� ����
    fclose(file);

    // ������� ����� �� ��������
    printf("������� ��������! ����: phonebook.vcf\n");
    printf("���������� ���������������� ���������: %d\n", count);
}