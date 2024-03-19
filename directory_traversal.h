#pragma once

#include "traversal.h"
#include "parameters.h"
#include "file_reader.h"

#include <boost/filesystem.hpp>
#include <boost/regex.hpp>

#include <type_traits>

/**
* @brief Класс реализует перебор файлов в директории.
*/
template <typename IteratorType>
class DirectoryTraversal : public Traversal
{
public:


private:
    std::vector<std::string> mDirectories;

    std::unique_ptr<TraversalExcluder<IteratorType>> mPathFilter;

    std::vector<std::string>::const_iterator mDirectoriesIterator;

    IteratorType mFilesIterator;
    IteratorType mFilesIteratorEnd;

    size_t mFileBlockSize;

    std::shared_ptr<Hasher> mHasher;


};