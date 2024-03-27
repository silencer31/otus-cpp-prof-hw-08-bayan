#pragma once

#include "type_aliases.h"

#include <fstream>
#include <cstddef>
#include <cstdint>

/**
* @brief ����� ��������� ��������� ������ �����.
*/
class FileReader
{
public:
    FileReader(std::string path, size_t bs)
        : file_stream(path), block_size(bs)
    {}

    /**
    * @return ������ ������ ��������� �����.
    */
    size_t get_block_size() const {
        return block_size;
    }

    /**
    * @return ��������� �� ����� �����.
    */
    bool file_end_reached() const {
        return file_stream.eof();
    }

    /**
    * ��������� ����� �����, ������������ �������� ������ �����.
    * @param buffer �������� ��� ����� ����������� ������.
    * @return ���������� ����������� ����.
    */
    size_t read_block(ui_vector& buffer) {
        file_stream.read((char*)buffer.data(), block_size);

        return (size_t)file_stream.gcount();
    }

private:
    std::ifstream file_stream;
    size_t block_size;
};