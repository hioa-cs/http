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

#include <cctype>
#include <utility>
#include <ostream>
#include <sstream>
#include <algorithm>
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
private:
  //-----------------------------------------------
  // Internal class type aliases
  using Const_iterator = Header_set::const_iterator;
  //-----------------------------------------------
public:
  /**
   * @brief Default constructor that limits the amount
   * of fields that can be added to 25
   */
  explicit Header() noexcept;

  /**
   * @brief Constructor to specify the limit of how many
   * fields that can be added to an instance of this class
   *
   * @param limit:
   * Capacity of how many fields that can be added
   */
  explicit Header(const Limit limit) noexcept;

  /**
   * @brief Constructor that takes a stream of characters
   * as a {std::string} object and parses it into a set
   * of name-value pairs
   *
   * @tparam T header_data:
   * The character stream of data to parse
   *
   * @param limit:
   * Capacity of how many fields can be added
   */
  template
  <
    typename T,
    typename = std::enable_if_t
               <std::is_same
               <std::string, std::remove_const_t
               <std::remove_reference_t<T>>>::value>
  >
  explicit Header(T&& header_data, const Limit limit = 25);

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
  Header& operator = (Header&&) = default;

  /**
   * @brief Set the limit of how many fields can be added
   *
   * @param limit:
   * Capacity of how many fields can be added
   */
  void set_limit(const Limit limit) noexcept;

  /**
   * @brief Get the current capacity
   *
   * @return The current capacity of the set
   */
  Limit get_limit() const noexcept;

  /**
   * @brief Add a new field to the current set
   *
   * @tparam F field:
   * The name of the field
   *
   * @tparam V value:
   * The field value
   *
   * @return true if the field was added, false otherwise
   */
  template
  <
    typename F, typename V,
    typename = std::enable_if_t
               <std::is_same
               <std::string, std::remove_const_t
               <std::remove_reference_t<F>>>::value and
                std::is_same
               <std::string, std::remove_const_t
               <std::remove_reference_t<V>>>::value>
  >
  bool add_field(F&& field, V&& value);

  /**
   * @brief Add a set of fields to the current set from
   * a {std::string} object in the following format:
   *
   * "name: value\r\n"
   * "name: value\r\n"
   * ...
   *
   * @tparam D data - The set of fields to add
   */
  template
  <
    typename D,
    typename = std::enable_if_t
               <std::is_same
               <std::string, std::remove_const_t
               <std::remove_reference_t<D>>>::value>
  >
  void add_fields(D&& data);

  /**
   * @brief Change the value of the specified field
   *
   * If the field is absent from the set it will
   * be added with the associated value once its
   * within capacity
   *
   * @tparam F field:
   * The name of the field

   * @tparam V value:
   * The field value
   *
   * @return true if successful, false otherwise
   */
  template
  <
    typename F, typename V,
    typename = std::enable_if_t
               <std::is_same
               <std::string, std::remove_const_t
               <std::remove_reference_t<F>>>::value and
                std::is_same
               <std::string, std::remove_const_t
               <std::remove_reference_t<V>>>::value>
  >
  bool set_field(F&& field, V&& value);

  /**
   * @brief Get the value associated with a field
   *
   * Should call <has_field> before calling this
   *
   * @tparam F field:
   * The name of the field
   *
   * @return The value associated with the specified
   * field name
   */
  template
  <
    typename F,
    typename = std::enable_if_t
               <std::is_same
               <std::string, std::remove_const_t
               <std::remove_reference_t<F>>>::value>
  >
  const std::string& get_value(F&& field) const noexcept;

  /**
   * @brief Check to see if the specified field is a
   * member of the set of fields
   *
   * @tparam F field:
   * The name of the field
   *
   * @return true if the field is a member, false otherwise
   */
  template
  <
    typename F,
    typename = std::enable_if_t
               <std::is_same
               <std::string, std::remove_const_t
               <std::remove_reference_t<F>>>::value>
  >
  bool has_field(F&& field) const noexcept;

  /**
   * @brief Check to see if the set of fields is empty
   *
   * @return true if there are no fields within the set,
   * false otherwise
   */
  bool is_empty() const noexcept;

  /**
   * @brief Check to see how many fields are currently
   * in the set of fields
   *
   * @return The amount of fields currently in the set
   */
  Limit size() const noexcept;

  /**
   * @brief Remove all fields from the set of fields with the
   * specified name
   *
   * @tparam F field:
   * The name of the field to remove
   */
  template
  <
    typename F,
    typename = std::enable_if_t
               <std::is_same
               <std::string, std::remove_const_t
               <std::remove_reference_t<F>>>::value>
  >
  void erase(F&& field) noexcept;

  /**
   * @brief Remove all fields from the set of fields leaving
   * it empty
   */
  void clear() noexcept;

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
  //-----------------------------------------------
  // Class data members
  Header_set fields_;
  //-----------------------------------------------

  /**
   * @brief Find the location of a field within the set of
   * fields
   *
   * @tparam F field:
   * The field name to locate a field within the set of fields
   *
   * @return Iterator to the location of the field, else
   * location to the end of the sequence
   */
  template
  <
    typename F,
    typename = std::enable_if_t
               <std::is_same
               <std::string, std::remove_const_t
               <std::remove_reference_t<F>>>::value>
  >
  Const_iterator find(F&& field) const noexcept;

  /**
   * @brief Operator to stream the contents of the set of fields
   * into the specified output device
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
  friend std::ostream& operator << (std::ostream& output_device, const Header& header);
}; //< class Header

/**--v----------- Implementation Details -----------v--**/

