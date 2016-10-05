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
#include <version.hpp>

///////////////////////////////////////////////////////////////////////////////
SCENARIO("Given a Version object") {
   http::Version version;

   WHEN("We set its major version number") {
    version.set_major(2U);

    THEN("It should be reflected") {
      REQUIRE(version.major() == 2U);
    }
   }

   WHEN("We set its minor version number") {
    version.set_minor(0U);

    THEN("It should be reflected") {
      REQUIRE(version.minor() == 0U);
    }
   }
}

///////////////////////////////////////////////////////////////////////////////
TEST_CASE("Convert a Version object to a std::string", "[Version]") {
  const http::Version version;
  const std::string test_string {"HTTP/1.1"};
  REQUIRE(test_string == version.to_string());
}

///////////////////////////////////////////////////////////////////////////////
TEST_CASE("Equality", "[Version]") {
  const http::Version a {2U, 0U};
  const http::Version b {a};
  REQUIRE(a == b);
}

///////////////////////////////////////////////////////////////////////////////
TEST_CASE("Inequality", "[Version]") {
  const http::Version a;
  const http::Version b {2U, 0U};
  REQUIRE(a not_eq b);
}

///////////////////////////////////////////////////////////////////////////////
TEST_CASE("Greater than", "[Version]") {
  const http::Version a;
  const http::Version b {2U, 0U};
  REQUIRE(b > a);
}

///////////////////////////////////////////////////////////////////////////////
TEST_CASE("Less than", "[Version]") {
  const http::Version a;
  const http::Version b {2U, 0U};
  REQUIRE(a < b);
}
