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

#include "status_line.hpp"
#include "header.hpp"

#include "status_code_constants.hpp" //< Standard status codes

namespace http {

//--------------------------------
// This class is used to represent
// an http response message
//--------------------------------
class Response {
private:
  //------------------------------
  // Internal class type aliases
  //------------------------------
  using Message_Body = std::string;
  using Code         = unsigned;
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
  // Add a new field to the current set of
  // headers
  //
  // @tparam (std::string) field - The field name
  // @tparam (std::string) value - The field value
  //
  // @return - The object that invoked this method
  //----------------------------------------
  template <typename Field, typename Value>
  Response& add_header(Field&& field, Value&& value);

  //----------------------------------------
  // Change the value of the specified field
  //
  // If the field is absent from the message it
  // will be added with the associated value
  //
  // @tparam (std::string) field - The field name
  // @tparam (std::string) value - The field value
  //
  // @return - The object that invoked this method
  //----------------------------------------
  template <typename Field, typename Value>
  Response& set_header(Field&& field, Value&& value);

  //----------------------------------------
  // Remove the specified header field from
  // this message
  //
  // @tparam (std::string) field - The header field to
  //                               remove from this message
  //
  // @return - The object that invoked this method
  //----------------------------------------
  template <typename Field>
  Response& erase_header(Field&& field) noexcept;

  //----------------------------------------
  // Remove all header fields from this
  // message
  //
  // @return - The object that invoked this method
  //----------------------------------------
  Response& clear_headers() noexcept;

  //----------------------------------------
  // Add the payload to the reponse message
  //
  // @tparam (std::string) message_body - The payload to be
  //                                      sent with the response
  //                                      message
  //
  // @return - The object that invoked this method
  //----------------------------------------
  template <typename Payload>
  Response& add_body(Payload&& message_body);

  //----------------------------------------
  // Remove the entity from the message
  //
  // @return - The object that invoked this method
  //----------------------------------------
  Response& clear_body() noexcept;

  //----------------------------------------
  // Reset the message as if it was now
  // default constructed
  //
  // @return - The object that invoked this method
  //----------------------------------------
  Response& reset() noexcept;
  
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
  //------------------------------
  // Class data members
  //------------------------------
  Status_Line  status_line_;
  Header       header_fields_;
  Message_Body message_body_;

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

inline Response& Response::set_status_code(const Code code) noexcept {
  status_line_.set_code(code);
  return *this;
}

template<typename Field, typename Value>
inline Response& Response::add_header(Field&& field, Value&& value) {
  header_fields_.add_field(std::forward<Field>(field), std::forward<Value>(value));
  return *this;
}

template<typename Field, typename Value>
inline Response& Response::set_header(Field&& field, Value&& value) {
  header_fields_.set_field(std::forward<Field>(field), std::forward<Value>(value));
  return *this;
}

template<typename Field>
inline Response& Response::erase_header(Field&& field) noexcept {
  header_fields_.erase(std::forward<Field>(field));
  return *this;
}

inline Response& Response::clear_headers() noexcept {
  header_fields_.clear();
  return *this;
}

template<typename Payload>
inline Response& Response::add_body(Payload&& message_body) {
  message_body_ = std::forward<Payload>(message_body);
  return add_header(header_fields::Entity::Content_Length,
                    std::to_string(message_body_.size()));
}

inline Response& Response::clear_body() noexcept {
  message_body_.clear();
  return erase_header(header_fields::Entity::Content_Length);
}

inline Response& Response::reset() noexcept {
  return set_status_code(OK).clear_headers().clear_body();
}

inline std::string Response::to_string() const {
  return *this;
}

inline Response::operator std::string () const {
  std::ostringstream res;
  //-----------------------------------
  res << status_line_
      << header_fields_
      << message_body_;
  //-----------------------------------
  return res.str();
}

std::ostream& operator << (std::ostream& output_device, const Response& res) {
  return output_device << res.to_string();
}

} //< namespace http

#endif //< HTTP_RESPONSE_HPP
