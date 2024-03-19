#include "file_duplicate.h"

uivector& FileDuplictate::Iterator::operator*()
{
    if (hb_iterator != hashed_blocks.end()) {
        return *hb_iterator;
    }

    if (!reader_ptr || reader_ptr->is_eof()) {
        throw std::range_error("Attempt to read iterator that has reached end.");
    }

    auto read_cont = reader_ptr->read_block(buffer);
    auto pad_count = reader_ptr->get_block_size() - read_cont;

    for (size_t i = 0; i < pad_count; i++) {
        buffer.push_back(static_cast<uint8_t>(0));
    }

    auto hashed_block = hasher_ptr->hash_block(buffer);
    hb_iterator = hashed_blocks.insert(hb_iterator, hashed_block);

    return *hb_iterator;
}

uivector* FileDuplictate::Iterator::operator->()
{
    return &(*hb_iterator);
}

FileDuplictate::Iterator& FileDuplictate::Iterator::operator++()
{
    hb_iterator++;
    return *this;
}

FileDuplictate::Iterator FileDuplictate::Iterator::operator++(int)
{
    Iterator tmp = *this;
    ++(*this);
    return tmp;
}


bool operator==(
    const FileDuplictate::Iterator& a,
    const FileDuplictate::Iterator& b)
{
    if (a.hb_iterator != b.hb_iterator) {
        return false;
    }

    return (a.reader_ptr && !a.reader_ptr->is_eof()) == (b.reader_ptr && !b.reader_ptr->is_eof());
}

bool operator!=(
    const FileDuplictate::Iterator& a,
    const FileDuplictate::Iterator& b)
{
    if (a.hb_iterator != b.hb_iterator)
    {
        return true;
    }

    return (a.reader_ptr && !a.reader_ptr->is_eof()) != (b.reader_ptr && !b.reader_ptr->is_eof());
}