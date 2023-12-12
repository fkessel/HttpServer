#pragma once

#include <string>

#include "../Tools.hpp"

namespace http {

    class Version {
    
    private:
        std::string version_;
    
    public:
        std::string getVersion() const { return version_; }
        void setVersion(const std::string& version) { this->version_ = version; }

        friend std::ostream& operator<< (std::ostream& os, const Version& version);
    };

    std::ostream& operator<< (std::ostream& os, const Version& version)
    {
        os << version.getVersion();
        return os;
    }
}