#include "options_reader.h"

#include <boost/any.hpp>
#include <boost/program_options.hpp>

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

    const Parameters parameters = opt_reader_ptr->read_arguments(args_number, args);
    
      
        
    return 0;
}