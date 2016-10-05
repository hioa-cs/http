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
#include <header.hpp>

///////////////////////////////////////////////////////////////////////////////
TEST_CASE("Default constructor limits capacity to 25 fields", "[Header]") {
  http::Header header;
  REQUIRE(25U == header.limit());
}

///////////////////////////////////////////////////////////////////////////////
TEST_CASE("Constructor to specify field limit", "[Header]") {
  http::Header header {100U};
  REQUIRE(100U == header.limit());
}

///////////////////////////////////////////////////////////////////////////////
TEST_CASE("Default constructed object is empty", "[Header]") {
  http::Header header;
  REQUIRE(true == header.is_empty());
}

///////////////////////////////////////////////////////////////////////////////
TEST_CASE("Add a field", "[Header]") {
  http::Header header;
  header.add_field(http::header_fields::server, "IncludeOS/Acorn v0.1");
  REQUIRE(false == header.is_empty());
  REQUIRE(1U == header.size());
  REQUIRE(true == header.has_field(http::header_fields::server));
}

///////////////////////////////////////////////////////////////////////////////
TEST_CASE("Get field value", "[Header]") {
  http::Header header;
  header.add_field(http::header_fields::server, "IncludeOS/Acorn v0.1");
  REQUIRE(true == header.has_field(http::header_fields::server));
  REQUIRE("IncludeOS/Acorn v0.1" == header.value(http::header_fields::server));
}

///////////////////////////////////////////////////////////////////////////////
TEST_CASE("Set field value", "[Header]") {
  http::Header header;
  header.add_field(http::header_fields::server, "IncludeOS/Acorn v0.1");
  REQUIRE("IncludeOS/Acorn v0.1" == header.value(http::header_fields::server));
  header.set_field(http::header_fields::server, "IncludeOS/Acorn v2.0");
  REQUIRE("IncludeOS/Acorn v2.0" == header.value(http::header_fields::server));
}

///////////////////////////////////////////////////////////////////////////////
TEST_CASE("Erase field", "[Header]") {
  http::Header header;
  header.add_field(http::header_fields::server, "IncludeOS/Acorn v0.1");
  header.erase(http::header_fields::server);
  REQUIRE(false == header.has_field(http::header_fields::server));
  REQUIRE(true == header.is_empty());
}

///////////////////////////////////////////////////////////////////////////////
TEST_CASE("Cannot add fields beyond limit", "[Header]") {
  http::Header header {3};
  header.add_field(http::header_fields::server, "IncludeOS/Acorn v0.1");
  header.add_field(http::header_fields::allow, "GET, HEAD");
  header.add_field(http::header_fields::location, "/public/doc/unikernels.pdf");
  header.add_field(http::header_fields::connection, "close");

  const std::experimental::string_view test_string {
    "Server: IncludeOS/Acorn v0.1\r\n"
    "Allow: GET, HEAD\r\n"
    "Location: /public/doc/unikernels.pdf\r\n\r\n"
  };
                                  ;
  REQUIRE(3 == header.size());
  REQUIRE(test_string == header.to_string());
}

///////////////////////////////////////////////////////////////////////////////
TEST_CASE("Clear all fields", "[Header]") {
  http::Header header;
  header.add_field(http::header_fields::server, "IncludeOS/Acorn v0.1");
  header.add_field(http::header_fields::connection, "close");
  REQUIRE(2U == header.size());
  header.clear();
  REQUIRE(0U == header.size());
  REQUIRE(true == header.is_empty());
}

///////////////////////////////////////////////////////////////////////////////
TEST_CASE("Header to std::string conversion", "[Header]") {
  http::Header header;
  header.add_field(http::header_fields::server, "IncludeOS/Acorn v0.1");
  header.add_field(http::header_fields::allow, "GET, HEAD");
  header.add_field(http::header_fields::connection, "close");

  const std::experimental::string_view test_string {
    "Server: IncludeOS/Acorn v0.1\r\n"
    "Allow: GET, HEAD\r\n"
    "Connection: close\r\n\r\n"
  };
                                  ;
  REQUIRE(test_string == header.to_string());
}

///////////////////////////////////////////////////////////////////////////////
TEST_CASE("Add fields from a character stream", "[Header]") {
  const std::experimental::string_view test_string {
    "Server: IncludeOS/Acorn v0.1\r\n"
    "Allow: GET, HEAD\r\n"
    "Connection: close\r\n\r\n"
  };

  http::Header header {test_string};
  REQUIRE(test_string == header.to_string());
}

///////////////////////////////////////////////////////////////////////////////
TEST_CASE("Nonesense character stream gets rejected", "[Header]") {
  const std::experimental::string_view test_string {
    "[IncludeOS] A minimal, resource efficient unikernel for cloud services"
  };

  http::Header header {test_string};
  REQUIRE(true == header.is_empty());
}
