#pragma once

#include "directory_excluder.h"

/**
* @brief ����� ��������� ������� ������ � ����������.
*/
template <typename IteratorType>
class DirectoryTraversal : public Traversal
{
public:
    DirectoryTraversal(const Parameters& parameters, hasher_shared hasher)
        : scan_dirs(parameters.scan_dirs)
        , block_size(parameters.block_size)
        , hasher_ptr(hasher)
    {
        // ������, ��������� �� ������� ����������� �����, ����� �������������� ������� � �����,
        // ����� ������� �� �������� ��� �������� �����.
        path_filter = std::make_unique<TraversalExcluder<IteratorType>>(parameters.exclude_dirs,
            parameters.file_masks, parameters.min_file_size);

        // �������� �� ����������� ������ ������.
        scan_dirs_iterator = scan_dirs.cbegin();

        // �������� �� ������.
        files_iterator = (scan_dirs_iterator != scan_dirs.cend())
            ? IteratorType(*scan_dirs_iterator)
            : IteratorType();

        files_iterator_end = IteratorType();

        // ���� ��� ����� ��� ���� �� �������� ����������, ��������� � ����������.
        if (is_directory(files_iterator) || !path_filter->check_filename_mask(files_iterator)) {
            move_to_next_file();
        }
    }

    /**
    * @return ��� �� �������� ����� ���� �������� ����������.
    */
    bool traverse_finished() const override
    {
        return (files_iterator == files_iterator_end) && (scan_dirs_iterator == scan_dirs.cend());
    }

    /**
    * @return ������, �������������� ����, ������� ����� ����� ��������� ��� ��� ���� ���������� ������ �� ���������.
    */
    FileWithDuplicates get_next_file() override
    {
        std::string path = files_iterator->path().string();

        move_to_next_file();

        return FileWithDuplicates(path, boost::filesystem::file_size(path),
                                  hasher_ptr,
                                  std::move(std::make_shared<FileReader>(path, block_size))
                                  );
    }

private: // methods
    /**
    * 
    */
    void move_to_next_file()
    {
        do {
            // ���� ��� ����������, ������������ ��������, ������ �� ��� � ������ �����������.
            if (is_directory(files_iterator)) {
                path_filter->filter_excluded_dirs(files_iterator);
            }

            ++files_iterator;

            if (files_iterator == files_iterator_end) {
                ++scan_dirs_iterator;

                if (scan_dirs_iterator != scan_dirs.cend()) {
                    files_iterator = IteratorType(*scan_dirs_iterator);
                }
            }
        } while (!traverse_finished() && (is_directory(files_iterator) || !path_filter->check_filename_mask(files_iterator)));
    }

private: // data
    str_vector scan_dirs;
    size_t block_size;

    hasher_shared hasher_ptr;

    std::unique_ptr<TraversalExcluder<IteratorType>> path_filter;

    str_vector::const_iterator scan_dirs_iterator;

    IteratorType files_iterator;
    IteratorType files_iterator_end;
};