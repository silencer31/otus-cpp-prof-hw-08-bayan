#pragma once

#include "parameters.h"

/**
* @brief Класс реализует чтение параметров программы.
*/
class OptionsReader {
public:

    /**
    * @brief Прочитать аргументы, с которыми была запущена программа.
    * @param argc кол-во аргументов.
    * @param argv аргуметны.
    * @return Структура с прочитанными параметрами.
    */
	Parameters read_arguments(int argc, const char* argv[]);

private:
    /**
    * @brief Проверить, что значение входит в допустимый диаппазон.
    */
    template <typename T>
    static void check_value_is_correct(const T& value, const std::initializer_list<T>& possible_values, std::string option_name);

    /**
    * @brief Проверить, что значение не является отрицательным.
    */
    template <typename T>
    static void check_value_is_not_negative(const T& value, std::string option_name);
};