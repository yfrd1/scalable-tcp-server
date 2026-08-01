#pragma once

#include "config/config.hpp"

#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <sstream>
#include <vector>

namespace scalable {
namespace server {

class Logger
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


    explicit Logger(Config& config_);

    bool getFileFolders();

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


    struct logPathInfo
    {
        std::string path;
        std::string name;
        std::string extention;
    } pathinfo;
};

}
}
