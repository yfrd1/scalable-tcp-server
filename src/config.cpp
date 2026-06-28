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
 
    json::value config::get(const std::string& key) const
    {
        auto value = find_key(key);
        if(value.is_null())
        {
            throw "Config key not found: " + key;
        }
        return value;
    }

    std::string config::get_string(
        const std::string& key, 
        const std::string& default_value) const
    {
        auto value = find_key(key);

        if(value.is_null() || !value.is_string())
        {
            return default_value;
        }

        return std::string(value.as_string());
    }

    int config::get_int(
        const std::string& key, 
        int default_value) const
    {
        auto value = find_key(key);
        if(value.is_null())
            return default_value;

        if(value.is_int64() || value.is_uint64())
        {
            return static_cast<int>(value.as_int64());
        }

        return default_value;
    }

    bool config::get_bool(
        const std::string& key, 
        bool default_value) const
    {
        auto value = find_key(key);

        if(value.is_null() || !value.is_bool())
        {
            return static_cast<bool>(value.as_bool());
        }

        return default_value;
    }



    bool config::has(const std::string& key) const
    {
        return !find_key(key).is_null();
    }

    json::value config::find_key(const std::string& key) const
    {
        size_t index_of_dot = key.find('.');

        // Key without child
        if(index_of_dot==std::string::npos)
        {
            auto it=root_.find(key);
            return (it!=root_.end())?it->value():json::value(nullptr);
        }

        std::string parenfKey = key.substr(0, index_of_dot);
        std::string childKey = key.substr(index_of_dot+1);

        auto parent_it = root_.find(parenfKey);
        if(parent_it==root_.end() || !parent_it->value().is_object())
        {
            return json::value(nullptr);
        }

        const json::object& child_obj = parent_it->value().as_object();

        auto child=child_obj.find(childKey);
        return (child!=child_obj.end())?child->value():json::value(nullptr);
    }
}
}