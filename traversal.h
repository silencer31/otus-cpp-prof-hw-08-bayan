#pragma once

#include "file_with_duplicates.h"

/**
* @brief ����������� �����, ����������� ���������� ��� ������ �������� ������ � ����������.
*/
class Traversal
{
public:
    virtual ~Traversal() = default;
    /**
    * @return ��� �� �������� ����� ���� �������� ����������.
    */
    virtual bool traverse_finished() const = 0;

    /**
    * @return ������, �������������� ����, ������� ����� ����� ��������� ��� ��� ���� ���������� ������ �� ���������.
    */
    virtual FileWithDuplicates get_next_file() = 0;
};