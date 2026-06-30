#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <sstream>
#include <filesystem>
#include "logger.hpp"
#include "config.hpp"

namespace scalable {
namespace server {

    logger::logger(config& conf): config_(conf){}
    
    std::string logger::getDate() {
        std::time_t t = std::time(nullptr);
        std::tm tm = *std::localtime(&t);

        std::ostringstream oss;
        oss << (tm.tm_year + 1900) << "-"
            << (tm.tm_mon + 1) << "-"
            << tm.tm_mday;
        return oss.str();
    }

    std::string logger::getDateTime() {
        std::time_t t = std::time(nullptr);
        std::tm tm = *std::localtime(&t);

        std::ostringstream oss;
        oss << (tm.tm_year + 1900) << "-"
            << (tm.tm_mon + 1) << "-"
            << tm.tm_mday << " "
            << (tm.tm_hour < 10 ? "0" : "") << tm.tm_hour << ":"
            << (tm.tm_min < 10 ? "0" : "") << tm.tm_min << ":"
            << (tm.tm_sec < 10 ? "0" : "") << tm.tm_sec;

        return oss.str();
    }

    void logger::openFileIfNeeded() {

        std::filesystem::create_directories("logs");

        std::string today = getDate();
        if (today != currentDate) {
            currentDate = today;

            if (file.is_open())
                file.close();

            std::string filename = "logs/server-" + currentDate + ".log";
            file.open(filename, std::ios::app);
        }
    }

    void logger::log(const std::string& level,
             const std::string& module,
             const std::string& message) {

        openFileIfNeeded();

        file << getDateTime()
             << " [" << level << "] "
             << "[" << module << "] "
             << message << std::endl;
    }

}
}

