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

#ifndef HTTP_REQUEST_LINE_HPP
#define HTTP_REQUEST_LINE_HPP

#include <regex>
#include <cctype>

#include "common.hpp"
#include "methods.hpp"
#include "version.hpp"

namespace http {

/**
 * @brief This class represents the {Request-Line}
 * of an HTTP request message
 */
class Request_Line {
public:
  /**
   * @brief Default constructor
   */
  explicit Request_Line() = default;

  /**
   * @brief Constructor to construct a {Request-Line}
   * from the incoming character stream of data which
   * is a {std::string} object
   *
   * @tparam T request - The character stream of data
   */
  template
  <
    typename T,
    typename = std::enable_if_t
               <std::is_same
               <std::string, std::remove_reference_t
               <std::remove_const_t<T>>>::value>
  >
  explicit Request_Line(T&& request);

  // Default lifetime operations
  Request_Line(Request_Line&)  = default;
  Request_Line(Request_Line&&) = default;
  Request_Line& operator = (Request_Line&)  = default;
  Request_Line& operator = (Request_Line&&) = default;
  ~Request_Line() = default;

  /**
   * @brief Get the method of the message
   *
   * @return - The method of the message
   */
  const Method& get_method() const noexcept;

  /**
   * @brief Set the method of the message
   *
   * @param method - The method of the message
   */
  void set_method(const Method& method);

  /**
   * @brief Get the URI of the message
   *
   * @return - The URI of the message
   */
  const URI& get_uri() const noexcept;

  /**
   * @brief Set the URI of the message
   *
   * @param uri - The URI of the message
   */
  void set_uri(const URI& uri);

  /**
   * @brief Get the version of the message
   *
   * @return - The version of the message
   */
  const Version& get_version() const noexcept;

  /**
   * @brief Set the version of the message
   *
   * @param version - The version of the message
   */
  void set_version(const Version& version) noexcept;

  /**
   * @brief Get a string representation of this
   * class
   *
   * @return - A string representation
   */
  std::string to_string() const;

  /**
   * @brief Operator to transform this class
   * into string form
   */
  operator std::string () const;
  //-----------------------------------
private:
  //-----------------------------------
  // Class data members
  //-----------------------------------
  Method  method_  { GET };
  URI     uri_     {"/"};
  Version version_ {1U, 1U};
}; //< class Request_Line

/**
 * @brief This class is used to represent an error that occurred
 * from within the operations of class Request_Line
 */
class Request_line_error : public std::runtime_error {
  using runtime_error::runtime_error;
};

/**--v----------- Implementation Details -----------v--**/

template <typename T, typename>
inline Request_Line::Request_Line(T&& request) {
  if (request.empty() or request.size() < 15 /*<-(15) minimum request length */) {
    throw Request_line_error("Invalid request");
  }

  // Extract {Request-Line} from request
  std::string request_line;
  std::size_t index;

  if ((index = request.find("\r\n")) not_eq std::string::npos) {
    request_line = {request.substr(0, index)};
  } else if ((index = request.find('\n')) not_eq std::string::npos) {
    request_line = {request.substr(0, index)};
  } else {
    throw Request_line_error("Invalid line-ending");
  }

  // Should identify strings according to RFC 2616 sect.5.1
  // https://tools.ietf.org/html/rfc2616#section-5.1
  const static std::regex request_line_pattern
  {
    "\\s*(GET|POST|PUT|DELETE|OPTIONS|HEAD|TRACE|CONNECT) " // Method
    "(\\S+) " // URI
    "HTTP/(\\d+)\\.(\\d+)" // Version Major.Minor
  };

  std::smatch m;
  auto matched = std::regex_match(request_line, m, request_line_pattern);

  if (not matched) {
    throw Request_line_error("Invalid request line: " + request_line);
  }

  method_ = method::code(m[1]);

  new (&uri_) URI(m[2]);

  unsigned maj = static_cast<unsigned>(std::stoul(m[3]));
  unsigned min = static_cast<unsigned>(std::stoul(m[4]));
  version_ = Version{maj, min};

  // Trim the request for further processing
  request = request.substr(index + 2);
}

inline const Method& Request_Line::get_method() const noexcept {
  return method_;
}

inline void Request_Line::set_method(const Method& method) {
  method_ = method;
}

inline const URI& Request_Line::get_uri() const noexcept {
  return uri_;
}

inline void Request_Line::set_uri(const URI& uri) {
  new (&uri_) URI(uri);
}

inline const Version& Request_Line::get_version() const noexcept {
  return version_;
}

inline void Request_Line::set_version(const Version& version) noexcept {
  version_ = version;
}

inline std::string Request_Line::to_string() const {
  return method::str(method_)+" "+uri_.to_string()+" "
    +version_.to_string();
}

inline Request_Line::operator std::string () const {
  std::ostringstream req_line;
  //-----------------------------
  req_line << method_  << " "
           << uri_     << " "
           << version_ << "\r\n";
 //------------------------------
  return req_line.str();
}

inline std::ostream& operator << (std::ostream& output_device, const Request_Line& req_line) {
  return output_device << req_line.to_string();
}

/**--^----------- Implementation Details -----------^--**/

} //< namespace http

#endif //< HTTP_REQUEST_LINE_HPP
