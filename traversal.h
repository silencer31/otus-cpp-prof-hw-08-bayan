#pragma once

#include "file_with_duplicates.h"

/**
* @brief јбстрактный класс, реализующий интерфейсы дл€ класса перебора файлов в директории.
*/
class Traversal
{
public:
    /**
    * @return был ли заевршен обход всех заданных директорий.
    */
    virtual bool traverse_finished() const = 0;

    /**
    * @return объект, представл€ющий файл, который может иметь дубликаты или сам быть дубликатом одного из коллекции.
    */
    virtual FileWithDuplicates get_next_file() = 0;
};