#pragma once

#include "type_aliases.h"

#include <cstddef>
#include <cstdint>
#include <iostream>

#include <boost/crc.hpp> 
#include <boost/uuid/detail/md5.hpp>

using namespace boost;
using namespace boost::uuids::detail;

/**
* @brief Абстрактный класс реализующий интерфейс для хеширования блока данных.
*/
class BlockHasher
{
public:
    /**
    * Виртуальный метод хеширования блока данных.
    * @param data_block блок данных из файла.
    * @return хеш данных.
    */
    virtual ui_vector hash_block(ui_vector data_block) = 0;
};

/**
* @brief Класс реализует crc32 хеширование.
*/
class HasherCrc32 : public BlockHasher
{
public:
    /**
    * Хеширование блока данных алгоритмом CRC32.
    * @param data_block блок данных из файла.
    * @return хеш данных.
    */
    ui_vector hash_block(ui_vector data_block) override {
        crc_32_type crc32;

        crc32.process_bytes(data_block.data(), data_block.size());
        unsigned int checksum = crc32.checksum();

        auto byte_array = reinterpret_cast<uint8_t*>(&checksum);

        return ui_vector(byte_array, byte_array + sizeof(checksum));
    }
};

/**
* @brief Класс реализует md5 хеширование.
*/
class HasherMd5 : public BlockHasher
{
public:
    /**
    * Хеширование блока данных алгоритмом Md5.
    * @param data_block блок данных из файла.
    * @return хеш данных.
    */
    ui_vector hash_block(ui_vector data_block) override {
        md5 hash;
        md5::digest_type digest;

        hash.process_bytes(data_block.data(), data_block.size());
        hash.get_digest(digest);

        auto byte_array = reinterpret_cast<uint8_t*>(digest);

        return ui_vector(byte_array, byte_array + sizeof(digest));
    }
};






