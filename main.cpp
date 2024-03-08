#include <boost/filesystem/operations.hpp>
#include <boost/asio.hpp>
#include <boost/core/noncopyable.hpp>

#include <iostream>

#ifdef WIN32
#define appname "bayan.exe"
#else
#define appname "./bayan"
#endif

namespace ba = boost::asio;

int main(int args_number, char const** args)
{
   
    if (args_number != 2) {
        std::cerr << "The number of commands hasn't been passed!" << std::endl;
        return 1;
    }

    int commands_number = std::atoi(args[1]);

    if (commands_number == 0) {
        std::cerr << "The number of commands must be more then 0!" << std::endl;
        return 1;
    }
      
        
    return 0;
}