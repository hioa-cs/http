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
#include <request_line.hpp>

///////////////////////////////////////////////////////////////////////////////
TEST_CASE("Default constructor", "[Request_line]") {
  const http::Request_line request_line;
  const std::experimental::string_view test_string {"GET / HTTP/1.1\r\n"};
  REQUIRE(test_string == request_line.to_string());
}

///////////////////////////////////////////////////////////////////////////////
TEST_CASE("Constructor taking a character stream", "[Request_line]") {
  const std::experimental::string_view test_string {
    "GET / HTTP/1.1\r\n"
    "Host: 98.139.183.24\r\n"
    "Accept: text/html\r\n"
    "Connection: close\r\n\r\n"
  };

  const http::Request_line request_line {test_string};
  const std::experimental::string_view result {"GET / HTTP/1.1\r\n"};
  REQUIRE(result == request_line.to_string());
}

///////////////////////////////////////////////////////////////////////////////
TEST_CASE("Nonesense character stream throws", "[Request_line]") {
  const std::experimental::string_view test_string {
    "[IncludeOS] A minimal, resource efficient unikernel for cloud services"
  };

  REQUIRE_THROWS_AS(http::Request_line{test_string}, http::Request_line_error);
}

///////////////////////////////////////////////////////////////////////////////
TEST_CASE("Empty character stream throws", "[Request_line]") {
  REQUIRE_THROWS_AS(http::Request_line{""}, http::Request_line_error);
}

///////////////////////////////////////////////////////////////////////////////
TEST_CASE("Invalid status line throws [Missing HTTP version]", "[Request_line]") {
  const std::experimental::string_view test_string {"GET / \n"};
  REQUIRE_THROWS_AS(http::Request_line{test_string}, http::Request_line_error);
}

///////////////////////////////////////////////////////////////////////////////
TEST_CASE("Invalid line ending throws", "[Request_line]") {
  const std::experimental::string_view test_string {"GET / HTTP/1.1\r"};
  REQUIRE_THROWS_AS(http::Request_line{test_string}, http::Request_line_error);
}

///////////////////////////////////////////////////////////////////////////////
TEST_CASE("Get method", "[Request_line]") {
  const http::Request_line request_line;
  REQUIRE(http::GET == request_line.method());
}

///////////////////////////////////////////////////////////////////////////////
TEST_CASE("Set method", "[Request_line]") {
  http::Request_line request_line;
  request_line.set_method(http::POST);
  REQUIRE(http::POST == request_line.method());
}

///////////////////////////////////////////////////////////////////////////////
TEST_CASE("Get version", "[Request_line]") {
  const http::Request_line request_line;
  REQUIRE((http::Version{1U, 1U} == request_line.version()));
}

///////////////////////////////////////////////////////////////////////////////
TEST_CASE("Set version", "[Request_line]") {
  http::Request_line request_line;
  request_line.set_version(http::Version{2U, 0U});
  REQUIRE((http::Version{2U, 0U} == request_line.version()));
}

///////////////////////////////////////////////////////////////////////////////
TEST_CASE("Get uri", "[Request_line]") {
  const http::Request_line request_line;
  REQUIRE(uri::URI{"/"} == request_line.uri());
}

///////////////////////////////////////////////////////////////////////////////
TEST_CASE("Set uri", "[Request_line]") {
  http::Request_line request_line;
  const uri::URI uri {"http://includeos.org"};
  request_line.set_uri(uri);
  REQUIRE(uri == request_line.uri());
}
