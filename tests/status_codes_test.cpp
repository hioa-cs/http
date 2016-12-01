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
#include <status_codes.hpp>

///////////////////////////////////////////////////////////////////////////////
TEST_CASE("Valid code", "[Status Codes]") {
  const std::string test_string {"Network Authentication Required"};
  REQUIRE(test_string == http::code_description(http::Network_Authentication_Required));
}

///////////////////////////////////////////////////////////////////////////////
TEST_CASE("Invalid code", "[Status Codes]") {
  const std::string test_string {"Internal Server Error"};
  REQUIRE(test_string == http::code_description(static_cast<http::status_t>(-200)));
}

///////////////////////////////////////////////////////////////////////////////
TEST_CASE("Information codes", "[Status Codes]") {
  REQUIRE(true == http::is_informational(http::Continue));
  REQUIRE(false == http::is_informational(http::OK));
}

///////////////////////////////////////////////////////////////////////////////
TEST_CASE("Success codes", "[Status Codes]") {
  REQUIRE(true == http::is_success(http::OK));
  REQUIRE(false == http::is_success(http::Continue));
}

///////////////////////////////////////////////////////////////////////////////
TEST_CASE("Redirection codes", "[Status Codes]") {
  REQUIRE(true == http::is_redirection(http::Temporary_Redirect));
  REQUIRE(false == http::is_redirection(http::Reset_Content));
}

///////////////////////////////////////////////////////////////////////////////
TEST_CASE("Client codes", "[Status Codes]") {
  REQUIRE(true == http::is_client_error(http::Not_Acceptable));
  REQUIRE(false == http::is_client_error(http::Gateway_Timeout));
}

///////////////////////////////////////////////////////////////////////////////
TEST_CASE("Server codes", "[Status Codes]") {
  REQUIRE(true == http::is_server_error(http::Not_Implemented));
  REQUIRE(false == http::is_server_error(http::Use_Proxy));
}
