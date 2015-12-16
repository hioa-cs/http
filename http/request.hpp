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

#ifndef HTTP_REQUEST_HPP
#define HTTP_REQUEST_HPP

#include "request_line.hpp"
#include "header.hpp"

namespace http {

//----------------------------------------
// This class is used to represent
// an http request message
//----------------------------------------
class Request {
private:
  //----------------------------------------
  // Internal class type aliases
  //----------------------------------------
  using Method       = std::string;
  using URI          = std::string;
  using Limit        = std::size_t;
  using HSize        = std::size_t;
  using Message_Body = std::string;
  //----------------------------------------
public:
  //----------------------------------------
  // Default constructor
  //
  // Constructs a request message as follows:
  //
  // <GET / HTTP/1.1>
  //----------------------------------------
  explicit Request() = default;

  //----------------------------------------
  // Constructor to construct a request
  // message from the incoming character
  // stream of data which is a <std::string>
  // object
  //
  // @tparam (std::string) request - The character stream
  //                                 of data
  //----------------------------------------
  template <typename Ingress>
  explicit Request(Ingress&& request);

  //----------------------------------------
  // Default destructor
  //----------------------------------------
  ~Request() noexcept = default;

  //----------------------------------------
  // Get the method of the request message
  //
  // @return - The method of the request
  //----------------------------------------
  const Method& get_method() const noexcept;

  //----------------------------------------
  // Set the method of the request message
  //
  // @param method - The method to set
  //
  // @return - The object that invoked this
  //           method
  //----------------------------------------
  Request& set_method(const Method& method);

  //----------------------------------------
  // Get the URI of the request message
  //
  // @return - The URI of the request
  //----------------------------------------
  const URI& get_uri() const noexcept;

  //----------------------------------------
  // Set the URI of the request message
  //
  // @param uri - The URI to set
  //
  // @return - The object that invoked this
  //           method
  //----------------------------------------
  Request& set_uri(const URI& uri);

  //----------------------------------------
  // Get the version of the request message
  //
  // @return - The version of the request
  //----------------------------------------
  const Version& get_version() const noexcept;

  //----------------------------------------
  // Set the version of the request message
  //
  // @param version - The version to set
  //
  // @return - The object that invoked this
  //           method
  //----------------------------------------
  Request& set_version(const Version& version) noexcept;

  //----------------------------------------
  // Set a header limit for this request message
  //
  // @param limit - The limit to set
  //
  // @return - The object that invoked this
  //           method
  //----------------------------------------
  Request& set_header_limit(const Limit limit) noexcept;

  //----------------------------------------
  // Get the current header limit for this
  // request message
  //
  // @return - The current header limit
  //----------------------------------------
  Limit get_header_limit() const noexcept;

  //----------------------------------------
  // Add a header to this request message
  // which will only be added if the limit
  // set hasn't been reached
  //
  // @tparam (std::string) field - The field name
  // @tparam (std::string) value - The field value
  //
  // @return - The object that invoked this
  //           method
  //----------------------------------------
  template <typename Field, typename Value>
  Request& add_header(Field&& field, Value&& value);

  //----------------------------------------
  // Change the value of the specified field
  //
  // If the field is absent from the message it
  // will be added with the associated value once its
  // within capacity
  //
  // @tparam (std::string) field - The field name
  // @tparam (std::string) value - The field value
  //
  // @return - The object that invoked this
  //           method
  //----------------------------------------
  template <typename Field, typename Value>
  Request& set_header(Field&& field, Value&& value);

  //----------------------------------------
  // Get the value associated with the
  // specified header field name
  //
  // Should call <has_header> before
  // calling this
  //
  // @tparam (std::string) field - The header field name to
  //                               get associated value
  //
  // @return - The value associated with the
  //           specified field name
  //----------------------------------------
  template <typename Field>
  const std::string& get_header_value(Field&& field) const noexcept;

  //----------------------------------------
  // Check if the specified header field
  // is within this message
  //
  // @tparam (std::string) field - The header field name to
  //                               search for
  //
  // @return - true is present, false otherwise
  //----------------------------------------
  template <typename Field>
  bool has_header(Field&& field) const noexcept;

  //----------------------------------------
  // Check if there are no headers in this
  // message
  //
  // @return - true if the message has no
  //           header fields, false otherwise
  //----------------------------------------
  bool is_header_empty() const noexcept;

  //----------------------------------------
  // Get the number of headers in this
  // message
  //
  // @return - The number of fields in this
  //           message
  //----------------------------------------
  HSize header_size() const noexcept;

  //----------------------------------------
  // Remove the specified header field from
  // this message
  //
  // @tparam (std::string) field - The header field name to
  //                               remove from this message
  //
  // @return - The object that invoked this
  //           method
  //----------------------------------------
  template <typename Field>
  Request& erase_header(Field&& field) noexcept;

  //----------------------------------------
  // Remove all header fields from this
  // message
  //
  // @return - The object that invoked this
  //           method
  //----------------------------------------
  Request& clear_headers() noexcept;

