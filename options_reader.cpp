#include "options_reader.h"

#include <boost/filesystem/operations.hpp>
#include <boost/program_options.hpp>
#include <boost/any.hpp>

#include <sstream>

using namespace std;
namespace po = boost::program_options;

const char* crc32_ha = "crc32";
const char* md5_ha = "md5";

Parameters OptionsReader::read_arguments(int argc, const char* argv[])
{
    po::options_description desc("File duplicates search utility. Parameters");
    
    desc.add_options()
        ("help,h", "Outputs help information")
        ("scan_dirs,s",
            po::value<std::vector<string>>()->default_value(std::vector<std::string>{"."}, ".")->multitoken()->composing(),
            "Paths to be scanned for file duplicates, can be multiple. Current folder by default.")
        ("exclude_dirs,e",
            po::value<vector<string>>()->multitoken()->composing(),
            "Paths to be excluded from scanning, can be multiple.")
        ("depth,d",
            po::value<unsigned short>()->default_value(0)->notifier([](const unsigned short& value)
                {
                    auto possible_values = {
                        static_cast<unsigned short>(0),
                        static_cast<unsigned short>(1)
                    };
                    
                    check_value_is_correct(value, possible_values, "depth");
                }),
            "Scan depth, 1 - all directories, 0 - current folder only. Default value is 0.")
        ("min_size,m",
            po::value<int>()->default_value(1)->notifier([](const int& value)
                {
                    check_value_is_not_negative(value, "min_size");
                }),
            "Minimum size of the file to be processed, in bytes. Default value is 1.")
        ("file_masks,f",
            po::value<vector<string>>()->multitoken()->composing(),
            "Masks for files participating in comparison, case-insensitive.")
        ("block_size,b",
            po::value<int>()->default_value(256)->notifier([](const int& value)
                {
                    check_value_is_not_negative(value, "block_size");
                }),
            "The size of the block to read the files with, in bytes. Default value is 256.")
        ("algorithm,a",
            po::value<string>()->default_value(crc32_ha)->notifier([](const string& value)
                {
                    check_value_is_correct(value, { std::string(crc32_ha), std::string(md5_ha) }, "algorithm");
                }),
            "Hash algorithm to hash file blocks. Default value is crc32.")
        ;

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    notify(vm);

    stringstream help;
    help << desc << endl;

    Parameters parameters;

    parameters.help_text = help.str();
    parameters.show_help = vm.count("help");

    if (parameters.show_help) {
        return parameters;
    }

    // директории для сканирования
    parameters.scan_dirs = vm["scan_dirs"].as<vector<string>>();
       
    // директории для исключения из сканирования
    if (vm.count("exclude_dirs")) {
        parameters.exclude_dirs = vm["exclude_dirs"].as<vector<string>>();
    }

    // уровень сканирования. 1 - все директории, 0 - только указанная директория без вложенных.
    parameters.scan_all_dirs = (vm["depth"].as<unsigned short>() == 1);

    // минимальный размер файла
    parameters.min_file_size = static_cast<unsigned int>(vm["min_size"].as<int>());

    // маски имен файлов разрешенных для сравнения
    if (vm.count("file_masks")) {
        parameters.file_masks = vm["file_masks"].as<vector<string>>();
    }

    // размер блока, которым производится чтения файлов
    parameters.block_size = static_cast<unsigned int>(vm["block_size"].as<int>());

    // один из имеющихся алгоритмов хэширования: crc32, md5
    // По умолчанию crc32
    if (vm["algorithm"].as<string>() == std::string(md5_ha)) {
        parameters.hash_algorithm = HashAlgoritm::MD5;
    }

    return parameters;
}

template <typename T>
void OptionsReader::check_value_is_correct(const T& value, const std::initializer_list<T>& possible_values, std::string option_name)
{
    for (const auto& supported : possible_values) {
        if (value == supported) {
            return;
        }
    }

    throw po::validation_error(po::validation_error::invalid_option_value, option_name);
}

template <typename T>
void OptionsReader::check_value_is_not_negative(const T& value, std::string option_name)
{
    if (value < 0) {
        throw po::validation_error(po::validation_error::invalid_option_value, option_name);
    }
}

// https://stackoverflow.com/questions/3897839/how-to-link-c-program-with-boost-using-cmake
