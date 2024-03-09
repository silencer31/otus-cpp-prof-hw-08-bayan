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
        ("help,h", "produces help message")
        ("scan_dirs,s",
            po::value<std::vector<string>>()->default_value(std::vector<std::string>{"."}, ".")->multitoken()->composing(),
            "Directories to be scanned, can be multiple. Default is current folder.")
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
