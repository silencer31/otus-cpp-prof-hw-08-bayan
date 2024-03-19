#include "options_reader.h"
#include "block_hasher.h"
#include "directory_traversal.h"
#include "bayan_collector.h"

#include <memory>
#include <iostream>

#ifdef WIN32
#define appname "bayan.exe"
#else
#define appname "./bayan"
#endif

//namespace ba = boost::asio;

int main(int args_number, char const** args)
{
    const std::unique_ptr<OptionsReader> opt_reader_ptr = std::make_unique<OptionsReader>();

    // Набор аргументов - параметров программы.
    const Parameters parameters = opt_reader_ptr->read_arguments(args_number, args);
    
    if (parameters.show_help) {
        std::cout << parameters.help_text << std::endl;
        return 0;
    }
    
    for (const auto& value : parameters.scan_dirs ) {
        std::cout << value << std::endl;
    }

    // Хэшер блока данных из файла.
    std::shared_ptr<BlockHasher> block_hasher_ptr; 
    if (parameters.hash_algorithm == HashAlgoritm::MD5) {
        block_hasher_ptr = std::make_shared<HasherMd5>();
    }
    else {
        block_hasher_ptr = std::make_shared<HasherCrc32>();
    }
    
    // Обходчик директорий.
    std::unique_ptr<Traversal> directory_traversal_ptr;

    // Рекурсивный обход директорий
    if (parameters.scan_all_dirs)
    {
        // Получает все настройки и хешер
        directory_traversal_ptr = std::make_unique<DirectoryTraversal<directory_iterator>>(
            parameters, std::move(block_hasher_ptr));
    }
    else // Обход только текущей директории
    {
        directory_traversal_ptr = std::make_unique<DirectoryTraversal<recursive_directory_iterator>>(
            parameters, std::move(block_hasher_ptr));
    }

    return 0;
}