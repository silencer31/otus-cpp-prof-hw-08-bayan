#pragma once

#include "file_duplicate.h"

/**
* @brief ����������� �����, ����������� ���������� ��� ������ �������� ������ � ����������.
*/
class Traversal
{
public:
    /**
    * ������, ��� �� ������ ����� ����������.
    * @return ��������� ��������.
    */
    virtual bool dir_traversed() const = 0;

    /**
    * �������� ��������� ����, ������� ���������.
    * @return ������, �������������� ����, ������� ���������.
    */
    virtual FileDuplictate get_next_file() = 0;
};