  //----------------------------------------
  // Get the value associated with the name
  // field from a query string
  //
  // @tparam (std::string) name - The name to find the associated
  //                              value
  //
  // @return - The associated value if name was found,
  //           an empty string otherwise
  //----------------------------------------
  template <typename Name>
  std::string get_query_value(Name&& name) const noexcept;

  //----------------------------------------
  // Get the value associated with the name
  // field from the message body in a post request
  //
  // @tparam (std::string) name - The name to find the associated
  //                              value
  //
  // @return - The associated value if name was found,
  //           an empty string otherwise
  //----------------------------------------
  template <typename Name>
  std::string get_post_value(Name&& name) const noexcept;

  //----------------------------------------
  // Get a string representation of this
  // class
  //
  // @return - A string representation
  //----------------------------------------
  std::string to_string() const;

  //----------------------------------------
  // Operator to transform this class
  // into string form
  //----------------------------------------
  operator std::string () const;
  //----------------------------------------
private:
  //----------------------------------------
  // Class data members
  //----------------------------------------
  Request_Line request_line_;
  Header       header_fields_;
  Message_Body message_body_;

  //----------------------------------------
  // Find the value associated with a name
  // in the following format:
  //
  // name=value
  //
  // @tparam (std::string) data - The data to search through
  // @tparam (std::string) name - The name to find the associated
  //                              value
  //
  // @return - The associated value if name was found,
  //           an empty string otherwise
  //----------------------------------------
  template <typename Data, typename Name>
  std::string get_value(Data&& data, Name&& name) const noexcept;
}; //< class HTTP_Request

template <typename Ingress>
inline Request::Request(Ingress&& request) :
  request_line_{std::forward<Ingress>(request)},
  header_fields_{std::forward<Ingress>(request)},
  message_body_{request.substr(request.find("\r\n\r\n") + 4)}
{}

inline const Request::Method& Request::get_method() const noexcept {
  return request_line_.get_method();
}

inline Request& Request::set_method(const Method& method) {
  request_line_.set_method(method);
  return *this;
}

inline const Request::URI& Request::get_uri() const noexcept {
  return request_line_.get_uri();
}

inline Request& Request::set_uri(const URI& uri) {
  request_line_.set_uri(uri);
  return *this;
}

inline const Version& Request::get_version() const noexcept {
  return request_line_.get_version();
}

inline Request& Request::set_version(const Version& version) noexcept {
  request_line_.set_version(version);
  return *this;
}

inline Request& Request::set_header_limit(const Limit limit) noexcept {
  header_fields_.set_limit(limit);
  return *this;
}

inline Request::Limit Request::get_header_limit() const noexcept {
  return header_fields_.get_limit();
}

template <typename Field, typename Value>
inline Request& Request::add_header(Field&& field, Value&& value) {
  header_fields_.add_field(std::forward<Field>(field), std::forward<Value>(value));
  return *this;
}

template <typename Field, typename Value>
inline Request& Request::set_header(Field&& field, Value&& value) {
  header_fields_.set_field(std::forward<Field>(field), std::forward<Value>(value));
  return *this;
}

template <typename Field>
inline const std::string& Request::get_header_value(Field&& field) const noexcept {
  return header_fields_.get_value(std::forward<Field>(field));
}

template <typename Field>
inline bool Request::has_header(Field&& field) const noexcept {
  return header_fields_.has_field(std::forward<Field>(field));
}

inline bool Request::is_header_empty() const noexcept {
  return header_fields_.is_empty();
}

inline Request::HSize Request::header_size() const noexcept {
  return header_fields_.size();
}

template <typename Field>
inline Request& Request::erase_header(Field&& field) noexcept {
  header_fields_.erase(std::forward<Field>(field));
  return *this;
}

inline Request& Request::clear_headers() noexcept {
  header_fields_.clear();
  return *this;
}

template <typename Name>
inline std::string Request::get_query_value(Name&& name) const noexcept {
  return get_value(get_uri(), std::forward<Name>(name));
}

template <typename Name>
inline std::string Request::get_post_value(Name&& name) const noexcept {
  if (get_method() not_eq method::POST) return std::string{};
  return get_value(message_body_, std::forward<Name>(name));
}

template <typename Data, typename Name>
inline std::string Request::get_value(Data&& data, Name&& name) const noexcept {
  if (data.empty() || name.empty()) return std::string{};
  //---------------------------------
  auto target = data.find(std::forward<Name>(name));
  //---------------------------------
  if (target == std::string::npos) return std::string{};
  //---------------------------------
  auto focal_point = data.substr(target);
  //---------------------------------
  focal_point = focal_point.substr(0, focal_point.find_first_of('&'));
  //---------------------------------
  auto lock_and_load = focal_point.find('=');
  //---------------------------------
  if (lock_and_load == std::string::npos) return std::string{};
  //---------------------------------
  return focal_point.substr(lock_and_load + 1);
}

inline std::string Request::to_string() const {
  return *this;
}

inline Request::operator std::string () const {
  std::ostringstream req;
  //-----------------------------
  req << request_line_
      << header_fields_
      << message_body_;
 //------------------------------
  return req.str();
}

std::ostream& operator << (std::ostream& output_device, const Request& req) {
  return output_device << req.to_string();
}

} //< namespace http

#endif //< HTTP_REQUEST_HPP
