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
    // ������ ��� ������ ���������� - ���������� ���������.
    const std::unique_ptr<OptionsReader> opt_reader_ptr = std::make_unique<OptionsReader>();

    // ����� ���������� - ���������� ���������.
    const Parameters parameters = opt_reader_ptr->read_arguments(args_number, args);
    
    if (parameters.show_help) {
        std::cout << parameters.help_text << std::endl;
        return 0;
    }

    // ����� ����� ������ �� �����.
    std::shared_ptr<BlockHasher> block_hasher_ptr; 
    if (parameters.hash_algorithm == HashAlgoritm::MD5) {
        block_hasher_ptr = std::make_shared<HasherMd5>();
    }
    else {
        block_hasher_ptr = std::make_shared<HasherCrc32>();
    }
    
    // ��������� �� ������ ��� ������ ���������� � ����� ������ ����������.
    std::unique_ptr<Traversal> directory_traversal_ptr;
        
    if (parameters.scan_all_dirs) {
        // ����������� ����� ����������.
        directory_traversal_ptr = std::make_unique<DirectoryTraversal<recursive_directory_iterator>>(
            parameters, std::move(block_hasher_ptr));
    }
    else { 
        // ������ ������ ������ ������ �������� ����������.
        directory_traversal_ptr = std::make_unique<DirectoryTraversal<directory_iterator>>(
            parameters, std::move(block_hasher_ptr));
    }

    // ��������� ���������� ������.
    BayanCollector collector(std::move(directory_traversal_ptr));
    
    // ��������� ����� ���������� �� �������� ��������.
    collector.scan_for_duplicates(); 
 
    // �������, ���� ��������� �� ���� �������.
    if (collector.nothing_found()) {
        //std::cout << "\nNo duplicates have been found" << std::endl;
        return 0;
    }

    //std::cout << "\nScan results:" << std::endl;

    // ���� �� ��������� � �������, � ������� �� ������� ���� ���� ��������� ����������.
    for (auto iter = collector.cbegin(); iter != collector.cend(); )
    {
        // ����� ���� � �����, �������� ���������.
        std::cout << iter->get_file_path() << std::endl;

        // ����� ���� ��� ����������.
        for (const auto& duplicate : iter->get_duplicates())
        {
            std::cout << duplicate << std::endl;
        }

        ++iter;

        if (iter == collector.cend()) {
            break;
        }

        // ��������� ������ ������ ������ �������.
        std::cout << std::endl;
    }

    return 0;
}