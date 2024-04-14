#pragma once

#include "parameters.h"

/**
* @brief ����� ��������� ������ ���������� ���������.
*/
class OptionsReader {
public:

    /**
    * @brief ��������� ���������, � �������� ���� �������� ���������.
    * @param argc ���-�� ����������.
    * @param argv ���������.
    * @return ��������� � ������������ �����������.
    */
	Parameters read_arguments(int argc, const char* argv[]);

private:
    /**
    * @brief ���������, ��� �������� ������ � ���������� ���������.
    */
    template <typename T>
    static void check_value_is_correct(const T& value, const std::initializer_list<T>& possible_values, std::string option_name);

    /**
    * @brief ���������, ��� �������� �� �������� �������������.
    */
    template <typename T>
    static void check_value_is_not_negative(const T& value, std::string option_name);
};