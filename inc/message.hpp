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

#include "time.hpp"
#include "header.hpp"

namespace http {

/**
 * @brief This class is used as a generic base class for an
 * HTTP message
 */
class Message {
private:
  //----------------------------------------
  // Internal class type aliases
  using HSize        = Limit;
  using HValue       = const std::string&;
  using Message_Body = std::string;
  //----------------------------------------
public:
  /**
   * @brief Default constructor
   */
  explicit Message() = default;

  /**
   * @brief Constructor to specify the limit of how many
   * fields that can be added to the header section
   *
   * @param limit:
   * Maximum number of fields that can be added to the
   * message             
   */
  explicit Message(const Limit limit) noexcept;

  /**
   * @brief Default copy constructor
   */
  Message(const Message&) = default;

  /**
   * @brief Default move constructor
   */
  explicit Message(Message&&) noexcept = default;

  /**
   * @brief Default destructor
   */
  virtual ~Message() noexcept = default;

  /**
   * @brief Default copy assignment operator
   */
  Message& operator = (const Message&) = default;

  /**
   * @brief Default move assignment operator
   */
  Message& operator = (Message&&) = default;

  /**
   * @brief Set the maximum number of fields that
   * can be added to the header section for an instance
   * of this class
   *
   * @param limit:
   * The maximum number of fields that can be added to
   * the header section
   *
   * @return The object that invoked this method
   */
  Message& set_header_limit(const Limit limit) noexcept;

  /**
   * @brief Get the current field limit for this
   * message
   *
   * @return The current field limit
   */
  Limit get_header_limit() const noexcept;

  /**
   * @brief Add a new field to the current set of
   * headers
   *
   * @tparam F field:
   * The name of the field
   *
   * @tparam V value:
   * The field value
   *
   * @return The object that invoked this method
   */
  template
  <
    typename F, typename V,
    typename = std::enable_if_t
               <std::is_same
               <std::string, std::remove_reference_t
               <std::remove_const_t<F>>>::value and
                std::is_same
               <std::string, std::remove_reference_t
               <std::remove_const_t<V>>>::value>
  >
  Message& add_header(F&& field, V&& value);

  /**
   * @brief Add a set of fields to the header section of
   * the message from a {std::string} object in the
   * following format:
   *
   * "name: value\r\n"
   * "name: value\r\n"
   * ...
   *
   * @tparam D data:
   * The set of fields to add to the header section of this
   * message                     
   *
   * @return The object that invoked this method
   */
  template
  <
    typename D,
    typename = std::enable_if_t
               <std::is_same
               <std::string, std::remove_reference_t
               <std::remove_const_t<D>>>::value>
  >
  Message& add_headers(D&& data);

  /**
   * @brief Change the value of the specified field
   *
   * If the field is absent from the message it
   * will be added with the associated value if
   * within capacity of maximum allowable limit
   *
   * @tparam F field:
   * The name of the field

   * @tparam V value:
   * The field value
   *
   * @return The object that invoked this method
   */
  template
  <
    typename F, typename V,
    typename = std::enable_if_t
               <std::is_same
               <std::string, std::remove_reference_t
               <std::remove_const_t<F>>>::value and
                std::is_same
               <std::string, std::remove_reference_t
               <std::remove_const_t<V>>>::value>
  >
  Message& set_header(F&& field, V&& value);

  /**
   * @brief Get a read-only reference to the object that
   * represents the header section in this message
   *
   * @return The header in this message
   */
  const Header& get_header() const noexcept;

  /**
   * @brief Get the value associated with the
   * specified field name
   *
   * Should call <has_header> before calling this
   *
   * @tparam F field:
   * The field name to get associated value
   *
   * @return The value associated with the specified
   * field name
   */
  template
  <
    typename F,
    typename = std::enable_if_t
               <std::is_same
               <std::string, std::remove_reference_t
               <std::remove_const_t<F>>>::value>
  >
  HValue header_value(F&& field) const noexcept;

  /**
   * @brief Check if the specified field is within
   * this message
   *
   * @tparam F field:
   * The field name to search for
   *
   * @return true is present, false otherwise
   */
  template
  <
    typename F,
    typename = std::enable_if_t
               <std::is_same
               <std::string, std::remove_reference_t
               <std::remove_const_t<F>>>::value>
  >
  bool has_header(F&& field) const noexcept;

