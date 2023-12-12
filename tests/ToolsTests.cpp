#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "../include/Tools.hpp"

using namespace Tools;

TEST_CASE("URLs with hexadecimal encoded characters are decoded correctly")
{
  std::string url = "https://www.example.com/path%20with%20spaces?key=value%20with%20spaces";
  std::string expectedOutput = "https://www.example.com/path with spaces?key=value with spaces";

  std::string decodedUrl = convertHexToAscii(url);
  CHECK(decodedUrl == expectedOutput);
}

TEST_CASE("URLs without hexadecimal encoded characters are not modified")
{
  std::string url = "https://www.example.com/path with spaces?key=value with spaces";

  std::string decodedUrl = convertHexToAscii(url);
  CHECK(decodedUrl == url);
}

TEST_CASE("Empty URLs are not modified")
{
  std::string url;
  std::string expectedOutput;

  std::string decodedUrl = convertHexToAscii(url);
  CHECK(decodedUrl == expectedOutput);
}