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

#include <array>
#include <string>
#include <ostream>

namespace http {

  /**
   * @enum This type consist of mappings from HTTP method strings
   * to their respective internal code values
   */
  enum Method {
    GET, POST, PUT, DELETE, OPTIONS, HEAD, TRACE, CONNECT, PATCH,
    INVALID = 0xffff
  }; //< enum Method

  namespace method {

    /**
     * @brief Get the string representation from an HTTP
     * method code
     *
     * @param m:
     * The HTTP method code
     *
     * @return The string representation of the code
     */
    inline const std::string& str(const Method m) {

      static std::array<std::string, 10> strings
      {
        {
         "GET", "POST", "PUT", "DELETE", "OPTIONS",
         "HEAD", "TRACE", "CONNECT", "PATCH", "INVALID"
        }
      };

      if ((m >= 0) and (m < (strings.size() - 1))) {
        return strings[m];
      }

      return strings[strings.size() - 1];
    }

    /**
     * @brief Get a code mapping from an HTTP
     * method string
     *
     * @param method:
     * The HTTP method code
     *
     * @return The code mapped to the method string
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

  } //< namespace method

  /**
   * @brief Operator to stream an HTTP method code into the specified
   * output device
   *
   * @param output_device:
   * The output device to stream the HTTP method code into
   *
   * @param m:
   * An HTTP method code
   *
   * @return Reference to the specified output device
   */
  inline std::ostream& operator << (std::ostream& output_device, const Method m) {
    return output_device << http::method::str(m);
  }

} //< namespace http

#endif //< HTTP_METHODS_HPP
