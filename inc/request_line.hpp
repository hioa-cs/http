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

#include <cctype>
#include <regex>
#include "common.hpp"
#include "methods.hpp"
#include "version.hpp"

namespace http {

//-----------------------------------
// This class represents the request-line
// of an incoming http request message
//-----------------------------------
class Request_Line {
public:
  //-----------------------------------
  // Constructor to create a default
  // request-line which is as follows:
  //
  // <GET / HTTP/1.1>
  //-----------------------------------
  explicit Request_Line() = default;

  //-----------------------------------
  // Constructor to construct a request-line
  // from the incoming character stream of
  // data which is a <std::string> object
  //
  // @tparam (std::string) request - The character stream of
  //                                 data
  //-----------------------------------
  template <typename Request>
  explicit Request_Line(Request&& request);

  // Copy / move constructors
  Request_Line(Request_Line&) = default;
  Request_Line(Request_Line&&) = default;
  ~Request_Line() noexcept = default;
  Request_Line& operator = (Request_Line&) = default;
  Request_Line& operator = (Request_Line&&) = default;

  //-----------------------------------
  // Get the method of the message
  //
  // @return - The method of the message
  //-----------------------------------
  const Method& get_method() const noexcept;

  //-----------------------------------
  // Set the method of the message
  //
  // @param method - The method of the message
  //-----------------------------------
  void set_method(const Method& method);

  //-----------------------------------
  // Get the URI of the message
  //
  // @return - The URI of the message
  //-----------------------------------
  const URI& get_uri() const noexcept;

  //-----------------------------------
  // Set the URI of the message
  //
  // @param uri - The URI of the message
  //-----------------------------------
  void set_uri(const URI& uri);

  //-----------------------------------
  // Get the version of the message
  //
  // @return - The version of the message
  //-----------------------------------
  const Version& get_version() const noexcept;

  //-----------------------------------
  // Set the version of the message
  //
  // @param version - The version of the message
  //-----------------------------------
  void set_version(const Version& version) noexcept;

  //-----------------------------------
  // Get a string representation of this
  // class
  //
  // @return - A string representation
  //-----------------------------------
  std::string to_string() const;

  //-----------------------------------
  // Operator to transform this class
  // into string form
  //-----------------------------------
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


  class Request_line_error : public std::runtime_error {
    using runtime_error::runtime_error;
  };

/**--v----------- Implementation Details -----------v--**/

template <typename Request>
inline Request_Line::Request_Line(Request&& request) {
  if (request.empty() or request.size() < 16 /*<-(16) minimum request length */) {
    return;
  }

  // Extract HTTP method
  std::string request_line = {request.substr(0, request.find("\r\n"))};

  // Should identify strings according to RFC 2616 sect.5.1
  // https://tools.ietf.org/html/rfc2616#section-5.1
  std::regex re_request_line {
    "\\s*(GET|POST|PUT|DELETE|OPTIONS|HEAD|TRACE|CONNECT) " // Method
      "(\\S+) " // URI
      "HTTP/(\\d+)\\.(\\d+)" // Version Major.Minor
      };

  std::smatch m;
  auto matched = std::regex_match(request_line,m, re_request_line);

  if (not matched)
    throw Request_line_error("Invalid request line: " + request_line);

  method_ = method::code(m[1]);
  uri_ = URI {m[2]};
  unsigned maj = static_cast<unsigned>(std::stoul(m[3]));
  unsigned min = static_cast<unsigned>(std::stoul(m[4]));
  version_ = Version{maj, min};

  // Trim the request for further processing
  request = request.substr(request.find("\r\n") + 2);
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
  uri_ = uri;
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
