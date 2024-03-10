#include "options_reader.h"

#include <boost/program_options.hpp>
#include <boost/any.hpp>

#include <sstream>

using namespace std;
namespace po = boost::program_options;

// https://stackoverflow.com/questions/3897839/how-to-link-c-program-with-boost-using-cmake

Parameters OptionsReader::read_arguments(int argc, const char* argv[])
{
    po::options_description desc("File duplicates search utility. Parameters");
    
    desc.add_options()
        ("help,h", "Outputs help information")
        ("scan_dirs,s",
            po::value<std::vector<string>>()->default_value(std::vector<std::string>{"."}, ".")->multitoken()->composing(),
            "Directories to be scanned, can be multiple. Default is current folder.")
        ("exclude_dirs,e",
            po::value<vector<string>>()->multitoken()->composing(),
            "Directories to be excluded, can be multiple.")
        ("depth,d",
            po::value<unsigned short>()->default_value(0)->notifier([](const unsigned short& value)
                {
                    auto possible_values = {
                        static_cast<unsigned short>(0),
                        static_cast<unsigned short>(1)
                    };
                    
                    check_value_is_correct(value, possible_values, "depth");
                    //throw po::validation_error(po::validation_error::invalid_option_value, "depth");
                }),
            "Scan depth, 1 - all directories, 0 - current folder only. Default value is 0.")
        ("min_size,m",
            po::value<int>()->default_value(1)->notifier([](const int& value)
                {
                    check_value_is_not_negative(value, "min_size");
                    //throw po::validation_error(po::validation_error::invalid_option_value, "option_name");
                }),
            "Minimum size of the file to be processed, in bytes. Default value is 1.")
        ("file_masks,f",
            po::value<vector<string>>()->multitoken()->composing(),
            "Masks for files participating in comparison, case-insensitive.")
        ("block_size,b",
            po::value<int>()->default_value(256)->notifier([](const int& value)
                {
                    check_value_is_not_negative(value, "block_size");
                    //throw po::validation_error(po::validation_error::invalid_option_value, "option_name");
                }),
            "The size of the block to read the files with, in bytes. Default value is 256.")
        ("algorithm,a",
            po::value<string>()->default_value("crc32")->notifier([](const string& value)
                {
                    check_value_is_correct(value, { std::string("crc32"), std::string("md5") }, "algorithm");
                
                    //throw po::validation_error(po::validation_error::invalid_option_value, "option_name");
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

    parameters.scan_dirs = vm["scan_dirs"].as<vector<string>>();

	

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
