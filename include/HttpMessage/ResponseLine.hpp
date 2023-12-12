#pragma once

#include <iostream>

namespace http {

    class ResponseLine {

    private:
        unsigned int statusCode_ = 200;
        std::string textualRepresentationOfStatusCode_ = "OK";

    public:
        ResponseLine() = default;

        std::string generateResponseLine(const std::string& version)
        {
            std::ostringstream osstr;
            osstr << version     << " "
                  << statusCode_ << " "
                  << textualRepresentationOfStatusCode_;

            return osstr.str();
        }
    };
}