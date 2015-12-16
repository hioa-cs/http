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

TEST_CASE("Default constructor only creates request line", "[Request]") {
  Request request;
  //-------------------------
  string test_string = "GET / HTTP/1.1" CRLF CRLF;
  //-------------------------
  REQUIRE(request.to_string() == test_string);
}

TEST_CASE("Constructor to parse a character stream", "[Request]") {
  string ingress = "GET https://github.com/hioa-cs/IncludeOS HTTP/1.1" CRLF
                   "Connection: close" CRLF CRLF;
  //-------------------------                   
  Request request {std::move(ingress)};
  //-------------------------
  REQUIRE(request.get_method() == "GET"s);
  REQUIRE(request.get_uri() == "https://github.com/hioa-cs/IncludeOS"s);
  REQUIRE(request.get_version() == Version(1, 1));
  REQUIRE(request.has_header("Connection"s) == true);
  REQUIRE(request.get_header_value("Connection"s) == "close"s);
}

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
  REQUIRE(request.to_string() == test_string);
}

TEST_CASE("Handling query data", "[HTTP_Request]") {
  string ingress = "GET includeos.net/q?file=install.sh&machine=x86_64 HTTP/1.1" CRLF
                   "Host: includeos.server:8080" CRLF
                   "Connection: close" CRLF CRLF;
  //-------------------------
  Request request {std::move(ingress)};
  //-------------------------
  REQUIRE(request.get_query_value("file"s) == "install.sh"s);
  REQUIRE(request.get_query_value("machine"s) == "x86_64"s);
}

TEST_CASE("Handling post data", "[HTTP_Request]") {
  string ingress = "POST / HTTP/1.1" CRLF
                   "Host: includeos.server:8080" CRLF
                   "Connection: close" CRLF CRLF
                   "name=rico&language=cpp&project=includeos";
  //-------------------------
  Request request {std::move(ingress)};
  //-------------------------
  REQUIRE(request.get_post_value("name"s) == "rico"s);
  REQUIRE(request.get_post_value("language"s) == "cpp"s);
  REQUIRE(request.get_post_value("project"s) == "includeos"s);
}

TEST_CASE("Header value folding", "[HTTP_Request]") {
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
  REQUIRE(request.to_string() == test_string);
}
