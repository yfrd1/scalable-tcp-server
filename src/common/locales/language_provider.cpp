#include "common/locales/language_provider.hpp"

#include <cstdint>
#include <string_view>
#include <vector>
#include <map>
#include <string_view>
#include <filesystem>
#include <fstream>
#include <charconv>

#include <boost/json.hpp>

#include "common/locales/language.hpp"
#include "common/locales/error_codes.hpp"

namespace json = boost::json;

namespace scalable {
namespace common {


    void LanguageProvider::load_language_files()
    {
        for(auto iter = languages.begin(); iter!= languages.end(); ++iter)
        {
            std::string lang = iter->second;
            lang.append(".json");
            load_file_content(lang);
        }
    }

    void LanguageProvider::load_language_file(Language language)
    {
        std::string lang = std::string(language_code_to_text(language));
        lang.append(".json");
        load_file_content(lang);
    }

    void LanguageProvider::load_file_content(const std::string& lang)
    {
        std::filesystem::path path("../resource/lang/"+lang);
        if(!std::filesystem::exists(path))
            return;
        
        std::ifstream fi(path.string().c_str());
        if(!fi)
        {
            return;
        }

        std::string content;
        char buff[512];
        while(fi.read(buff, sizeof(buff)))
        {
            content.append(buff, sizeof(buff));
        }

        content.append(buff, fi.gcount());

        if(!fi.eof() && fi.fail())
        {
            return;
        }

        save_errors(content, lang);
    }

    bool LanguageProvider::check_language(Language language) const
    {
        return languages.contains(language);
    }

    std::string_view LanguageProvider::language_code_to_text(Language language) const
    {
        auto iter = languages.find(language);
        if(iter!=languages.end())
        {
            return iter->second;
        }
        return "en";
    }

    Language LanguageProvider::language_text_to_code(std::string_view language) const
    {
        for(auto iter = languages.begin(); iter!=languages.end(); ++iter)
        {
            if(iter->second ==language)
                return iter->first;
        }
        return Language::en;
    }

    std::string_view LanguageProvider::error_code_to_text(ErrorCodes code, Language lang)
    {
        auto it = error_messages_.find(lang);
        if(it != error_messages_.end())
        {
            auto const& errors = it->second;
            auto const& iter = errors.find(code);
            if(iter!=errors.end())
                return iter->second;
        }
        return "";
    }

    void LanguageProvider::save_errors(
        const std::string& content, const std::string& lang)
    {
        boost::system::error_code ec;
        json::value value = json::parse(content, ec);

        if(ec || !value.is_object())
            return;

        json::object obj = value.as_object();

        uint16_t code = 0;
        Language lang_enum = language_text_to_code(lang);
        
        std::map<ErrorCodes, std::string> map;
        for(auto iter = obj.begin(); iter!=obj.end(); ++iter)
        {
            auto key = iter->key();
            auto[ptr, errc] = std::from_chars(
                key.data(),
                key.data() + key.size(),
                code);
            
            if(errc == std::errc{})
            {
                map[static_cast<ErrorCodes>(code)] = 
                    std::string(iter->value().as_string());

            }
        }
        error_messages_[lang_enum] = std::move(map);
    }

}
}