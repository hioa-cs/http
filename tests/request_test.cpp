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
using namespace http;

///////////////////////////////////////////////////////////////////////////////
TEST_CASE("Default constructor only creates request line", "[Request]") {
  Request request;
  //-------------------------
  string test_string = "GET / HTTP/1.1" CRLF CRLF;
  //-------------------------
  REQUIRE(test_string == request.to_string());
}

///////////////////////////////////////////////////////////////////////////////
TEST_CASE("Constructor to parse a character stream", "[Request]") {
  string ingress = "GET https://github.com/hioa-cs/IncludeOS HTTP/1.1" CRLF
                   "Connection: close" CRLF CRLF;
  //-------------------------                   
  Request request {std::move(ingress)};
  //-------------------------
  REQUIRE(request.method()                    == "GET");
  REQUIRE(request.uri()                       == "https://github.com/hioa-cs/IncludeOS");
  REQUIRE(request.version()                   == Version(1, 1));
  REQUIRE(request.has_header("Connection"s)   == true);
  REQUIRE(request.header_value("Connection"s) == "close");
}

///////////////////////////////////////////////////////////////////////////////
TEST_CASE("Building a request", "[Request]") {
  Request request;
  //-------------------------
  request.add_header("Host"s, "includeos.server:8080"s)
         .add_header("Accept"s, "text/html"s)
         .add_header("Connection"s, "close"s);
  //-------------------------
  string test_string = "GET / HTTP/1.1" CRLF
                       "Host: includeos.server:8080" CRLF
                       "Accept: text/html" CRLF
                       "Connection: close" CRLF CRLF;
  //-------------------------
  REQUIRE(test_string == request.to_string());
}

///////////////////////////////////////////////////////////////////////////////
TEST_CASE("Handling query data", "[Request]") {
  string ingress = "GET includeos.net/q?file=install.sh&machine=x86_64 HTTP/1.1" CRLF
                   "Host: includeos.server:8080" CRLF
                   "Connection: close" CRLF CRLF;
  //-------------------------
  Request request {std::move(ingress)};
  //-------------------------
  REQUIRE(request.query_value("file"s)    == "install.sh");
  REQUIRE(request.query_value("machine"s) == "x86_64");
}

///////////////////////////////////////////////////////////////////////////////
TEST_CASE("Handling post data", "[Request]") {
  string ingress = "POST / HTTP/1.1" CRLF
                   "Host: includeos.server:8080" CRLF
                   "Connection: close" CRLF CRLF
                   "name=rico&language=cpp&project=includeos";
  //-------------------------
  Request request {std::move(ingress)};
  //-------------------------
  REQUIRE(request.post_value("name"s)     == "rico");
  REQUIRE(request.post_value("language"s) == "cpp");
  REQUIRE(request.post_value("project"s)  == "includeos");
}

///////////////////////////////////////////////////////////////////////////////
TEST_CASE("Header value folding", "[Request]") {
  string ingress = "GET / HTTP/1.1" CRLF
                   "Host: includeos.server:8080" CRLF
                   "Client: curl/7.68" CRLF
                   "Accept: text/plain;q=0.2," CRLF
                   "        text/html;q=0.9," CRLF
                   "        */*;q=0.1" CRLF
                   "Connection: close" CRLF CRLF;
  //-------------------------
  Request request {std::move(ingress)};
  //-------------------------
  string test_string = "GET / HTTP/1.1" CRLF
                       "Host: includeos.server:8080" CRLF
                       "Client: curl/7.68" CRLF
                       "Accept: text/plain;q=0.2, text/html;q=0.9, */*;q=0.1" CRLF
                       "Connection: close" CRLF CRLF;
  //-------------------------
  REQUIRE(test_string == request.to_string());
}
