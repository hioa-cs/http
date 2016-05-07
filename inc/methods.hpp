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

namespace http {

  enum Method {
    GET, POST, PUT, DELETE, OPTIONS, HEAD, TRACE, CONNECT, PATCH,
    INVALID = 0xffff
  };

  namespace method {

    /** Get method string from method code **/
    inline std::string str(Method m){

      static std::array<std::string, 9> strings {
        {"GET", "POST", "PUT", "DELETE",
            "OPTIONS", "HEAD", "TRACE", "CONNECT", "PATCH" }};

      if (m < strings.size() - 1)
        return strings[m] ;

      return "INVALID";
    }

    /**
     * Get a code mapping from an HTTP
     * method
     *
     * @param method - The HTTP method
     *
     * @return - The code mapped to the method
     **/
    inline Method code(const std::string& method) noexcept {

      if (method == str(GET))     return GET;
      if (method == str(POST))    return POST;
      if (method == str(PUT))     return PUT;
      if (method == str(DELETE))  return DELETE;
      if (method == str(OPTIONS)) return OPTIONS;
      if (method == str(HEAD))    return HEAD;
      if (method == str(TRACE))   return TRACE;
      if (method == str(CONNECT)) return CONNECT;
      if (method == str(PATCH))   return PATCH;

      return INVALID;
    }

  } // namespace method

} // namespace http

#endif // HTTP_METHODS_HPP
