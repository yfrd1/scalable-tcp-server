#include <boost/asio.hpp>
#include <string>
#include <iostream>
#include "server.hpp"

int main(int argc, char* argv[])
{
    if(argc!=3)
    {
        std::cerr << "Usage: main <host> <port>\n";
        return 1;
    }

    try
    {
        boost::asio::io_context io;
        scalable::server::server server(io, argv[1], argv[2]);
        io.run();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
}