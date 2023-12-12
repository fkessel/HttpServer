#pragma once

#include <exception>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <map>
#include <sstream>
#include <string>

#include "HeaderFieldName.hpp"
#include "Message.hpp"
#include "ResponseLine.hpp"

namespace http
{

    namespace fs = std::filesystem;

    class ResponseException : public std::exception
    {
    public:
        ResponseException(const std::string &reason) : std::exception(), reason_(reason) {}
        virtual const char *what() const noexcept override { return reason_.c_str(); }

    private:
        std::string reason_;
    };

    class Response : public Message
    {

    private:
        ResponseLine respLine_;

    public:
        Response() = default;

        Response(const Request &request)
        {
            Message::setVersion("HTTP/1.1");
            const auto target = request.getTarget();
            loadFilesFromPath(target);
        }

        std::string generateResponse()
        {
            std::string responseLineString = respLine_.generateResponseLine(Message::getVersion());

            std::string response = ("HTTP/1.1 200 OK\r\n");
            const auto conType = Message::getHeader("Content-Type");
            response.append("Content-Type: " + conType + "\r\n");
            response.append("Content-Length: " + Message::getBodyLength());
            response.append("\r\n\r\n");
            response.append(Message::getBody());

            return response;
        }

    private:
        void determineContentType(const std::string &pathToFile)
        {
            fs::path path{pathToFile};
            std::ostringstream extension;
            if (fs::exists(path))
            {
                extension << path.extension();
            }
            else
            {
                return;
            }

            std::string buffer;
            for (const auto &c : extension.str())
            {
                if (c == '\"')
                {
                }
                else
                {
                    buffer += c;
                }
            }

            if (buffer == ".html")
            {
                Message::setHeader("Content-Type", "text/html");
            }
            else if (buffer == ".ico")
            {
                Message::setHeader("Content-Type", "image/x-icon");
            }
            else
            {
                Message::setHeader("Content-Type", "application/octet-stream");
            }
        }

        void loadFilesFromPath(const Target &target)
        {
            std::string tempPath = R"(C:\Users\NCEW\Desktop\HttpServer\public)";
            if (target == "/")
            {
                tempPath = R"(C:\Users\NCEW\Desktop\HttpServer\public\index.html)";
            }
            else
            {
                for (auto &c : target)
                {
                    if (c == '/')
                    {
                        tempPath += '\\';
                    }
                    else
                    {
                        tempPath += c;
                    }
                }
            }

            std::fstream fstr{tempPath, std::ios::out | std::ios::_Nocreate};
            std::string buffer;
            while (getline(fstr, buffer))
            {
                Message::addToBody(buffer);
            }

            determineContentType(tempPath);
        }
    };
}