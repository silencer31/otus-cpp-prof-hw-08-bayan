#include "file_with_duplicates.h"

// Получить хеш блок
ui_vector& FileWithDuplicates::Iterator::operator*()
{
    // Возвращаем блок, если итератор валиден.
    if (hb_iterator != hashed_blocks.end()) {
        return *hb_iterator;
    }

    // Проверяем, можно ли читать файл.
    if (!reader_ptr || reader_ptr->file_end_reached()) {
        throw std::range_error("File read range error!");
    }

    // Читаем блок данных из файла.
    auto bytes_read = reader_ptr->read_block(buffer);
    auto delta = reader_ptr->get_block_size() - bytes_read;

    // Если данных меньше, чем размер блока, остаток заполняем нулями.
    for (size_t i = 0; i < delta; i++) {
        buffer.push_back(static_cast<uint8_t>(0));
    }

    // Хешируем полученный блок данных.
    auto hashed_block = hasher_ptr->hash_block(buffer);

    // Вставляем в коллекцию хеш блоков.
    hb_iterator = hashed_blocks.insert(hb_iterator, hashed_block);

    return *hb_iterator;
}

// Получить указатель на хеш блок
ui_vector* FileWithDuplicates::Iterator::operator->()
{
    return &(*hb_iterator);
}

// префикс
FileWithDuplicates::Iterator& FileWithDuplicates::Iterator::operator++()
{
    hb_iterator++;
    return *this;
}

// постфикс
FileWithDuplicates::Iterator FileWithDuplicates::Iterator::operator++(int)
{
    Iterator tmp = *this;
    ++(*this);
    return tmp;
}

// 
bool operator==(const FileWithDuplicates::Iterator& li, const FileWithDuplicates::Iterator& ri)
{
    if (li.hb_iterator != ri.hb_iterator) {
        return false;
    }

    return (li.reader_ptr && !li.reader_ptr->file_end_reached()) == (ri.reader_ptr && !ri.reader_ptr->file_end_reached());
}

// 
bool operator!=(const FileWithDuplicates::Iterator& li, const FileWithDuplicates::Iterator& ri)
{
    if (li.hb_iterator != ri.hb_iterator) {
        return true;
    }

    return (li.reader_ptr && !li.reader_ptr->file_end_reached()) != (ri.reader_ptr && !ri.reader_ptr->file_end_reached());
}