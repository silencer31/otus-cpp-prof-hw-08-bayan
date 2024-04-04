#pragma once

#include "directory_excluder.h"

/**
* @brief Класс реализует перебор файлов в директории.
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
        // Фильтр, убирающий из анализа исключенные папки, файлы недостаточного размера и файлы,
        // имена которых не подходят под заданные маски.
        path_filter = std::make_unique<TraversalExcluder<IteratorType>>(parameters.exclude_dirs,
            parameters.file_masks, parameters.min_file_size);

        // Итератор по директориям поиска файлов.
        scan_dirs_iterator = scan_dirs.cbegin();

        // Итератор по файлам.
        files_iterator = (scan_dirs_iterator != scan_dirs.cend())
            ? IteratorType(*scan_dirs_iterator)
            : IteratorType();

        files_iterator_end = IteratorType();

        // Если это папка или файл не проходит фильтрацию, переходим к следующему.
        if (is_directory(files_iterator) || !path_filter->check_filename_mask(files_iterator)) {
            move_to_next_file();
        }
    }

    /**
    * @return был ли заевршен обход всех заданных директорий.
    */
    bool traverse_finished() const override
    {
        return (files_iterator == files_iterator_end) && (scan_dirs_iterator == scan_dirs.cend());
    }

    /**
    * @return объект, представляющий файл, который может иметь дубликаты или сам быть дубликатом одного из коллекции.
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
            // Если это директория, осуществляем проверку, входил ли она в состав исключенных.
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