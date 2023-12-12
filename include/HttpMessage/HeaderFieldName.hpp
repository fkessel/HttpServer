#pragma once

#include <string>

#include "../Tools.hpp"

namespace http {

    class HeaderFieldName {

    private:
        std::string fieldName_;

    public:
        HeaderFieldName(const std::string& name) : fieldName_(name) {}
        std::string getFieldName() const { return fieldName_; }
        friend bool operator < (const HeaderFieldName& lhs, const HeaderFieldName& rhs);

    };

    bool operator < (const HeaderFieldName& lhs, const HeaderFieldName& rhs)
    {
        std::string lhs_temp = Tools::stringToLower(lhs.getFieldName());
        std::string rhs_temp = Tools::stringToLower(rhs.getFieldName());

        return lhs_temp < rhs_temp;
    }
}
