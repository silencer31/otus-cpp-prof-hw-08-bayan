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

using namespace boost::filesystem;

int main(int args_number, char const** args)
{
    // Объект для чтения параметров - аргументов программы.
    const std::unique_ptr<OptionsReader> opt_reader_ptr = std::make_unique<OptionsReader>();

    // Набор аргументов - параметров программы.
    const Parameters parameters = opt_reader_ptr->read_arguments(args_number, args);
    
    if (parameters.show_help) {
        std::cout << parameters.help_text << std::endl;
        return 0;
    }
    
    // Теcтовый вывод прочитанных аргументов.
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
    
    // Указатель на объект для обхода директорий с целью поиска дубликатов.
    std::unique_ptr<Traversal> directory_traversal_ptr;
        
    if (parameters.scan_all_dirs) {
        // Рекурсивный обход директорий.
        directory_traversal_ptr = std::make_unique<DirectoryTraversal<recursive_directory_iterator>>(
            parameters, std::move(block_hasher_ptr));
    }
    else { 
        // Анализ файлов только внутри заданной директории.
        directory_traversal_ptr = std::make_unique<DirectoryTraversal<directory_iterator>>(
            parameters, std::move(block_hasher_ptr));
    }

    // Коллектор дубликатов файлов.
    BayanCollector collector(std::move(directory_traversal_ptr));

    // Выполняем поиск дубликатов по заданным условиям.
    collector.scan_for_duplicates(); 
    /*
    bool first = true;

    // Цикл по коллекции с файлами, у каждого из которых есть своя коллекция дубликатов.
    for (const auto& file : searcher.mComparisonFiles)
    {
        if (!first)
        {
            std::cout << endl;
        }
        else
        {
            first = false;
        }

        // Вывод пути к файлу, имеющему дубликаты.
        std::cout << file.get_path() << std::endl;

        // Вывод всех дубликатов.
        for (const auto& duplicate : file.get_duplicates())
        {
            std::cout << duplicate << std::endl;
        }
    }*/

    return 0;
}