#include "options_reader.h"

#include <boost/program_options.hpp>
#include <boost/program_options/options_description.hpp>
#include <boost/any.hpp>
#include <sstream>

using namespace std;
namespace po = boost::program_options;


Parameters OptionsReader::read_arguments(int argc, const char* argv[])
{
    po::options_description desc("Allowed options");
    desc.add_options()
        ("help", "produce help message")
        ("compression", po::value<int>(), "set compression level")
        ;

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);
    /*
    if (vm.count("help")) {
        cout << desc << "\n";
        return 1;
    }

    if (vm.count("compression")) {
        cout << "Compression level was set to "
            << vm["compression"].as<int>() << ".\n";
    }
    else {
        cout << "Compression level was not set.\n";
    }*/

    /*
    if (args_number != 2) {
        std::cerr << "The number of commands hasn't been passed!" << std::endl;
        return 1;
    }

    int commands_number = std::atoi(args[1]);

    if (commands_number == 0) {
        std::cerr << "The number of commands must be more then 0!" << std::endl;
        return 1;
    }*/

	Parameters parameters;

    return parameters;
}
