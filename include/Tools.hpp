#pragma once

#include <string>

const std::string WHITESPACE = " \r\n\t\v\b\f";

namespace Tools
{

    void stripMarginWhitespace(std::string &str)
    {
        const auto left = str.find_first_not_of(WHITESPACE);
        const auto right = str.find_last_not_of(WHITESPACE);

        if (left == std::string::npos)
        {
            str = "";
        }
        else
        {
            str = str.substr(left, (right - left) + 1);
        }
    }

    std::string stringToLower(const std::string &str)
    {
        std::string temp;
        for (const auto &character : str)
        {
            temp += std::tolower(character);
        }
        return temp;
    }

    std::string convertHexToAscii(const std::string &str)
    {
        std::string decodedString;
        decodedString.reserve(str.size()); // Preallocate enough space to avoid reallocations

        for (std::size_t i = 0; i < str.size(); ++i)
        {
            if (str[i] == '%')
            {
                std::string hexStr = str.substr(i + 1, 2);
                int hexValue = std::stoi(hexStr, nullptr, 16);
                decodedString += static_cast<char>(hexValue);
                i += 2;
            }
            else
            {
                decodedString += str[i];
            }
        }

        return decodedString;
    }
}
