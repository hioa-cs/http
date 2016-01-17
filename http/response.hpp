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

#ifndef HTTP_RESPONSE_HPP
#define HTTP_RESPONSE_HPP

#include "message.hpp"
#include "status_line.hpp"

#include "status_code_constants.hpp" //< Standard status codes

namespace http {

//--------------------------------
// This class is used to represent
// an http response message
//--------------------------------
class Response : public Message {
private:
  //------------------------------
  // Internal class type aliases
  //------------------------------
  using Code  = int;
  using Limit = std::size_t;
  //------------------------------
public:
  //------------------------------
  // Constructor to set up a response
  // by providing information for the
  // status line of the response message
  //
  // @param code    - The status code
  // @param version - The version of the message
  //------------------------------
  explicit Response(const Code code = OK, const Version& version = Version{}) noexcept;

  //----------------------------------------
  // Constructor to construct a response
  // message from the incoming character
  // stream of data which is a <std::string>
  // object
  //
  // @tparam (std::string) response - The character stream
  //                                  of data
  //
  // @param limit - Capacity of how many fields can
  //                be added
  //----------------------------------------
  template <typename Egress>
  explicit Response(Egress&& response, const Limit limit = 100);

  //------------------------------
  // Default destructor
  //------------------------------
  ~Response() noexcept = default;

  //------------------------------
  // Change the status code of this
  // message
  //
  // @param code - The new status code to set
  //               on this message
  //
  // @return - The object that invoked this method
  //------------------------------
  Response& set_status_code(const Code code) noexcept;

  //----------------------------------------
  // Reset the response message as if it was now
  // default constructed
  //
  // @return - The object that invoked this method
  //----------------------------------------
  virtual Response& reset() noexcept override;
  
  //-----------------------------------
  // Get a string representation of this
  // class
  //
  // @return - A string representation
  //-----------------------------------
  virtual std::string to_string() const override;

  //-----------------------------------
  // Operator to transform this class
  // into string form
  //-----------------------------------
  operator std::string () const;
  //-----------------------------------
private:
  //------------------------------
  // Class data members
  //------------------------------
  Status_Line  status_line_;

  //-----------------------------------
  // Deleted move and copy operations
  //-----------------------------------
  Response(const Response&) = delete;
  Response(Response&&) = delete;

  //-----------------------------------
  // Deleted move and copy assignment operations
  //-----------------------------------
  Response& operator = (const Response&) = delete;
  Response& operator = (Response&&) = delete;
}; //< class Response

inline Response::Response(const Code code, const Version& version) noexcept:
  status_line_{version, code}
{}

template <typename Egress>
inline Response::Response(Egress&& response, const Limit limit) :
  Message{limit},
  status_line_{response}
{
  add_headers(std::forward<Egress>(response));
  add_body(response.substr(response.find("\r\n\r\n") + 4));
}

inline Response& Response::set_status_code(const Code code) noexcept {
  status_line_.set_code(code);
  return *this;
}

inline Response& Response::reset() noexcept {
  Message::reset();
  return set_status_code(OK);
}

inline std::string Response::to_string() const {
  return *this;
}

inline Response::operator std::string () const {
  std::ostringstream res;
  //-----------------------------------
  res << status_line_
      << Message::to_string();
  //-----------------------------------
  return res.str();
}

inline std::ostream& operator << (std::ostream& output_device, const Response& res) {
  return output_device << res.to_string();
}

} //< namespace http

#endif //< HTTP_RESPONSE_HPP
