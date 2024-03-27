#pragma once

#include "file_duplicate.h"

/**
* @brief Абстрактный класс, реализующий интерфейсы для класса перебора файлов в директории.
*/
class Traversal
{
public:
    /**
    * Узнать, был ли сделан обход директории.
    * @return результат проверки.
    */
    virtual bool dir_traversed() const = 0;

    /**
    * Получить следующий файл, имеющий дубликаты.
    * @return объект, представляющий файл, имеющий дубликаты.
    */
    virtual FileDuplictate get_next_file() = 0;
};