#pragma once

#include "traversal.h"
#include "parameters.h"

#include <boost/filesystem.hpp>
#include <boost/regex.hpp>
#include <sstream>
#include <string>
#include <type_traits>


/**
* Указывает ли итератор на директорю
* @param iterator итератор
*/
template<typename IteratorType>
bool is_directory(const IteratorType& iterator)
{
    return boost::filesystem::is_directory(iterator->path().string());
}

/**
* @brief Класс реализует исключение из анализа неподходящих файлов и запрещённых для поиска папок.
*/
template <typename IteratorType>
class TraversalExcluderBase
{
public:
    TraversalExcluderBase<IteratorType>(
        const str_vector& eds, const str_vector& fmasks, size_t mfs)
        : excluded_dirs(eds)
        , min_file_size(mfs)
    {
        file_masks.reserve(fmasks.size());

        // Подготовка масок для фильтрации файлов.
        for (const auto& glob_mask : fmasks) {
            auto replaced_mask = glob_mask;

            for (const auto& pair : glob_to_regex) {
                std::stringstream stream;
                std::ostream_iterator<char> out_iterator(stream);

                boost::regex_replace(out_iterator, replaced_mask.begin(), replaced_mask.end(), boost::regex(pair.first), pair.second);

                replaced_mask = stream.str();
            }

            file_masks.push_back(boost::regex("^" + replaced_mask + "$"));
        }
    }

    /**
    * Фильтрация путей, исключенных из поиска дубликатов файлов.
    * @param iterator итератор на путь
    */
    void filter_excluded_dirs(IteratorType& iterator)
    {
        // Ищем путь среди списка директорий, исключенных для анализа.
        auto iter = std::find(excluded_dirs.begin(), excluded_dirs.end(), iterator->path().string());
        if (iter == excluded_dirs.end()) {
            return; // Выходим, если не нашли.
        }

        exclude_directory(iterator);
    }

    /**
    * Проверка размера и имени файла.
    * @param iterator
    */
    bool check_filename_mask(const IteratorType& iterator)
    {
        std::string str_path = iterator->path().string();

        // Проверяем, чтобы размер файла не был меньше минимального значения.
        if (boost::filesystem::file_size(str_path) < min_file_size) {
            return false;
        }

        // Обрабатываем, если это файл.
        if (boost::filesystem::is_regular_file(str_path)) {
            // Проверка имени файла на соответствие заданным маскам.
            for (const auto& mask : file_masks) {
                boost::smatch what;
                //std::filesystem::path::filename
                //auto path = iterator->path();
                std::string str_filename = iterator->path().filename().string();

                if (boost::regex_match(str_filename, what, mask)) {
                    return true;
                }
            }
        }

        // Если маски не заданы, возвращаем true
        return file_masks.empty();
    }

private: // methods
    /**
    *
    * @param iterator
    */
    virtual void exclude_directory(IteratorType& iterator) {
        (void)iterator;
    }

private: // data
    str_vector excluded_dirs;
    const size_t min_file_size = 0;

    std::vector<boost::regex> file_masks;

    // 
    const std::vector<str_pair> glob_to_regex = { {"\\.", "\\\\."}, {"\\*", ".*"}, {"\\?", "."} };
};


template <typename IteratorType>
class TraversalExcluder : public TraversalExcluderBase<IteratorType>
{
public:
    TraversalExcluder(const str_vector& eds, const str_vector& fmasks, size_t mfs)
        : TraversalExcluderBase<IteratorType>(eds, fmasks, mfs)
    {}
};


template<>
class TraversalExcluder<boost::filesystem::recursive_directory_iterator>
    : public TraversalExcluderBase<boost::filesystem::recursive_directory_iterator>
{
public:
    TraversalExcluder(const str_vector& eds, const str_vector& fmasks, size_t mfs)
        : TraversalExcluderBase(eds, fmasks, mfs)
    {}

private:
    /**
    * @brief исключает директорию из анализа
    */
    void exclude_directory(boost::filesystem::recursive_directory_iterator& iterator)
    {
        // Disables recursion to the currently referred subdirectory, if any.
        iterator.disable_recursion_pending();
    }
};

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