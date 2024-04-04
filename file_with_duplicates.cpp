#include "file_with_duplicates.h"

// �������� ��� ����
ui_vector& FileWithDuplicates::Iterator::operator*()
{
    // ���������� ����, ���� �������� �������.
    if (hb_iterator != hashed_blocks.end()) {
        return *hb_iterator;
    }

    // ���������, ����� �� ������ ����.
    if (!reader_ptr || reader_ptr->is_eof()) {
        throw std::range_error("Attempt to read iterator that has reached end.");
    }

    // ������ ���� ������ �� �����.
    auto bytes_read = reader_ptr->read_block(buffer);
    auto delta = reader_ptr->get_block_size() - bytes_read;

    // ���� ������ ������, ��� ������ �����, ������� ��������� ������.
    for (size_t i = 0; i < delta; i++) {
        buffer.push_back(static_cast<uint8_t>(0));
    }

    // �������� ���������� ���� ������.
    auto hashed_block = hasher_ptr->hash_block(buffer);

    // ��������� � ��������� ��� ������.
    hb_iterator = hashed_blocks.insert(hb_iterator, hashed_block);

    return *hb_iterator;
}

// �������� ��������� �� ��� ����
ui_vector* FileWithDuplicates::Iterator::operator->()
{
    return &(*hb_iterator);
}

// �������
FileWithDuplicates::Iterator& FileWithDuplicates::Iterator::operator++()
{
    hb_iterator++;
    return *this;
}

// ��������
FileWithDuplicates::Iterator FileWithDuplicates::Iterator::operator++(int)
{
    Iterator tmp = *this;
    ++(*this);
    return tmp;
}

// 
bool operator==(const FileWithDuplicates::Iterator& li, const FileWithDuplicates::Iterator& ri)
{
    if (li.hb_iterator != b.hb_iterator) {
        return false;
    }

    return (li.reader_ptr && !li.reader_ptr->is_eof()) == (ri.reader_ptr && !ri.reader_ptr->is_eof());
}

// 
bool operator!=(const FileWithDuplicates::Iterator& li, const FileWithDuplicates::Iterator& ri)
{
    if (li.hb_iterator != ri.hb_iterator) {
        return true;
    }

    return (li.reader_ptr && !li.reader_ptr->is_eof()) != (ri.reader_ptr && !ri.reader_ptr->is_eof());
}