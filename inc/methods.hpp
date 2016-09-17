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
#include <unordered_map>

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
    static const std::string& str(const Method m) {

      const static std::array<std::string, 10> strings
      {
        {
         "GET", "POST", "PUT", "DELETE", "OPTIONS",
         "HEAD", "TRACE", "CONNECT", "PATCH", "INVALID"
        }
      };

      auto e = strings.size() - 1;

      if ((m >= 0) and (m < e)) {
        return strings[m];
      }

      return strings[e];
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

      const static std::unordered_map<std::string, Method> code_map {
        {"GET",     GET},
        {"POST",    POST},
        {"PUT",     PUT},
        {"DELETE",  DELETE},
        {"OPTIONS", OPTIONS},
        {"HEAD",    HEAD},
        {"TRACE",   TRACE},
        {"CONNECT", CONNECT},
        {"PATCH",   PATCH}
      };

      auto it = code_map.find(method);

      return (it not_eq code_map.end()) ? it->second : INVALID;
    }

    inline bool is_content_length_allowed(const Method method) noexcept {
      return (method == POST) || (method == PUT);
    }

    inline bool is_content_length_required(const Method method) noexcept {
      return (method == POST) || (method == PUT);
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