  /**
   * @brief Remove the specified field from this
   * message
   *
   * @tparam F field:
   * The header field to remove from this message
   *
   * @return The object that invoked this method
   */
  template
  <
    typename F,
    typename = std::enable_if_t
               <std::is_same
               <std::string, std::remove_reference_t
               <std::remove_const_t<F>>>::value>
  >
  Message& erase_header(F&& field) noexcept;

  /**
   * @brief Remove all header fields from this
   * message
   *
   * @return The object that invoked this method
   */
  Message& clear_headers() noexcept;

  /**
   * @brief Check if there are no fields in this
   * message
   *
   * @return true if the message has no fields,
   * false otherwise
   */
  bool is_header_empty() const noexcept;

  /**
   * @brief Get the number of fields in this
   * message
   *
   * @return The number of fields in this message
   */
  HSize header_size() const noexcept;

  /**
   * @brief Add an entity to the message
   *
   * @tparam E message_body:
   * The entity to be sent with the message
   *
   * @return The object that invoked this method
   */
  template
  <
    typename E,
    typename = std::enable_if_t
               <std::is_same
               <std::string, std::remove_reference_t
               <std::remove_const_t<E>>>::value>
  >
  Message& add_body(E&& message_body);

  /**
   * @brief Get a read-only reference to the entity in
   * this the message
   *
   * @return A read-only reference to the entity in
   * this the message
   */
  const Message_Body& get_body() const noexcept;

  /**
   * @brief Remove the entity from the message
   *
   * @return The object that invoked this method
   */
  Message& clear_body() noexcept;

  /**
   * @brief Reset the message as if it was now
   * default constructed
   *
   * @return The object that invoked this method
   */
  virtual Message& reset() noexcept;

  /**
   * @brief Get a string representation of this
   * class
   *
   * @return A string representation
   */
  virtual std::string to_string() const;

  /**
   * @brief Operator to transform this class
   * into string form
   */
  operator std::string () const;
private:
  //------------------------------
  // Class data members
  Header       header_fields_;
  Message_Body message_body_;
  //------------------------------
}; //< class Message

/**--v----------- Implementation Details -----------v--**/

inline Message::Message(const Limit limit) noexcept:
  header_fields_{limit},
  message_body_{}
{}

inline Message& Message::set_header_limit(const Limit limit) noexcept {
  header_fields_.set_limit(limit);
  return *this;
}

inline Limit Message::get_header_limit() const noexcept {
  return header_fields_.get_limit();
}

template <typename Field, typename Value, typename>
inline Message& Message::add_header(Field&& field, Value&& value) {
  header_fields_.add_field(std::forward<Field>(field), std::forward<Value>(value));
  return *this;
}

template <typename Data, typename>
inline Message& Message::add_headers(Data&& data) {
  header_fields_.add_fields(std::forward<Data>(data));
  return *this;
}

template <typename Field, typename Value, typename>
inline Message& Message::set_header(Field&& field, Value&& value) {
  header_fields_.set_field(std::forward<Field>(field), std::forward<Value>(value));
  return *this;
}

inline const Header& Message::get_header() const noexcept {
  return header_fields_;
}

template <typename Field, typename>
inline Message::HValue Message::header_value(Field&& field) const noexcept {
  return header_fields_.get_value(std::forward<Field>(field));
}

template <typename Field, typename>
inline bool Message::has_header(Field&& field) const noexcept {
  return header_fields_.has_field(std::forward<Field>(field));
}

template <typename Field, typename>
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

template<typename Entity, typename>
inline Message& Message::add_body(Entity&& message_body) {
  if (message_body.empty()) return *this;
  //-----------------------------------
  message_body_ = std::forward<Entity>(message_body);
  //-----------------------------------
  return add_header(header_fields::Entity::Content_Length,
                    std::to_string(message_body_.size()));
}

inline const Message::Message_Body& Message::get_body() const noexcept {
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
  std::ostringstream message;
  //-----------------------------------
  message << header_fields_
          << message_body_;
  //-----------------------------------
  return message.str();
}

inline Message::operator std::string () const {
  return to_string();
}

inline std::ostream& operator << (std::ostream& output_device, const Message& message) {
  return output_device << message.to_string();
}

/**--^----------- Implementation Details -----------^--**/

} //< namespace http

#endif //< HTTP_MESSAGE_HPP
