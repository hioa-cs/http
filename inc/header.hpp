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

#ifndef HTTP_HEADER_HPP
#define HTTP_HEADER_HPP

#include <algorithm>
#include <cctype>
#include <sstream>
#include <type_traits>

#include "common.hpp"
#include "header_fields.hpp" //< Standard header field names

namespace http {

/**
 * @brief This class is used to store header information
 * associated with an HTTP message
 *
 * By default it is limited to 25 fields but the amount
 * can be specified by using the appropriate constructor
 * and provided method
 */
class Header {
public:
  /**
   * @brief Default constructor that limits the amount
   * of fields that can be added to 25
   */
  template<typename = void>
  explicit Header() noexcept;

  /**
   * @brief Constructor to specify the limit of how many
   * fields that can be added to an instance of this class
   *
   * @param limit:
   * Capacity of how many fields that can be added
   */
  template<typename = void>
  explicit Header(const Limit limit) noexcept;

  /**
   * @brief Constructor that takes a stream of characters
   * and parses it into a set of name-value pairs
   *
   * @param header_data:
   * The character stream of data to parse
   *
   * @param limit:
   * Capacity of how many fields can be added
   */
  template<typename = void>
  explicit Header(const std::experimental::string_view header_data, const Limit limit = 25);

  /**
   * Default destructor
   */
  ~Header() noexcept = default;

  /**
   * Default copy constructor
   */
  Header(const Header&) = default;

  /**
   * Default move constructor
   */
  Header(Header&&) noexcept = default;

  /**
   * Default assignment operator
   */
  Header& operator = (const Header&) = default;

  /**
   * Default move assignemt operator
   */
  Header& operator = (Header&&) noexcept = default;

  /**
   * @brief Get the current capacity
   *
   * @return The current capacity of the set
   */
  template<typename = void>
  Limit limit() const noexcept;

  /**
   * @brief Add a new field to the current set
   *
   * @param field:
   * The name of the field
   *
   * @param value:
   * The field value
   *
   * @return true if the field was added, false otherwise
   */
  template<typename = void>
  bool add_field(const std::experimental::string_view field, const std::experimental::string_view value);

  /**
   * @brief Add a set of fields to the current set from
   * a stream of characters in the following format:
   *
   * "name: value\r\n"
   * "name: value\r\n"
   * ...
   *
   * @param data
   * The set of fields to add
   */
  void add_fields(const std::experimental::string_view data);

  /**
   * @brief Change the value of the specified field
   *
   * If the field is absent it will be added with
   * the associated value once its within capacity
   *
   * @param field:
   * The name of the field

   * @param value:
   * The field value
   *
   * @return true if successful, false otherwise
   */
  template<typename = void>
  bool set_field(const std::experimental::string_view field, const std::experimental::string_view value);

  /**
   * @brief Get the value associated with a field
   *
   * @param field:
   * The name of the field
   *
   * @return The value associated with the specified
   * field name if exist, empty view otherwise
   */
  template<typename = void>
  std::experimental::string_view value(const std::experimental::string_view field) const noexcept;

  /**
   * @brief Check to see if the specified field is present
   *
   * @param field:
   * The name of the field
   *
   * @return true if the field is a member, false otherwise
   */
  template<typename = void>
  bool has_field(const std::experimental::string_view field) const noexcept;

  /**
   * @brief Check to see if there are no fields present
   *
   * @return true if there are no fields present,
   * false otherwise
   */
  template<typename = void>
  bool is_empty() const noexcept;

  /**
   * @brief Check to see how many fields are currently
   * present
   *
   * @return The amount of fields currently present
   */
  template<typename = void>
  Limit size() const noexcept;

  /**
   * @brief Remove all fields currently present with the
   * specified name
   *
   * @param field:
   * The name of the field to remove
   */
  template<typename = void>
  void erase(const std::experimental::string_view field) noexcept;

  /**
   * @brief Remove all fields leaving none present
   */
  template<typename = void>
  void clear() noexcept;

  /**
   * @brief Get a string representation of this
   * class
   *
   * @return A string representation
   */
  std::string to_string() const;

  /**
   * @brief Operator to transform this class
   * into string form
   */
  operator std::string () const;
private:
  //-----------------------------------------------
  // Class data members
  Header_set fields_;
  //-----------------------------------------------

  /**
   * @brief Set the limit of how many fields can be added
   *
   * @param limit:
   * Capacity of how many fields can be added
   */
  template<typename = void>
  void set_limit(const Limit limit) noexcept;

