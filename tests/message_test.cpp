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
#include <message.hpp>

///////////////////////////////////////////////////////////////////////////////
TEST_CASE("Default constructed message is blank", "[Message]") {
  const http::Message message;
  REQUIRE("" == message.to_string());
}

///////////////////////////////////////////////////////////////////////////////
TEST_CASE("Get header section", "[Message]") {
  http::Message message;
  auto& header = message.header();
  header.add_field(http::header_fields::server, "IncludeOS/Acorn v0.1");
  header.add_field(http::header_fields::allow, "GET, HEAD");
  header.add_field(http::header_fields::connection, "close");

  const std::experimental::string_view test_string {
    "Server: IncludeOS/Acorn v0.1\r\n"
    "Allow: GET, HEAD\r\n"
    "Connection: close\r\n\r\n"
  };
                                  ;
  REQUIRE(test_string == message.to_string());
}

///////////////////////////////////////////////////////////////////////////////
TEST_CASE("Add body", "[Message]") {
  http::Message message;
  message.add_body("[IncludeOS] A minimal, resource efficient unikernel for cloud services");

  const std::experimental::string_view test_string {
    "Content-Length: 70\r\n\r\n"
    "[IncludeOS] A minimal, resource efficient unikernel for cloud services"
  };

  REQUIRE(test_string == message.to_string());
}

///////////////////////////////////////////////////////////////////////////////
TEST_CASE("Append to body", "[Message]") {
  http::Message message;
  message.add_body("[IncludeOS] A minimal, resource efficient unikernel for cloud services")
         .append_body(" http://www.includeos.org");

  const std::experimental::string_view test_string {
    "Content-Length: 95\r\n\r\n"
    "[IncludeOS] A minimal, resource efficient unikernel for cloud services http://www.includeos.org"
  };

  REQUIRE(test_string == message.to_string());
}
