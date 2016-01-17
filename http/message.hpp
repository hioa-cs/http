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

#ifndef HTTP_MESSAGE_HPP
#define HTTP_MESSAGE_HPP

#include <sstream>

#include "header.hpp"

namespace http {

class Message {
private:
  //----------------------------------------
  // Internal class type aliases
  using Limit        = std::size_t;
  using HSize        = std::size_t;
  using HValue       = const std::string&;
  using Message_Body = std::string;
  //----------------------------------------
public:
  //----------------------------------------
  // Default constructor
  //----------------------------------------
  explicit Message() = default;

  //----------------------------------------
  // Constructor to specify the limit of how many
  // fields that can be added to the message
  //
  // @param limit - Capacity of how many fields can
  //                be added to the message
  //----------------------------------------
  explicit Message(const Limit limit) noexcept;

  //----------------------------------------
  // Default destructor
  //----------------------------------------
  virtual ~Message() noexcept = default;

  //----------------------------------------
  // Set a field limit for this message
  //
  // @param limit - The limit to set
  //
  // @return - The object that invoked this
  //           method
  //----------------------------------------
  Message& set_header_limit(const Limit limit) noexcept;

  //----------------------------------------
  // Get the current field limit for this
  // message
  //
  // @return - The current field limit
  //----------------------------------------
  Limit get_header_limit() const noexcept;

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
  Message& add_header(Field&& field, Value&& value);

  //----------------------------------------
  // Add a set of fields to the message from
  // a <std::string> object in the following
  // format:
  //
  // "name: value\r\n"
  //
  // @tparam (std::string) data - The set of fields
  //                              to add to this
  //                              message
  //
  // @return - The object that invoked this method
  //----------------------------------------
  template <typename Data>
  Message& add_headers(Data&& data);

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
  Message& set_header(Field&& field, Value&& value);

  //----------------------------------------
  // Get the value associated with the
  // specified field name
  //
  // Should call <has_header> before calling this
  //
  // @tparam (std::string) field - The field name to
  //                               get associated value
  //
  // @return - The value associated with the
  //           specified field name
  //----------------------------------------
  template <typename Field>
  HValue header_value(Field&& field) const noexcept;

  //----------------------------------------
  // Check if the specified  field is within
  // this message
  //
  // @tparam (std::string) field - The field name to
  //                               search for
  //
  // @return - true is present, false otherwise
  //----------------------------------------
  template <typename Field>
  bool has_header(Field&& field) const noexcept;

  //----------------------------------------
  // Remove the specified field from this
  // message
  //
  // @tparam (std::string) field - The header field to
  //                               remove from this message
  //
  // @return - The object that invoked this method
  //----------------------------------------
  template <typename Field>
  Message& erase_header(Field&& field) noexcept;

  //----------------------------------------
  // Remove all header fields from this
  // message
  //
  // @return - The object that invoked this method
  //----------------------------------------
  Message& clear_headers() noexcept;

  //----------------------------------------
  // Check if there are no fields in this
  // message
  //
  // @return - true if the message has no
  //           fields, false otherwise
  //----------------------------------------
  bool is_header_empty() const noexcept;

  //----------------------------------------
  // Get the number of fields in this
  // message
  //
  // @return - The number of fields in this
  //           message
  //----------------------------------------
  HSize header_size() const noexcept;

  //----------------------------------------
  // Add an entity to the message
  //
  // @tparam (std::string) message_body - The entity to be
  //                                      sent with the
  //                                      message
  //
  // @return - The object that invoked this method
  //----------------------------------------
  template <typename Entity>
  Message& add_body(Entity&& message_body);

  //----------------------------------------
  // Get the entity in this the message
  //
  // @return - The entity in this message
  //----------------------------------------
  const Message_Body& get_body() const noexcept;

  //----------------------------------------
  // Remove the entity from the message
  //
  // @return - The object that invoked this method
  //----------------------------------------
  Message& clear_body() noexcept;

  //----------------------------------------
  // Reset the message as if it was now
  // default constructed
  //
  // @return - The object that invoked this method
  //----------------------------------------
  virtual Message& reset() noexcept;
  
  //-----------------------------------
  // Get a string representation of this
  // class
  //
  // @return - A string representation
  //-----------------------------------
  virtual std::string to_string() const;

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
  Header       header_fields_;
  Message_Body message_body_;

  //-----------------------------------
  // Deleted move and copy operations
  //-----------------------------------
  Message(const Message&) = delete;
  Message(Message&&) = delete;

  //-----------------------------------
  // Deleted move and copy assignment operations
  //-----------------------------------
  Message& operator = (const Message&) = delete;
  Message& operator = (Message&&) = delete;
}; //< class Message

inline Message::Message(const Limit limit) noexcept:
  header_fields_{limit},
  message_body_{}
{}

inline Message& Message::set_header_limit(const Limit limit) noexcept {
  header_fields_.set_limit(limit);
  return *this;
}

inline Message::Limit Message::get_header_limit() const noexcept {
  return header_fields_.get_limit();
}

template <typename Field, typename Value>
inline Message& Message::add_header(Field&& field, Value&& value) {
  header_fields_.add_field(std::forward<Field>(field), std::forward<Value>(value));
  return *this;
}

template <typename Data>
inline Message& Message::add_headers(Data&& data) {
  header_fields_.add_fields(std::forward<Data>(data));
  return *this;
}

template <typename Field, typename Value>
inline Message& Message::set_header(Field&& field, Value&& value) {
  header_fields_.set_field(std::forward<Field>(field), std::forward<Value>(value));
  return *this;
}

template <typename Field>
inline Message::HValue Message::header_value(Field&& field) const noexcept {
  return header_fields_.get_value(std::forward<Field>(field));
}

template <typename Field>
inline bool Message::has_header(Field&& field) const noexcept {
  return header_fields_.has_field(std::forward<Field>(field));
}

template <typename Field>
inline Message& Message::erase_header(Field&& field) noexcept {
  header_fields_.erase(std::forward<Field>(field));
  return *this;
}

inline Message& Message::clear_headers() noexcept {
  header_fields_.clear();
  return *this;
}

inline bool Message::is_header_empty() const noexcept {
  return header_fields_.is_empty();
}

inline Message::HSize Message::header_size() const noexcept {
  return header_fields_.size();
}

template<typename Entity>
inline Message& Message::add_body(Entity&& message_body) {
  if (message_body.empty()) return *this;
  //-----------------------------------
  message_body_ = std::forward<Entity>(message_body);
  //-----------------------------------
  return add_header(header_fields::Entity::Content_Length,
                    std::to_string(message_body_.size()));
}

const Message::Message_Body& Message::get_body() const noexcept {
  return message_body_;
}

inline Message& Message::clear_body() noexcept {
  message_body_.clear();
  return erase_header(header_fields::Entity::Content_Length);
}

inline Message& Message::reset() noexcept {
  return clear_headers().clear_body();
}

inline std::string Message::to_string() const {
  return *this;
}

inline Message::operator std::string () const {
  std::ostringstream message;
  //-----------------------------------
  message << header_fields_
          << message_body_;
  //-----------------------------------
  return message.str();
}

inline std::ostream& operator << (std::ostream& output_device, const Message& message) {
  return output_device << message.to_string();
}

} //< namespace http

#endif //< HTTP_MESSAGE_HPP