inline Header::Header() noexcept {
  set_limit(25);
}

inline Header::Header(const Limit limit) noexcept {
  set_limit(limit);
}

template <typename T, typename>
inline Header::Header(T&& header_data, const Limit limit)
  : Header {limit}
{
  add_fields(header_data);
}

inline void Header::set_limit(const Limit limit) noexcept {
  fields_.reserve(limit);
}

inline Limit Header::get_limit() const noexcept {
  return fields_.capacity();
}

template <typename Field, typename Value, typename>
inline bool Header::add_field(Field&& field, Value&& value) {
  if (field.empty()) return false;
  //-----------------------------------
  if (size() < fields_.capacity()) {
    fields_.emplace_back(std::forward<Field>(field), std::forward<Value>(value));
    return true;
  }
  //-----------------------------------
  return false;
}

template <typename Data, typename>
inline void Header::add_fields(Data&& data) {
  if (data.empty()) return;
  //-----------------------------------
  auto iterator = data.cbegin();
  auto sentinel = data.cend();
  //-----------------------------------
  std::string field;
  std::string value;
  field.reserve(24);
  value.reserve(64);
  //-----------------------------------
  Limit limit {0};
  int character = *iterator;
  const int stop_char = std::char_traits<std::string::value_type>::eof();
  //-----------------------------------
  while (iterator not_eq sentinel
         and character not_eq stop_char
         and limit < fields_.capacity())
  {
    field.clear();
    value.clear();
    //-----------------------------------
    while (iterator not_eq sentinel and isspace(character)) {
      character = *++iterator;
    }
    //-----------------------------------
    while (iterator not_eq sentinel
           and character not_eq stop_char
           and character not_eq ':'
           and not iscntrl(character)
           and not isspace(character))
    {
      field += character;
      character = *++iterator;
    }
    //-----------------------------------
    while (iterator not_eq sentinel and isspace(character)) {
      character = *++iterator;
    }
    //-----------------------------------
    if (character not_eq ':') return;
    //-----------------------------------
    if (iterator not_eq sentinel) character = *++iterator;
    //-----------------------------------
    while (iterator not_eq sentinel and isspace(character)) {
      character = *++iterator;
    }
    //-----------------------------------
parse_value:
    while (iterator not_eq sentinel
           and character not_eq stop_char
           and not iscntrl(character)
           and character not_eq '\r'
           and character not_eq '\n')
    {
      value += character;
      character = *++iterator;
    }
    //-----------------------------------
    int lws_count {0};
    //-----------------------------------
    while (iterator not_eq sentinel
           and (character == '\r' || character == '\n'))
    {
      character = *++iterator;
      ++lws_count;
    }
    //-----------------------------------
    if (lws_count == 3) break;
    //-----------------------------------
    while (iterator not_eq sentinel and isspace(character)) {
      character = *++iterator;
      //-----------------------------------
      if (iterator not_eq sentinel
          and ((iterator + 1) not_eq sentinel)
          and isspace(*(iterator + 1)))
        continue;
      //-----------------------------------
      goto parse_value;
    }
    //-----------------------------------
    add_field(field, value);
    //-----------------------------------
    ++limit;
    //-----------------------------------
  }
}

template <typename Field, typename Value, typename>
inline bool Header::set_field(Field&& field, Value&& value) {
  if (field.empty() || value.empty()) return false;
  //-----------------------------------
  auto target = find(field);
  //-----------------------------------
  if (target not_eq fields_.end()) {
    const_cast<std::string&>((*target).second) = std::forward<Value>(value);
    return true;
  }
  else return add_field(std::forward<Field>(field), std::forward<Value>(value));
}

template <typename Field, typename>
inline const std::string& Header::get_value(Field&& field) const noexcept {
  if (field.empty()) return field;
  //-----------------------------------
  return find(std::forward<Field>(field))->second;
}

template <typename Field, typename>
inline bool Header::has_field(Field&& field) const noexcept {
  if (field.empty()) return false;
  //-----------------------------------
  return find(std::forward<Field>(field)) not_eq fields_.end();
}

inline bool Header::is_empty() const noexcept {
  return fields_.empty();
}

inline Limit Header::size() const noexcept {
  return fields_.size();
}

template <typename Field, typename>
inline void Header::erase(Field&& field) noexcept {
  if (field.empty()) return;
  //-----------------------------------
  auto target = find(std::forward<Field>(field));
  //-----------------------------------
  if (target not_eq fields_.end()) fields_.erase(target);
}

inline void Header::clear() noexcept {
  fields_.clear();
}

inline static std::string string_to_lower_case(std::string s) {
  std::transform(s.begin(), s.end(), s.begin(), ::tolower);
  return s;
}

template <typename Field, typename>
inline Header::Const_iterator Header::find(Field&& field) const noexcept {
  if (field.empty()) return fields_.end();
  //-----------------------------------
  return
  std::find_if(fields_.begin(), fields_.end(), [&field](const auto& f) {
    return string_to_lower_case(f.first) == string_to_lower_case(field);
  });
}

inline std::string Header::to_string() const {
  std::ostringstream header;
  //-----------------------------------
  for (const auto& field : fields_) {
    header << field.first << ": " << field.second << "\r\n";
  }
  //-----------------------------------
  header << "\r\n";
  //-----------------------------------
  return header.str();
}

inline Header::operator std::string () const {
  return to_string();
}

inline std::ostream& operator << (std::ostream& output_device, const Header& header) {
  return output_device << header.to_string();
}

/**--^----------- Implementation Details -----------^--**/

} //< namespace http

#endif //< HTTP_HEADER_HPP
