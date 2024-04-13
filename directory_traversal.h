#pragma once

#include "directory_excluder.h"

/**
* ��������� �� �������� �� ���������
* @param iterator ��������
*/
template<typename IteratorType>
bool path_is_directory(const IteratorType& iterator)
{
    return boost::filesystem::is_directory(iterator->path().string());
}

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
        // ���������� ����� �� ���������� ������.
        for (; scan_dirs_iterator != scan_dirs.cend(); ++scan_dirs_iterator) {
            if (!is_empty(*scan_dirs_iterator)) {
                break;
            }
        }
        
        // �������� �� ������.
        files_iterator = scan_dirs_iterator != scan_dirs.cend()
            ? IteratorType(*scan_dirs_iterator)
            : IteratorType();

        files_iterator_end = IteratorType();

        // ���� ��� ����� ��� ���� �� �������� ����������, ��������� � ����������.
        if (files_iterator != IteratorType{}) {
            if (path_is_directory(files_iterator) || !path_filter->check_filename_mask(files_iterator)) {
                move_to_next_file();
            }
        }
    }

    /**
    * @return ���� �� ��������� ��� ����� �� ���� �������� �����������.
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
    * @brief ������� ������� � ���������� ����� ��� �������.
    */
    void move_to_next_file()
    {
        do {
            // ���� ��� ����������, ������������ ��������, ������ �� ��� � ������ �����������.
            if (path_is_directory(files_iterator)) {
                path_filter->filter_excluded_dirs(files_iterator);
            }
            
            ++files_iterator;
            
            if (files_iterator == files_iterator_end) {
                ++scan_dirs_iterator; // ������� � ��������� ����� � ������.
                // ���������� ����� �� ���������� ������.
                for (; scan_dirs_iterator != scan_dirs.cend(); ++scan_dirs_iterator) {
                    if (!is_empty(*scan_dirs_iterator)) {
                        break;
                    }
                }

                if (scan_dirs_iterator != scan_dirs.cend()) {
                    files_iterator = IteratorType(*scan_dirs_iterator);
                }
            }
            
            if (traverse_finished()) {
                break;
            }
        } while (path_is_directory(files_iterator) || !path_filter->check_filename_mask(files_iterator));
    }

private: // data
    str_vector scan_dirs;
    size_t block_size;

    hasher_shared hasher_ptr;

    std::unique_ptr<TraversalExcluder<IteratorType>> path_filter;

    str_vector::const_iterator scan_dirs_iterator; // �������� �� �����������.

    IteratorType files_iterator; // �������� �� ������.
    IteratorType files_iterator_end;
};