#pragma once

#include "traversal.h"
#include "parameters.h"

#include <boost/filesystem.hpp>
#include <boost/regex.hpp>

#include <type_traits>

template<typename IteratorType>
bool is_directory(const IteratorType& iterator)
{
    return boost::filesystem::is_directory(iterator->path().string());
}

/**
* @brief Класс реализует 
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

    void filter_exclude_dirs(IteratorType& iterator)
    {
        if (std::find(excluded_dirs.begin(), excluded_dirs.end(), iterator->path().string()) != excluded_dirs.end())
        {
            exclude_directory(iterator);
        }
    }

    bool should_include_file(const IteratorType& iterator)
    {
        auto path = iterator->path().string();

        if (boost::filesystem::file_size(path) < min_file_size) {
            return false;
        }

        if (boost::filesystem::is_regular_file(path)) {
            for (const auto& mask : file_masks) {
                boost::smatch what;
                auto path = iterator->path();
                
                if (boost::regex_match(path.filename().string(), what, mask)) {
                    return true;
                }
            }
        }

        return file_masks.empty();
    }

private: // methods
    virtual void exclude_directory(IteratorType& iterator) {
        (void)iterator;
    }

private: // data
    str_vector excluded_dirs;
    const size_t min_file_size = 0;

    std::vector<boost::regex> file_masks;
 
    // not using map as the order matters
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
    void exclude_directory(boost::filesystem::recursive_directory_iterator& iterator)
    {
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
    BoostDirectoryTraversal(const Parameters& parameters, std::shared_ptr<BlockHasher> hasher)
        : scan_dirs(parameters.scan_dirs)
        , block_size(parameters.block_size)
        , block_hasher(hasher)
    {
        path_filter = std::make_unique<TraversalExcluder<IteratorType>>(parameters.exclude_dirs, parameters.file_masks, parameters.min_file_size);

        scan_dirs_iterator = scan_dirs.cbegin();

        files_iterator = (scan_dirs_iterator != scan_dirs.cend())
            ? IteratorType(*scan_dirs_iterator)
            : IteratorType();

        files_iterator_end = IteratorType();

        if (is_directory(files_iterator) || !path_filter->should_include_file(files_iterator)) {
            move_to_next_file();
        }
    }



    bool dir_traversed() const override
    {
        return (files_iterator == files_iterator_end) && (scan_dirs_iterator == scan_dirs.cend());
    }

    FileDuplictate get_next_file() override
    {
        std::string path = files_iterator->path().string();

        move_to_next_file();

        return FileDuplictate(
            path,
            boost::filesystem::file_size(path),
            std::move(std::make_shared<FileReader>(path, block_size)),
            block_hasher);
    }

private: // methods
    void move_to_next_file()
    {
        do {
            if (is_directory(files_iterator)) {
                path_filter->filter_exclude_dirs(files_iterator);
            }

            ++files_iterator;

            if (files_iterator == files_iterator_end) {
                ++scan_dirs_iterator;

                if (scan_dirs_iterator != scan_dirs.cend()) {
                    files_iterator = IteratorType(*scan_dirs_iterator);
                }
            }
        } while (!dir_traversed() && (is_directory(files_iterator) || !path_filter->should_include_file(files_iterator)));
    }

private: // data
    str_vector scan_dirs;
    size_t block_size;
    std::shared_ptr<BlockHasher> block_hasher;

    std::unique_ptr<TraversalExcluder<IteratorType>> path_filter;

    str_vector::const_iterator scan_dirs_iterator;

    IteratorType files_iterator;
    IteratorType files_iterator_end;
};