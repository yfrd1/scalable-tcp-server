#pragma once

#include <cstdint>
#include <string_view>
#include <vector>
#include <map>

#include <boost/json.hpp>

#include "common/locales/language.hpp"
#include "common/locales/error_codes.hpp"

namespace scalable::common::locales {


class LanguageProvider
{
public:
    LanguageProvider() = default;

    void load_language_files();
    void load_language_file(Language language);
    bool check_language(Language language) const;
    std::string_view language_code_to_text(Language language) const;
    Language language_text_to_code(std::string_view language) const;
    std::string_view error_code_to_text(ErrorCodes code, Language lang);

private:
    void save_errors(const std::string& content, const std::string& lang);
    void load_file_content(const std::string& lang);

    std::map<Language, std::map<ErrorCodes, std::string>> error_messages_;
};


}