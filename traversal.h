#pragma once

#include "file_duplicate.h"

/**
* @brief Абстрактный класс для класса перебора файлов в директории.
*/
class Traversal
{
public:
    virtual bool dir_traversed() const = 0;
    virtual FileDuplictate get_next_file() = 0;
};