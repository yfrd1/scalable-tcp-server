#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <sstream>
#include <filesystem>
#include "logger.hpp"
#include "config/config.hpp"

namespace scalable {
namespace server {

    logger::logger(config& config_)
    {
        configLevel=getLevel(config_.get_string("logging.level"));
        configFile=config_.get_string("logging.file");
        configIntervalSeconds=config_.get_int("logging.flush_interval_seconds");
    }

    logger::LogLevel logger::getLevel(const std::string& levelStr)
    {
        if(levelStr=="Debug") return LogLevel::Debug;
        else if(levelStr=="Info") return LogLevel::Info;
        else if(levelStr=="Notice") return LogLevel::Notice;
        else if(levelStr=="Warning") return LogLevel::Warning;
        else if(levelStr=="Error") return LogLevel::Error;
        else if(levelStr=="Critical") return LogLevel::Critical;
        else if(levelStr=="Alert") return LogLevel::Alert;
        else if(levelStr=="Emergency") return LogLevel::Emergency;
        else throw std::invalid_argument("Invalid log level");
    }
    
    const char* logger::levelToString(LogLevel level)
    {
        if(level==LogLevel::Debug) return "Debug";
        else if(level==LogLevel::Info) return "Info";
        else if(level==LogLevel::Notice) return "Notice";
        else if(level==LogLevel::Warning) return "Warning";
        else if(level==LogLevel::Error) return "Error";
        else if(level==LogLevel::Critical) return "Critical";
        else if(level==LogLevel::Alert) return "Alert";
        else if(level==LogLevel::Emergency) return "Emergency";
        throw std::invalid_argument("Invalid log level");
    }
    
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

    bool logger::getFileFolders()
    {
        std::filesystem::path path(configFile);
        std::string extension = path.extension().string();
        if(!path.has_extension())
        {
            return false;
        }
        
        if(extension!=".log" && extension!=".txt")
        {
            return false;
        }
        
        std::string parent = path.parent_path().string();
        if(!parent.empty())
        {
            std::error_code ec;
            std::filesystem::create_directories(parent, ec);
            if(ec)
            {
                return false;
            }
        }
        
        if(!parent.empty())
        {
            pathinfo.path = parent;
            pathinfo.path.push_back(std::filesystem::path::preferred_separator);
        }
        else
        {
            pathinfo.path="";
        }

        pathinfo.name=path.stem().string();
        pathinfo.extention=extension;

        return true;
    }

    void logger::openFileIfNeeded() {


        std::string today = getDate();
        if (today != currentDate) {
            currentDate = today;

            if (file.is_open())
                file.close();

            std::string filename = pathinfo.path+pathinfo.name+"-" + currentDate + pathinfo.extention;
            file.open(filename, std::ios::app);
        }
    }

    void logger::log(LogLevel level,
             const std::string& module,
             const std::string& message) {

        if(level<configLevel)
        {
            return;
        }

        openFileIfNeeded();

        file << getDateTime()
             << " [" << levelToString(level) << "] "
             << "[" << module << "] "
             << message << std::endl;
    }

}
}