  /**
   * @brief Find the location of a field within the set of
   * fields
   *
   * @param field:
   * The field name to locate a field if present
   *
   * @return Iterator to the location of the field, else
   * location to the end of the sequence
   */
  template<typename = void>
  Header_set::const_iterator find(const std::experimental::string_view field) const noexcept;
};

/**--v----------- Implementation Details -----------v--**/

template<typename>
inline Header::Header() noexcept {
  set_limit(25U);
}

template<typename>
inline Header::Header(const Limit limit) noexcept {
  set_limit(limit);
}

template<typename>
inline Header::Header(const std::experimental::string_view header_data, const Limit limit)
  : Header {limit}
{
  add_fields(header_data);
}

template<typename>
inline Limit Header::limit() const noexcept {
  return fields_.capacity();
}

template<typename>
inline bool Header::add_field(const std::experimental::string_view field, const std::experimental::string_view value) {
  if ((not field.empty()) and (size() < limit())) {
    fields_.emplace_back(field, value);
    return true;
  }

  return false;
}

inline void Header::add_fields(std::experimental::string_view fields_view) {
  if (fields_view.empty() or (size() == limit())) return;
  //-----------------------------------
  std::experimental::string_view field {};
  std::experimental::string_view value {};
  std::experimental::string_view::size_type base {0U};
  std::experimental::string_view::size_type break_point {};
  //-----------------------------------
  fields_view.remove_prefix(fields_view.find_first_not_of(' '));
  //-----------------------------------
  while (size() < limit()) {
    if ((break_point = fields_view.find(':')) not_eq std::experimental::string_view::npos) {
      field = fields_view.substr(base, break_point);
      //-----------------------------------
      fields_view.remove_prefix(field.length() + 1U);
      fields_view.remove_prefix(fields_view.find_first_not_of(' '));
    }
    else {
      break;
    }

    if ((break_point = fields_view.find("\r\n")) not_eq std::experimental::string_view::npos) {
      value = fields_view.substr(base, break_point);
      fields_.emplace_back(field, value);
      fields_view.remove_prefix(value.length() + 2U);
    }
    else if ((break_point = fields_view.find('\n')) not_eq std::experimental::string_view::npos) {
      value = fields_view.substr(base, break_point);
      fields_.emplace_back(field, value);
      fields_view.remove_prefix(value.length() + 1U);
    }

    if (fields_view[0] == '\r' or fields_view[0] == '\n') {
      break;
    }
  }
}

template<typename>
inline bool Header::set_field(const std::experimental::string_view field, const std::experimental::string_view value) {
  if (field.empty() || value.empty()) return false;
  //-----------------------------------
  const auto target = find(field);
  //-----------------------------------
  if (target not_eq fields_.cend()) {
    const_cast<std::experimental::string_view&>(target->second) = value;
    return true;
  }
  else return add_field(field, value);
}

template<typename>
inline std::experimental::string_view Header::value(const std::experimental::string_view field) const noexcept {
  if (field.empty()) return field;
  const auto it = find(field);
  return (it not_eq fields_.cend()) ? it->second : std::experimental::string_view{};
}

template<typename>
inline bool Header::has_field(const std::experimental::string_view field) const noexcept {
  if (field.empty()) return false;
  //-----------------------------------
  return find(field) not_eq fields_.cend();
}

template<typename>
inline bool Header::is_empty() const noexcept {
  return fields_.empty();
}

template<typename>
inline Limit Header::size() const noexcept {
  return fields_.size();
}

template<typename>
inline void Header::erase(const std::experimental::string_view field) noexcept {
  if (field.empty()) return;
  //-----------------------------------
  Header_set::const_iterator target;
  while ((target = find(field)) not_eq fields_.cend()) {
    fields_.erase(target);
  }
}

template<typename>
inline void Header::clear() noexcept {
  fields_.clear();
}

inline std::string Header::to_string() const {
  if (size()) {
    std::ostringstream header;
    //-----------------------------------
    for (const auto& field : fields_) {
      header << field.first << ": " << field.second << "\r\n";
    }
    header << "\r\n";
    //-----------------------------------
    return header.str();
  }

  return {};
}

inline Header::operator std::string () const {
  return to_string();
}

template<typename>
inline void Header::set_limit(const Limit limit) noexcept {
  fields_.reserve(limit);
}

template<typename>
inline Header_set::const_iterator Header::find(const std::experimental::string_view field) const noexcept {
  if (field.empty()) return fields_.cend();
  //-----------------------------------
  return
    std::find_if(fields_.cbegin(), fields_.cend(), [&field](const auto __) {
      return (__.first.length() == field.length())
        and std::equal(__.first.data(), __.first.data() + __.first.length(), field.data(), [](const auto a, const auto b) {
          return std::tolower(a) == std::tolower(b);
        });
    });
}

/**
 * @brief Operator to stream the of this class into the
 * specified output device
 *
 * The output format is as follows:
 * field : value "\r\n"
 * field : value "\r\n"
 * ...
 * "\r\n"
 *
 * @param output_device:
 * The output device to stream the contents from an instance of this
 * class into
 *
 * @param header:
 * An instance of this class
 *
 * @return Reference to the specified output device
 */
inline std::ostream& operator << (std::ostream& output_device, const Header& header) {
  return output_device << header.to_string();
}

/**--^----------- Implementation Details -----------^--**/

} //< namespace http

#endif //< HTTP_HEADER_HPP
