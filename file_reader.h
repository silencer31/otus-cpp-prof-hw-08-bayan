#pragma once

#include "type_aliases.h"

#include <fstream>
#include <cstddef>
#include <cstdint>

/**
* @brief Класс реализует поблочное чтение файла.
*/
class FileReader
{
public:
    FileReader(std::string path, size_t bs)
        : file_stream(path), block_size(bs)
    {}

    /**
    * @return размер одного читаемого блока.
    */
    size_t get_block_size() const {
        return block_size;
    }

    /**
    * @return достигнут ли конец файла.
    */
    bool file_end_reached() const {
        return file_stream.eof();
    }

    /**
    * Прочитать часть файла, ограниченную размером одного блока.
    * @param buffer параметр для приёма прочитанных данных.
    * @return количество прочитанных байт.
    */
    size_t read_block(ui_vector& buffer) {
        file_stream.read((char*)buffer.data(), block_size);

        return (size_t)file_stream.gcount();
    }

private:
    std::ifstream file_stream;
    size_t block_size;
};