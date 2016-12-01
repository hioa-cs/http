// This file is a part of the IncludeOS unikernel - www.includeos.org
//
// Copyright 2015-2016 Oslo and Akershus University College of Applied Sciences
// and Alfred Bratterud
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <catch.hpp>
#include <request.hpp>

#define CRLF "\r\n"

using namespace std;

///////////////////////////////////////////////////////////////////////////////
TEST_CASE("Default constructor only creates request line", "[Request]") {
  const http:: Request request;
  const std::string test_string {"GET / HTTP/1.1" CRLF};
  REQUIRE(test_string == request.to_string());
}

///////////////////////////////////////////////////////////////////////////////
TEST_CASE("Constructor to parse a character stream", "[Request]") {
 const std::string test_string {
    "GET https://github.com/hioa-cs/IncludeOS HTTP/1.1" CRLF
    "Connection: close" CRLF CRLF
  };

  http::Request request {std::move(test_string)};

  REQUIRE(http::GET                              == request.method());
  REQUIRE(true                                   == request.header().has_field(http::header_fields::connection));
  REQUIRE("close"                                == request.header().value(http::header_fields::connection));
  REQUIRE((http::Version{1, 1}                   == request.version()));
  REQUIRE("https://github.com/hioa-cs/IncludeOS" == request.uri().to_string());
}

///////////////////////////////////////////////////////////////////////////////
TEST_CASE("Building a request", "[Request]") {
  http::Request request;

  request.header().add_field(http::header_fields::host, "includeos.server:8080");
  request.header().add_field(http::header_fields::accept, "text/html");
  request.header().add_field(http::header_fields::connection, "close");

  const std::string test_string {
    "GET / HTTP/1.1" CRLF
    "Host: includeos.server:8080" CRLF
    "Accept: text/html" CRLF
    "Connection: close" CRLF CRLF
  };

  REQUIRE(test_string == request.to_string());
}

///////////////////////////////////////////////////////////////////////////////
TEST_CASE("Handling query data", "[Request]") {
  const std::string test_string {
    "GET includeos.net/q?file=install.sh&machine=x86_64 HTTP/1.1" CRLF
    "Host: includeos.server:8080" CRLF
    "Connection: close" CRLF CRLF
  };

  http::Request request {std::move(test_string)};

  REQUIRE("x86_64"     == request.query_value("machine"));
  REQUIRE("install.sh" == request.query_value("file"));
}

///////////////////////////////////////////////////////////////////////////////
TEST_CASE("Handling post data", "[Request]") {
  const std::string test_string {
    "POST / HTTP/1.1" CRLF
    "Host: includeos.server:8080" CRLF
    "Connection: close" CRLF CRLF
    "name=rico&language=cpp&project=includeos"
  };

  http::Request request {std::move(test_string)};

  REQUIRE("cpp"       == request.post_value("language"));
  REQUIRE("rico"      == request.post_value("name"));
  REQUIRE("includeos" == request.post_value("project"));
}
