#pragma once

#include "directory_excluder.h"

/**
* @param iterator итератор обхода по содержимому директории
* @return Указывает ли итератор на директорю
*/
template<typename TraversalIterator>
bool path_is_directory(const TraversalIterator& iterator)
{
    return boost::filesystem::is_directory(iterator->path().string());
}

/**
* @brief Класс реализует обход содержимого директории.
*/
template <typename TraversalIterator>
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
        path_filter = std::make_unique<TraversalExcluder<TraversalIterator>>(parameters.exclude_dirs,
            parameters.file_masks, parameters.min_file_size);

        // Итератор по директориям поиска файлов.
        scan_dirs_iterator = scan_dirs.cbegin();
        // Пропускаем папки не содержащие файлов.
        for (; scan_dirs_iterator != scan_dirs.cend(); ++scan_dirs_iterator) {
            if (!is_empty(*scan_dirs_iterator)) {
                break;
            }
        }
        
        // Итератор по содержимому директории.
        files_iterator = scan_dirs_iterator != scan_dirs.cend()
            ? TraversalIterator(*scan_dirs_iterator)
            : TraversalIterator();

        files_iterator_end = TraversalIterator();

        // Если это папка или файл не проходит фильтрацию, переходим к следующему.
        if (files_iterator != TraversalIterator{}) {
            if (path_is_directory(files_iterator) || !path_filter->check_filename_mask(files_iterator)) {
                find_next_file();
            }
        }
    }

    /**
    * @return Были ли проверены все файлы во всех заданных директориях.
    */
    bool traverse_finished() const override
    {
        return (files_iterator == files_iterator_end) && (scan_dirs_iterator == scan_dirs.cend());
    }

    /**
    * @return Объект, представляющий файл, который может иметь дубликаты или сам быть дубликатом одного из коллекции.
    */
    FileWithDuplicates get_next_file() override
    {
        std::string path = files_iterator->path().string();
        
        find_next_file();
        
        return FileWithDuplicates(path, boost::filesystem::file_size(path),
                                  hasher_ptr,
                                  std::move(std::make_shared<FileReader>(path, block_size))
                                  );
    }

private: // methods
    /**
    * @brief Пробуем перейти к следующему файлу для анализа.
    */
    void find_next_file()
    {
        do {
            // Если это директория, осуществляем проверку, входит ли она в состав исключенных.
            if (path_is_directory(files_iterator)) {
                path_filter->filter_excluded_dirs(files_iterator);
            }
            
            ++files_iterator;
            
            if (files_iterator == files_iterator_end) {
                ++scan_dirs_iterator; // Переход к следующей папке в списке.
                // Пропускаем папки не содержащие файлов.
                for (; scan_dirs_iterator != scan_dirs.cend(); ++scan_dirs_iterator) {
                    if (!is_empty(*scan_dirs_iterator)) {
                        break;
                    }
                }

                if (scan_dirs_iterator != scan_dirs.cend()) {
                    files_iterator = TraversalIterator(*scan_dirs_iterator);
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

    std::unique_ptr<TraversalExcluder<TraversalIterator>> path_filter; // Для фильтрации того, что анализировать не будем.

    str_vector::const_iterator scan_dirs_iterator; // Итератор по директориям.

    TraversalIterator files_iterator; // Итератор по содержимому директории.
    TraversalIterator files_iterator_end;
};