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
#include <status_line.hpp>

///////////////////////////////////////////////////////////////////////////////
TEST_CASE("Default constructor", "[Status_line]") {
  http::Status_line status_line;
  const std::experimental::string_view test_string {"HTTP/1.1 200 OK\r\n"};
  REQUIRE(test_string == status_line.to_string());
}

///////////////////////////////////////////////////////////////////////////////
TEST_CASE("Constructor specifying version and code", "[Status_line]") {
  http::Status_line status_line {http::Version{2U, 0U}, http::Moved_Permanently};
  const std::experimental::string_view test_string {"HTTP/2.0 301 Moved Permanently\r\n"};
  REQUIRE(test_string == status_line.to_string());
}

///////////////////////////////////////////////////////////////////////////////
TEST_CASE("Constructor taking a character stream", "[Status_line]") {
  const std::experimental::string_view test_string {
    "HTTP/2.0 301 Moved Permanently\r\n"
    "Server: IncludeOS/Acorn v0.1\r\n"
    "Allow: GET, HEAD\r\n"
    "Location: /public/doc/unikernels.pdf\r\n\r\n"
  };

  http::Status_line status_line {test_string};

  const std::experimental::string_view result {"HTTP/2.0 301 Moved Permanently\r\n"};
  REQUIRE(result == status_line.to_string());
}

///////////////////////////////////////////////////////////////////////////////
TEST_CASE("Nonesense character stream throws", "[Status_line]") {
  const std::experimental::string_view test_string {
    "[IncludeOS] A minimal, resource efficient unikernel for cloud services"
  };

  REQUIRE_THROWS_AS(http::Status_line{test_string}, http::Status_line_error);
}

///////////////////////////////////////////////////////////////////////////////
TEST_CASE("Empty character stream throws", "[Status_line]") {
  REQUIRE_THROWS_AS(http::Status_line{""}, http::Status_line_error);
}

///////////////////////////////////////////////////////////////////////////////
TEST_CASE("Invalid status line throws [Missing code description]", "[Status_line]") {
  const std::experimental::string_view test_string {"HTTP/2.0 301\n"};
  REQUIRE_THROWS_AS(http::Status_line{test_string}, http::Status_line_error);
}

///////////////////////////////////////////////////////////////////////////////
TEST_CASE("Invalid line ending throws", "[Status_line]") {
  const std::experimental::string_view test_string {"HTTP/2.0 301 Moved Permanently\r"};
  REQUIRE_THROWS_AS(http::Status_line{test_string}, http::Status_line_error);
}

///////////////////////////////////////////////////////////////////////////////
TEST_CASE("Set version", "[Status_line]") {
  http::Status_line status_line {http::Version{}, http::OK};
  status_line.set_version(http::Version{2U, 0U});
  const std::experimental::string_view test_string {"HTTP/2.0 200 OK\r\n"};
  REQUIRE(test_string == status_line.to_string());
}

///////////////////////////////////////////////////////////////////////////////
TEST_CASE("Get version", "[Status_line]") {
  http::Status_line status_line {http::Version{1U, 1U}, http::OK};
  REQUIRE((http::Version{1U, 1U} == status_line.version()));
}

///////////////////////////////////////////////////////////////////////////////
TEST_CASE("Set code", "[Status_line]") {
  http::Status_line status_line {http::Version{}, http::OK};
  status_line.set_code(http::Processing);
  const std::experimental::string_view test_string {"HTTP/1.1 102 Processing\r\n"};
  REQUIRE(test_string == status_line.to_string());
}

///////////////////////////////////////////////////////////////////////////////
TEST_CASE("Get code", "[Status_line]") {
  http::Status_line status_line {http::Version{1U, 1U}, http::Processing};
  REQUIRE(http::Processing == status_line.code());
}
