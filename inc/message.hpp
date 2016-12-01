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

#include "time.hpp"
#include "header.hpp"

namespace http {

/**
 * @brief This class is used as a generic base class for an
 * HTTP message
 */
class Message {
public:
  /**
   * @brief Default constructor
   */
  template<typename = void>
  explicit Message();

  /**
   * @brief Constructor to specify the limit of how many
   * fields that can be added to the header section
   *
   * @param limit:
   * Maximum number of fields that can be added to the
   * message
   */
  template<typename = void>
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
  Message& operator = (Message&&) noexcept = default;

  /**
   * @brief Get a modifiable reference to the object that
   * represents the header section
   *
   * @return Modifiable reference to the header object
   */
  template<typename = void>
  Header& header() noexcept;

  /**
   * @brief Get a read-only reference to the object that
   * represents the header section
   *
   * @return Read-only reference to the header object
   */
  template<typename = void>
  const Header& header() const noexcept;

  /**
   * @brief Add an entity to the message
   *
   * @param view:
   * A view of the entity to be sent with the message
   *
   * @return The object that invoked this method
   */
  template<typename = void>
  Message& add_body(const std::experimental::string_view view);

  /**
   * @brief Append data to the entity of the message
   *
   * @param view:
   * A view of the data to append to the entity of the message
   *
   * @return The object that invoked this method
   */
  template<typename = void>
  Message& append_body(const std::experimental::string_view view);

  /**
   * @brief Get a read-only reference to the entity in
   * this the message
   *
   * @return A read-only reference to the entity in
   * this the message
   */
  template<typename = void>
  std::experimental::string_view body() const noexcept;

  /**
   * @brief Remove the entity from the message
   *
   * @return The object that invoked this method
   */
  template<typename = void>
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
  Header header_fields_;
  std::string content_length_;
  std::experimental::string_view message_body_;

  std::string extended_body_;
  //------------------------------
}; //< class Message

/**--v----------- Implementation Details -----------v--**/

template<typename>
inline Message::Message() {}

template<typename>
inline Message::Message(const Limit limit) noexcept:
  header_fields_{limit}
{}

template<typename>
inline Header& Message::header() noexcept {
  return header_fields_;
}

template<typename>
inline const Header& Message::header() const noexcept {
  return header_fields_;
}

template<typename>
inline Message& Message::add_body(const std::experimental::string_view view) {
  if (view.empty()) return *this;
  message_body_   = view;
  content_length_ = std::to_string(message_body_.length());
  header_fields_.add_field(header_fields::content_length, content_length_);
  return *this;
}

template<typename>
inline Message& Message::append_body(const std::experimental::string_view view) {
  if (view.empty()) return *this;
  //-----------------------------------
  extended_body_ = message_body_.to_string();
  extended_body_.insert(extended_body_.cend(), view.data(), view.data() + view.length());
  message_body_ = extended_body_;
  content_length_ = std::to_string(message_body_.length());
  //-----------------------------------
  header_fields_.set_field(header_fields::content_length, content_length_);
  return *this;
}

template<typename>
inline std::experimental::string_view Message::body() const noexcept {
  return message_body_;
}

template<typename>
inline Message& Message::clear_body() noexcept {
  message_body_ = {};
  extended_body_.clear();
  header_fields_.erase(header_fields::content_length);
  return *this;
}

inline Message& Message::reset() noexcept {
  header_fields_.clear();
  return clear_body();
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
