#include <boost/asio.hpp>
#include <string>
#include <iostream>
#include <memory>
#include "application/application.hpp"

using scalable::server::Application;

int main(int argc, char* argv[])
{

    try
    {
        Application app();
        app.run();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
        
    return 0;
}