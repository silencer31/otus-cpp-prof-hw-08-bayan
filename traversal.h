#pragma once

#include "file_duplicate.h"

/**
* @brief ����������� ����� ��� ������ �������� ������ � ����������.
*/
class Traversal
{
public:
    virtual bool dir_traversed() const = 0;
    virtual FileDuplictate get_next_file() = 0;
};