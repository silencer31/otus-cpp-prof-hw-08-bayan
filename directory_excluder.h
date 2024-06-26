#pragma once

#include "traversal.h"
#include "parameters.h"

#include <boost/filesystem.hpp>
#include <boost/regex.hpp>
#include <sstream>
#include <string>
#include <type_traits>

/**
* @brief ����� ��������� ���������� �� ������� ������������ ������ � ����������� ��� ������ �����.
*/
template <typename TraversalIterator>
class TraversalFilter
{
public:
    TraversalFilter<TraversalIterator>(
        const str_vector& eds, const str_vector& fmasks, size_t mfs)
        : excluded_dirs(eds)
        , min_file_size(mfs)
    {
        file_masks.reserve(fmasks.size());

        // ���������� ����� ��� ���������� ������.
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
    * @brief ���������� �����, ����������� �� ������ ���������� ������.
    * @param iterator �������� ������ �����������.
    */
    void filter_excluded_dirs(TraversalIterator& iterator)
    {
        // ���� ���� ����� ������ ����������, ����������� ��� �������.
        auto iter = std::find(excluded_dirs.begin(), excluded_dirs.end(), iterator->path().string());

        if (iter == excluded_dirs.end()) {
            return; // �������, ���� �� �����.
        }

        exclude_directory(iterator);
    }

    /**
    * @brief �������� ������� � ����� �����.
    * @param iterator �������� ������ �����������.
    * @return �������� �� ���� ��� �������� �������.
    */
    bool check_filename_mask(const TraversalIterator& iterator)
    {
        std::string str_path = iterator->path().string();

        // ���������, ����� ������ ����� �� ��� ������ ������������ ��������.
        if (boost::filesystem::file_size(str_path) < min_file_size) {
            return false;
        }

        // ������������, ���� ��� ����.
        if (boost::filesystem::is_regular_file(str_path)) {
            // �������� ����� ����� �� ������������ �������� ������.
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

        // ���� ����� �� ������, ���������� true
        return file_masks.empty();
    }

private: // methods
    /**
    * @brief ��������� ����� �� �������.
    * @param iterator �������� ������ �����������.
    */
    virtual void exclude_directory(TraversalIterator& iterator) {
        (void)iterator;
    }

private: // data
    str_vector excluded_dirs;
    const size_t min_file_size = 0;

    std::vector<boost::regex> file_masks;

    const std::vector<str_pair> glob_to_regex = { {"\\.", "\\\\."}, {"\\*", ".*"}, {"\\?", "."} };
};

/**
* @brief ���������� ��� ������ ����������� � ����� �����.
*/
template <typename TraversalIterator>
class TraversalExcluder : public TraversalFilter<TraversalIterator>
{
public:
    TraversalExcluder(const str_vector& eds, const str_vector& fmasks, size_t mfs)
        : TraversalFilter<TraversalIterator>(eds, fmasks, mfs)
    {}
};

/**
* @brief ���������� ��� ������������ ������ ����������� �����.
*/
template<>
class TraversalExcluder<boost::filesystem::recursive_directory_iterator>
    : public TraversalFilter<boost::filesystem::recursive_directory_iterator>
{
public:
    TraversalExcluder(const str_vector& eds, const str_vector& fmasks, size_t mfs)
        : TraversalFilter(eds, fmasks, mfs)
    {}

private:
    /**
    * @brief ��������� ���������� �� �������.
    */
    void exclude_directory(boost::filesystem::recursive_directory_iterator& iterator)
    {
        // Disables recursion to the currently referred subdirectory, if any.
        iterator.disable_recursion_pending();
    }
};