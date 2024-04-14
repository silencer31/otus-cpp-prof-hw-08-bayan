#pragma once

#include "block_hasher.h"
#include "file_reader.h"

using hasher_shared = std::shared_ptr<BlockHasher>;
using reader_shared = std::shared_ptr<FileReader>;

/**
* @brief Класс отображает некий файл, имеющий свою коллекцию дубликатов.
*/
class FileWithDuplicates
{
public:
    FileWithDuplicates(const std::string& path, size_t fs,
        hasher_shared bh_ptr, reader_shared fr_ptr)
        : file_path(path)
        , file_size(fs)
        , hasher_ptr(std::move(bh_ptr))
        , reader_ptr(std::move(fr_ptr))
    {}
    
    /**
    * @brief Структура, позволяющая реализовать поблочное сравнение файлов путём сравнения хеш блоков.
    */
    struct Iterator
    {
        Iterator(hb_list_vector& blocks, hasher_shared bh_ptr, reader_shared fr_ptr)
            : hashed_blocks(blocks)
            , hb_iterator(hashed_blocks.begin())
            , hasher_ptr(bh_ptr)
            , reader_ptr(fr_ptr)
        {
            if (reader_ptr) {
                buffer.reserve(reader_ptr->get_block_size());
            }
        }

        Iterator(hb_list_vector& blocks)
            : hashed_blocks(blocks)
            , hb_iterator(hashed_blocks.end())
            , hasher_ptr( hasher_shared(nullptr))
            , reader_ptr( reader_shared(nullptr))
        {}

        /**
        * @brief хеш блок.
        */
        ui_vector& operator*();

        /**
        * @brief указатель на хеш блок.
        */
        ui_vector* operator->();

        /**
        * @brief префикс
        */
        Iterator& operator++();

        /**
        * @brief постфикс
        */
        Iterator  operator++(int);

        // Операторы для сравнения хеш блоков.
        friend bool operator== (const Iterator& li, const Iterator& ri);
        friend bool operator!= (const Iterator& li, const Iterator& ri);

    private:
        hb_list_vector& hashed_blocks;          // Ссылка на список с набором хешированных блоков.
        hb_list_vector::iterator hb_iterator;   // Итератор по коллекции хеш блоков.

        hasher_shared hasher_ptr;
        reader_shared reader_ptr;
            
        ui_vector buffer; // Один хеш блок.
    };

    Iterator begin() { return Iterator(hashed_blocks, hasher_ptr, reader_ptr); }
    Iterator end()   { return Iterator(hashed_blocks); }
    

    /**
    * @return Путь к файлу.
    */
    std::string get_file_path() const {
        return file_path;
    }

    /**
    * @return Размер файла.
    */
    std::size_t get_file_size() const {
        return file_size;
    }

    /**
    * Добавить путь к дубликату файла в его коллекцию.
    * @param path путь к дубликату.
    */
    void add_duplicate(const std::string& path) {
        duplicate_paths.push_back(path);
    }

    /**
    * @return Список путей к дубликатам.
    */
    fp_list get_duplicates() const {
        return duplicate_paths;
    }

    /**
    * @return Отсутствуют ли дубликаты у данного файла.
    */
    bool no_duplicates() {
        return duplicate_paths.empty();
    }

private:
	std::string file_path;  // Путь к файлу.
	size_t file_size;		// Размер файла

    hasher_shared hasher_ptr;
    reader_shared reader_ptr;

	hb_list_vector hashed_blocks; // Коллекция хешей, представляющая данный файл.
	fp_list duplicate_paths;	  // Коллекция путей к файлам дубликатам.
};

