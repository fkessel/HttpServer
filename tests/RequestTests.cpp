#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "../include/HttpMessage/Request.hpp"
#include "../include/HttpMessage/Response.hpp"

#include <map>
#include <string>

using namespace http;

TEST_CASE("Parse a Correct Request String")
{
    std::string correctRequestString = (
        "GET /hello.txt HTTP/1.1\r\n"
        "User-Agent: curl/7.64.1\r\n"
        "Host: www.example.com\r\n"
        "Accept-Language: en, mi\r\n"
        "\r\n"
        "This is the body."
        );

    Request request(correctRequestString);
    
    SUBCASE("Parse Correct StartLine")
    {
        CHECK(request.getMethod() == Method::GET_M);
        CHECK(request.getTarget() == "/hello.txt");
        CHECK(request.getVersion() == "HTTP/1.1");
    }

    SUBCASE("Parse Correct Header")
    {
        std::map<std::string, std::string> expectedHeaders{
            {"User-Agent", "curl/7.64.1"},
            {"Host", "www.example.com"},
            {"Accept-Language", "en, mi"},
        };

        size_t count = request.getHeaderCount();

        CHECK(expectedHeaders.size() == count);
        for (const auto& header : expectedHeaders)
        {
            CHECK(header.second == request.getHeader(header.first));
        }
    }

    SUBCASE("Parse Body")
    {
        std::string expectedBody = "This is the body.";
        CHECK(expectedBody == request.getBody());
    }
}

TEST_CASE("Malformed Header In Request")
{
    std::string noHeaderInRequestString = (
        "GET /hello.txt HTTP/1.1\r\n\r\n"
        "This: is the body."
    );

    Request request(noHeaderInRequestString);
    const auto header = request.getAllHeader();
    
    SUBCASE("There are no headers detected")
    {
        CHECK(header.empty());
    }
    SUBCASE("Body is ignored, because no header fields were found")
    {
        CHECK(request.getBody().empty());
    }
}

TEST_CASE("There is a Specific Header in the Request")
{
    std::string requestString = (
        "GET /hello.txt HTTP/1.1\r\n"
        "User-Agent: curl/7.64.1\r\n"
        "Host: www.example.com\r\n"
        "Accept-Language: en, mi\r\n"
        "\r\n"
        );
    
    Request request(requestString);
    CHECK("curl/7.64.1" == request.getHeader("User-Agent"));

    SUBCASE("Case-Insensitive: Header Found")
    {
        CHECK("curl/7.64.1" == request.getHeader("USER-AGENT"));
        CHECK("curl/7.64.1" == request.getHeader("user-agent"));
        CHECK("curl/7.64.1" == request.getHeader("UsEr-AgEnT"));
    }
}

// TEST_CASE("Whitespace Before Start of Header")
// {
//     "GET / HTTP/1.1\r\n Host: host\r\n\r\n"
//                        ^ => Not allowed
//     "GET / HTTP/1.1\r\nHost: host\r\n\r\n"
// }

// TEST_CASE("Transfer-Encoding or Content-Length are part of header")
// {
//     // RFC 9112 Section 6: https://www.rfc-editor.org/rfc/rfc9112.html#name-message-body
    
// }
