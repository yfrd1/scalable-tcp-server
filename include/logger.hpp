#ifndef SCALABLE_SERVER_LOG_HPP
#define SCALABLE_SERVER_LOG_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <sstream>
#include "config.hpp"

namespace scalable {
namespace server {

class logger
{
public:
    enum class LogLevel
    {
        Debug,
        Info,
        Notice,
        Warning,
        Error,
        Critical,
        Alert,
        Emergency,
    };

    explicit logger(config& config_);

    void log(LogLevel level,
             const std::string& module,
             const std::string& message);

private:
    std::string getDate();    
    std::string getDateTime();
    void openFileIfNeeded();
    LogLevel getLevel(const std::string& level);
    const char* levelToString(LogLevel level);

    std::ofstream file;
    std::string currentDate;

    LogLevel configLevel;
    std::string configFile;
    int configIntervalSeconds;
};

}
}

#endif