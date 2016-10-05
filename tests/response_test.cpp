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
#include <response.hpp>

#define CRLF "\r\n"

using namespace std;

///////////////////////////////////////////////////////////////////////////////
TEST_CASE("Default constructor only creates status line", "[Response]") {
  const http::Response response;
  const std::string test_string {"HTTP/1.1 200 OK" CRLF};
  REQUIRE(test_string == response.to_string());
}

///////////////////////////////////////////////////////////////////////////////
TEST_CASE("Get status code", "[Response]") {
  const http::Response response;
  REQUIRE(http::OK == response.status_code());
}

///////////////////////////////////////////////////////////////////////////////
SCENARIO("Given a Response object") {
   http::Response response;

   WHEN("We set it's status code") {
    response.set_status_code(http::Not_Found);

    THEN("It should be reflected") {
      const std::string test_string {"HTTP/1.1 404 Not Found" CRLF};
      REQUIRE(test_string == response.to_string());
    }
   }
}

///////////////////////////////////////////////////////////////////////////////
TEST_CASE("Add header field", "[Response]") {
  http::Response response;
  response.header().add_field(http::header_fields::server, "IncludeOS/Acorn v0.7.0");

  const std::string test_string {
    "HTTP/1.1 200 OK" CRLF
    "Server: IncludeOS/Acorn v0.7.0" CRLF CRLF
  };

  REQUIRE(test_string == response.to_string());
}

///////////////////////////////////////////////////////////////////////////////
TEST_CASE("{Date} header field", "[Response]") {
  http::Response response;
  const auto time_stamp = http::time::now();

  response.header().add_field(http::header_fields::server, "IncludeOS/Acorn v0.7.0");
  response.header().add_field(http::header_fields::date, time_stamp);

  const std::string test_string {
    "HTTP/1.1 200 OK"s + CRLF +
    "Server: IncludeOS/Acorn v0.7.0" + CRLF +
    "Date: " + time_stamp + CRLF + CRLF
  };

  REQUIRE(test_string == response.to_string());
}

///////////////////////////////////////////////////////////////////////////////
TEST_CASE("Change header field value", "[Response]") {
  http::Response response;

  response.header().add_field(http::header_fields::content_type, "text/plain");
  response.header().set_field(http::header_fields::content_type, "text/html");

  const std::string test_string {
    "HTTP/1.1 200 OK" CRLF
    "Content-Type: text/html" CRLF CRLF
  };

  REQUIRE(test_string == response.to_string());
}

///////////////////////////////////////////////////////////////////////////////
TEST_CASE("Erase header field", "[Response]") {
  http::Response response{http::Bad_Request};

  response.header().add_field(http::header_fields::server, "IncludeOS/Acorn v0.7.0");
  response.header().erase(http::header_fields::server);

  const std::string test_string {"HTTP/1.1 400 Bad Request" CRLF};

  REQUIRE(test_string == response.to_string());
}

///////////////////////////////////////////////////////////////////////////////
TEST_CASE("Clear headers", "[Response]") {
  http::Response response{http::Bad_Request};

  response.header().add_field(http::header_fields::server, "IncludeOS/Acorn v0.7.0");
  response.header().add_field(http::header_fields::content_type, "text/javascript");
  response.header().clear();

  const std::string test_string {"HTTP/1.1 400 Bad Request" CRLF};

  REQUIRE(test_string == response.to_string());
}

///////////////////////////////////////////////////////////////////////////////
TEST_CASE("Add message body", "[Response]") {
  http::Response response;

  response.header().add_field(http::header_fields::server, "IncludeOS/Acorn v0.7.0");
  response.header().add_field(http::header_fields::content_type, "text/javascript");

  const std::string javascript_file {"document.write('Hello from IncludeOS');"};

  response.add_body(javascript_file);

  const std::string test_string {
    "HTTP/1.1 200 OK" CRLF
    "Server: IncludeOS/Acorn v0.7.0" CRLF
    "Content-Type: text/javascript" CRLF
    "Content-Length: 39" CRLF CRLF
    "document.write('Hello from IncludeOS');"
  };

  REQUIRE(test_string == response.to_string());
}

///////////////////////////////////////////////////////////////////////////////
TEST_CASE("Make Response_ptr", "[Response]") {
  const auto response = http::make_response("HTTP/1.1 200 OK\r\n");
  const std::string test_string {"HTTP/1.1 200 OK\r\n"};
  REQUIRE(test_string == response->to_string());
}
