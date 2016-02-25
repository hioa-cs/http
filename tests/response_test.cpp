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
using namespace http::header_fields;

///////////////////////////////////////////////////////////////////////////////
TEST_CASE("Default constructor only creates status line", "[Response]") {
  http::Response response;
  //-------------------------
  string test_string = "HTTP/1.1 200 OK" CRLF CRLF;
  //-------------------------
  REQUIRE(test_string == response.to_string());
}

///////////////////////////////////////////////////////////////////////////////
SCENARIO("Given a Response object") {
   http::Response response;
   //-------------------------
   WHEN("We set it's status code") {
    response.set_status_code(http::status_t::Not_Found);
    //-------------------------
    THEN("It should be reflected") {
      REQUIRE(response.to_string() == "HTTP/1.1 404 Not Found" CRLF CRLF);
    }
   }
}

///////////////////////////////////////////////////////////////////////////////
TEST_CASE("Add header field", "[Response]") {
  http::Response response;
  //-------------------------
  response.add_header(Response::Server, "IncludeOS/0.7.0"s);
  //-------------------------
  string test_string = "HTTP/1.1 200 OK" CRLF
                       "Server: IncludeOS/0.7.0" CRLF CRLF;
  //-------------------------
  REQUIRE(test_string == response.to_string());
}

///////////////////////////////////////////////////////////////////////////////
TEST_CASE("Change header field value", "[Response]") {
  http::Response response;
  //-------------------------
  response.add_header(Entity::Content_Type, "text/plain"s)
          .set_header(Entity::Content_Type, "text/html"s);
  //-------------------------
  string test_string = "HTTP/1.1 200 OK" CRLF
                       "Content-Type: text/html" CRLF CRLF;
  //-------------------------
  REQUIRE(test_string == response.to_string());
}

///////////////////////////////////////////////////////////////////////////////
TEST_CASE("Erase header field", "[Response]") {
  http::Response response{http::status_t::Bad_Request};
  //-------------------------
  response.add_header(Response::Server, "IncludeOS/0.7.0"s)
          .erase_header(Response::Server);
  //-------------------------
  string test_string = "HTTP/1.1 400 Bad Request" CRLF CRLF;
  //-------------------------
  REQUIRE(test_string == response.to_string());
}

///////////////////////////////////////////////////////////////////////////////
TEST_CASE("Clear headers", "[Response]") {
  http::Response response{http::status_t::Bad_Request};
  //-------------------------
  response.add_header(Response::Server,     "IncludeOS/0.7.0"s)
          .add_header(Entity::Content_Type, "text/javascript"s)
          .clear_headers();
  //-------------------------
  string test_string = "HTTP/1.1 400 Bad Request" CRLF CRLF;
  //-------------------------
  REQUIRE(test_string == response.to_string());
}

///////////////////////////////////////////////////////////////////////////////
TEST_CASE("Add message body", "[Response]") {
  http::Response response;
  //-------------------------
  response.add_header(Response::Server,     "IncludeOS/0.7.0"s)
          .add_header(Entity::Content_Type, "text/javascript"s);
  //-------------------------
  string javascript_file = "document.write('Hello from IncludeOS');";
  //-------------------------
  response.add_body(javascript_file);
  //-------------------------
  string test_string = "HTTP/1.1 200 OK" CRLF
                       "Server: IncludeOS/0.7.0" CRLF
                       "Content-Type: text/javascript" CRLF
                       "Content-Length: 39" CRLF CRLF
                       "document.write('Hello from IncludeOS');";
  //-------------------------
  REQUIRE(test_string == response.to_string());
}
