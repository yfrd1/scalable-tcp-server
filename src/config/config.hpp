// config.hpp
#pragma once

#include <string>
#include <boost/json.hpp>

namespace scalable {
namespace server {
    
class config {
public:
    explicit config(const std::string& file_path);

    // Load JSON file into memory
    bool load();

    // Reload config from file
    bool reload();

    // Get raw JSON value
    boost::json::value get(const std::string& key) const;

    // Typed helpers
    std::string get_string(const std::string& key, const std::string& default_value = "") const;
    int get_int(const std::string& key, int default_value = 0) const;
    bool get_bool(const std::string& key, bool default_value = false) const;

    // Check key existence
    bool has(const std::string& key) const;

private:
    std::string file_path_;
    boost::json::object root_;

    boost::json::value find_key(const std::string& key) const;
};

}
}