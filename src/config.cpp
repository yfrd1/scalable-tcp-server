#include <string>
#include <fstream>
#include <boost/json.hpp>
#include "config.hpp"

namespace json = boost::json;

namespace scalable {
namespace server {

    config::config(const std::string& file_path) :
        file_path_(file_path) 
    { 
        load();
    }

    bool config::load()
    {
        std::ifstream file(file_path_, std::ios::in);
        if(!file.is_open())
            return false;

        std::string config;
        char buffer[512];
        while(file.read(buffer, sizeof(buffer)))
        {
            config.append(buffer, sizeof(buffer));
        }
        
        config.append(buffer, file.gcount());

        if(!file.eof() && file.fail())
        {
            return false;
        }

        // Parse JSON text
        boost::system::error_code ec;
        json::value json = json::parse(config, ec);

        if(ec || !json.is_object())
        {
            return false;
        }

        root_ = json.as_object();
        
        return true;
    }

    bool config::reload()
    {
        return load();
    }
  

}
}