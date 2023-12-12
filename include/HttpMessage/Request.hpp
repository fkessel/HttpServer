#pragma once

#include <exception>
#include <ios>
#include <map>
#include <string>
#include <sstream>

#include "Message.hpp"
#include "RequestLine.hpp"
#include "../Tools.hpp"

namespace http {

    class RequestException : public std::exception {
    public:
        RequestException(const std::string& reason) : std::exception(), reason_(reason) {}
        virtual const char* what() const noexcept override { return reason_.c_str(); }
    private:
        std::string reason_;
    };

    class Request : public Message {

        RequestLine reqLine_;

    public:

        Request(const std::string& rawRequest)
        {
            parseRequest(rawRequest);
        }

        Method getMethod() const { return reqLine_.getMethod(); }

        Target getTarget() const { return reqLine_.getTarget(); }

    private:
        /*
         * This method parses the request line as individual strings 
         * in the main components: startline, header and body.
         */

        void parseRequest(const std::string& rawRequest)
        {
            enum class ParsingMessage {
                FirstLine,
                Headers,
                PossibleEndOfHeaders,
                PossibleStartOfBody,
                Body,
            };

            std::istringstream input{ rawRequest };
            char next;
            std::string firstLine;
            std::string allHeader;
            ParsingMessage parsingState{ ParsingMessage::FirstLine };

            while (input.get(next))
            {
                switch (parsingState)
                {
                case(ParsingMessage::FirstLine):
                    if (next != '\n') { firstLine += next; }
                    else { parsingState = ParsingMessage::PossibleStartOfBody; }
                    break;
                case(ParsingMessage::Headers):
                    allHeader += next;
                    if (next == '\n')
                    {
                        parsingState = ParsingMessage::PossibleEndOfHeaders;
                    }
                    break;
                case(ParsingMessage::PossibleEndOfHeaders):
                    if ((next == '\n') || (next == '\r'))
                    {
                        parsingState = ParsingMessage::PossibleStartOfBody;
                    }
                    else
                    {
                        allHeader += next;
                        parsingState = ParsingMessage::Headers;
                    }
                    break;
                case(ParsingMessage::PossibleStartOfBody):
                    if (next == '\n' || (next == '\r'))
                    {
                        parsingState = ParsingMessage::Body;
                    }
                    else if (isalnum(next))
                    {
                        allHeader += next;
                        parsingState = ParsingMessage::Headers;
                    }
                    break;
                case(ParsingMessage::Body):
                    Message::body_ += next;
                    break;
                }
            }
            
            parseFirstLine(firstLine);
            Message::parseHeaderNameAndValue(allHeader);
            
            if (Message::getAllHeader().empty())
            {
                Message::body_ = "";
            }
            else
            {
               Tools::stripMarginWhitespace(Message::body_);
            }
        }


        /*
         * This method parses method, target and version from 
         * the first line of the request.
         */

        void parseFirstLine(std::string& line)
        {
            Tools::stripMarginWhitespace(line);

            int state = 0;
            std::string tempMethod;
            std::string tempVersion;
            std::string tempTarget;
            for (const auto& character : line)
            {
                switch (state)
                {
                case(0):
                    if (!isspace(character)) { tempMethod += character; }
                    else { state = 1; }
                    break;
                case(1):
                    if (!isspace(character)) { tempTarget += character; }
                    else { state = 2; }
                    break;
                case(2):
                    if (character != std::ios_base::end) { tempVersion += character; }
                    else { break; }
                }
            }
            
            reqLine_.setMethod(tempMethod);
            reqLine_.setTarget(tempTarget);
            Message::setVersion(tempVersion);
        }
    };
}