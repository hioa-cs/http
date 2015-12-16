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

#ifndef HTTP_METHODS_HPP
#define HTTP_METHODS_HPP

#include <string>

#include "method_constants.hpp" //< Code mappings to standard HTTP methods

namespace http {
namespace method {
//-------------------------------
using Method = const std::string;
//-------------------------------
Method OPTIONS {"OPTIONS"};
Method GET     {"GET"};
Method HEAD    {"HEAD"};
Method POST    {"POST"};
Method PUT     {"PUT"};
Method DELETE  {"DELETE"};
Method TRACE   {"TRACE"};
Method CONNECT {"CONNECT"};
Method PATCH   {"PATCH"};
//-------------------------------
} //< namespace method

//-------------------------------
// Get a code mapping from an HTTP
// method as a <std::string> object
//
// @param method - The HTTP method
//
// @return - The code mapped to the method
//-------------------------------
inline unsigned http_method_code(const std::string& method) noexcept {
  if (method == method::OPTIONS) return HTTP_OPTIONS;
  //----------------------------------
  if (method == method::GET)     return HTTP_GET;
  //----------------------------------
  if (method == method::HEAD)    return HTTP_HEAD;
  //----------------------------------
  if (method == method::POST)    return HTTP_POST;
  //----------------------------------
  if (method == method::PUT)     return HTTP_PUT;
  //----------------------------------
  if (method == method::DELETE)  return HTTP_DELETE;
  //----------------------------------
  if (method == method::TRACE)   return HTTP_TRACE;
  //----------------------------------
  if (method == method::CONNECT) return HTTP_CONNECT;
  //----------------------------------
  if (method == method::PATCH)   return HTTP_PATCH;
  //----------------------------------
  return HTTP_INVALID;
}

} //< namespace http

#endif //< HTTP_METHODS_HPP
