#pragma once
#ifndef EXPORT_H
#define EXPORT_H

#include "phonebook.h"

// Функция экспорта телефонной книги в формат vCard
void exportToVCard(Contact* phonebook, int count);

#endif