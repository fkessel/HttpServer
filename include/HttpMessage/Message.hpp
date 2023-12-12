#pragma once

#include <map>
#include <string>
#include <vector>

#include "Body.hpp"
#include "HeaderFieldName.hpp"
#include "Version.hpp"

namespace http
{

    class HeaderException : std::exception
    {
    public:
        HeaderException(const std::string &reason) : std::exception(), reason_(reason) {}
        virtual const char *what() const noexcept override { return reason_.c_str(); }

    private:
        std::string reason_;
    };

    class Message
    {

    private:
        typedef std::map<HeaderFieldName, std::string> Header;
        Header allHeader_;

    protected:
        Version version_;
        std::string body_;

    public:
        Message() = default;
        virtual ~Message() = default;

        std::string getHeader(const std::string &key) const { return allHeader_.at(key); }

        Header getAllHeader() { return allHeader_; }

        size_t getHeaderCount() const { return allHeader_.size(); }

        std::string getBody() const { return body_; }

        size_t getBodyLength() const { return body_.length(); }

        std::string getVersion() const { return version_.getVersion(); }

        void setVersion(const std::string &version) { version_.setVersion(version); }

        void setHeader(const std::string &name, const std::string &value)
        {
            allHeader_[HeaderFieldName(name)] = value;
        }

        void addToBody(const std::string &line)
        {
            body_.append(line);
        }

    protected:
        void parseHeaderNameAndValue(const std::string &rawHeader)
        {
            std::vector<std::string> rawHeaderLines = splitHeaderInLines(rawHeader);

            for (size_t index = 0; index < rawHeaderLines.size(); ++index)
            {
                std::string line = rawHeaderLines[index];
                const auto position = line.find_first_of(":");
                if (position == std::string::npos)
                {
                    continue;
                }

                std::string key = line.substr(0, position);
                std::string value = line.substr(line.find_first_not_of(" ", position + 1));
                allHeader_[HeaderFieldName(key)] = value;
            }
        }

    private:
        std::vector<std::string> splitHeaderInLines(const std::string &rawHeader)
        {
            std::vector<std::string> rawHeaderLines;
            std::istringstream input{rawHeader};
            if (!input)
            {
                throw HeaderException(std::string("Header could not be split into lines of strings."));
            }

            for (std::string line; std::getline(input, line);)
            {
                if (line.ends_with('\r'))
                {
                    line.pop_back();
                }

                if (!line.empty())
                {
                    rawHeaderLines.push_back(line);
                }
                else
                {
                    rawHeaderLines.push_back("");
                }
            }

            return rawHeaderLines;
        }
    };
}