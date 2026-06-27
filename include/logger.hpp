#ifndef SCALABLE_SERVER_LOG_HPP
#define SCALABLE_SERVER_LOG_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <sstream>

namespace scalable {
namespace server {

class logger
{
public:
    void log(const std::string& level,
             const std::string& module,
             const std::string& message);

private:
    std::string getDate();    
    std::string getDateTime();
    void openFileIfNeeded();

    std::ofstream file;
    std::string currentDate;
};

}
}

#endif