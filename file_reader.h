#pragma once

#include <fstream>
#include <string>
#include <cstddef>
#include <cstdint>
#include <vector>

/**
* @brief ����� ��������� ��������� ������ �����.
*/
class FileReader
{
public:
    FileReader(std::string path, size_t bs)
        : file_stream(path), block_size(bs) {}

    /**
    * ������ ������ ������ �����.
    */
    size_t get_block_size() const {
        return block_size;
    }

    /**
    * ������, ��������� �� ����� �����.
    */
    bool file_end_reached() const {
        return file_stream.eof();
    }

    /**
    * ��������� ����� �����, ������������ �������� ������ �����.
    * @param buffer �������� ��� ����� ����������� ������.
    */
    size_t read_block(std::vector<uint8_t>& buffer) {
        file_stream.read((char*)buffer.data(), block_size);

        return (size_t)file_stream.gcount();
    }

private:
    std::ifstream file_stream;
    size_t block_size;
};