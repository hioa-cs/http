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

#include <list>
#include <cctype>
#include <string>
#include <utility>
#include <ostream>
#include <algorithm>
#include <type_traits>

#include "header_fields.hpp" //< Standard header field names

namespace http {

//-----------------------------------------------
// This class is used to store header information
// associated with an http message
//
// By default it is limited to 100 fields
// but the amount can be specified by using the
// appropriate constructor and provided method.
//-----------------------------------------------
class Header {
private:
  //-----------------------------------------------
  // Internal class type aliases
  //-----------------------------------------------
  using Map_Type       = std::pair<std::string, std::string>;
  using Field_Map      = std::list<Map_Type>;
  using Limit          = Field_Map::size_type;
  using Const_Iterator = Field_Map::const_iterator;
  //-----------------------------------------------
public:
  //-----------------------------------------------
  // Default constructor that limits the amount
  // of fields that can be added to 100
  //-----------------------------------------------
  explicit Header() noexcept = default;

  //-----------------------------------------------
  // Constructor to specify the limit of how many
  // fields that can be added
  //
  // @param limit - Capacity of how many fields can
  //                be added
  //-----------------------------------------------
  explicit Header(const Limit limit) noexcept;
  
  //-----------------------------------------------
  // Constructor that takes a stream of characters
  // as a <std::string> object and parses it into a
  // set of name-value pairs
  //
  // @tparam (std::string) header_data - The character stream of data
  //                                     to parse
  //
  // @param limit - Capacity of how many fields can
  //                be added
  //-----------------------------------------------
  template <typename Header_Data>
  explicit Header(Header_Data&& header_data, const Limit limit = 100);

  //-----------------------------------------------
  // Default destructor
  //-----------------------------------------------
  ~Header() noexcept = default;

  //-----------------------------------------------
  // Default copy constructor
  //-----------------------------------------------
  Header(const Header&) = default;

  //-----------------------------------------------
  // Default move constructor
  //-----------------------------------------------
  Header(Header&&) noexcept = default;

  //-----------------------------------------------
  // Default assignment operator
  //-----------------------------------------------
  Header& operator = (const Header&) = default;

  //-----------------------------------------------
  // Default move assignemt operator
  //-----------------------------------------------
  Header& operator = (Header&&) = default;

  //-----------------------------------------------
  // Set the limit of how many fields can be added
  //
  // @param limit - Capacity of how many fields can
  //                be added
  //-----------------------------------------------
  void set_limit(const Limit limit) noexcept;

  //-----------------------------------------------
  // Get the current capacity
  //
  // @return - The current capacity of the set
  //-----------------------------------------------
  Limit get_limit() const noexcept;

  //-----------------------------------------------
  // Add a new field to the current set
  //
  // @tparam (std::string) field - The field name
  // @tparam (std::string) value - The field value
  //
  // @return - true if the field was added, false
  //           otherwise
  //-----------------------------------------------
  template <typename Field, typename Value>
  bool add_field(Field&& field, Value&& value);

  //-----------------------------------------------
  // Change the value of the specified field
  //
  // If the field is absent from the set it will
  // be added with the associated value once its
  // within capacity
  //
  // @tparam (std::string) field - The field name
  // @tparam (std::string) value - The field value
  //
  // @return - true if successful, false otherwise
  //-----------------------------------------------
  template <typename Field, typename Value>
  bool set_field(Field&& field, Value&& value);

  //-----------------------------------------------
  // Get the value associated with a field
  //
  // Should call <has_field> before using this
  //
  // @tparam (std::string) field - The field name
  //
  // @return - The associated field value
  //-----------------------------------------------
  template <typename Field>
  const std::string& get_value(Field&& field) const noexcept;

  //-----------------------------------------------
  // Check to see if the specified field is a
  // member of the set
  //
  // @tparam (std::string) field - The field name
  //
  // @return - true if the field is a member,
  //           false otherwise
  //-----------------------------------------------
  template <typename Field>
  bool has_field(Field&& field) const noexcept;

  //-----------------------------------------------
  // Check to see if the set is empty
  //
  // @return - true if there are no fields within
  //           the set, false otherwise
  //-----------------------------------------------
  bool is_empty() const noexcept;

  //-----------------------------------------------
  // Check to see how many fields are currently
  // in the set
  //
  // @return - The amount of fields currently in
  //           the set
  //-----------------------------------------------
  Limit size() const noexcept;

  //-----------------------------------------------
  // Remove all fields from the set with the
  // specified name
  //
  // @tparam (std::string) field - The field name to remove
  //-----------------------------------------------
  template <typename Field>
  void erase(Field&& field) noexcept;

  //-----------------------------------------------
  // Remove all fields from the set leaving it
  // empty
  //-----------------------------------------------
  void clear() noexcept;
private:
  //-----------------------------------------------
  // Class data members
  //-----------------------------------------------
  Field_Map map_;
  Limit     limit_ {100};

  //-----------------------------------------------
  // Find the location of a field within the set
  //
  // @tparam (std::string) field - The field name to locate
  //
  // @return - Iterator to the location of the field,
  //           else location to the end of the
  //           sequence
  //-----------------------------------------------
  template <typename Field>
  Const_Iterator find(Field&& field) const noexcept;

  //-----------------------------------------------
  // Operator to stream the contents of the set
  // into the specified output stream device
  //
  // The format is as follows:
  // field : value "\r\n"
  //-----------------------------------------------
  friend std::ostream& operator << (std::ostream&, const Header&);
}; //< class Header

inline Header::Header(const Limit limit) noexcept {
  if (limit <= 0) return;
  limit_ = limit;
}

template <typename Header_Data>
inline Header::Header(Header_Data&& header_data, const Limit lim) :
  Header{lim}
{
  if (header_data.empty()) return;
  //-----------------------------------
  auto iterator = header_data.cbegin();
  auto sentinel = header_data.cend();
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
         and limit < limit_)
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

inline void Header::set_limit(const Limit limit) noexcept {
  if (limit <= 0) return;
  limit_ = limit;
}

inline Header::Limit Header::get_limit() const noexcept {
  return limit_;
}

template <typename Field, typename Value>
inline bool Header::add_field(Field&& field, Value&& value) {
  if (field.empty() || value.empty()) return false;
  //-----------------------------------
  if (size() < limit_) {
    map_.emplace_back(std::forward<Field>(field), std::forward<Value>(value));
    return true;
  }
  //-----------------------------------
  return false;
}

template <typename Field, typename Value>
inline bool Header::set_field(Field&& field, Value&& value) {
  if (field.empty() || value.empty()) return false;
  //-----------------------------------
  auto target = find(std::forward<Field>(field));
  //-----------------------------------
  if (target not_eq map_.end()) {
    const_cast<std::string&>((*target).second) = std::forward<Value>(value);
    return true;
  }
  else return add_field(std::forward<Field>(field), std::forward<Value>(value));
}

template <typename Field>
inline const std::string& Header::get_value(Field&& field) const noexcept {
  if (field.empty()) return field;
  //-----------------------------------
  return find(std::forward<Field>(field))->second;
}

template <typename Field>
inline bool Header::has_field(Field&& field) const noexcept {
  if (field.empty()) return false;
  //-----------------------------------
  return find(std::forward<Field>(field)) not_eq map_.end();
}

inline bool Header::is_empty() const noexcept {
  return map_.empty();
}

inline Header::Limit Header::size() const noexcept {
  return map_.size();
}

template <typename Field>
inline void Header::erase(Field&& field) noexcept {
  if (field.empty()) return;
  //-----------------------------------
  auto target = find(std::forward<Field>(field));
  //-----------------------------------
  if (target not_eq map_.end()) map_.erase(target);
}

inline void Header::clear() noexcept {
  map_.clear();
}

static std::string string_to_lower_case(const std::string& string) {
  std::string output_value {string};
  //-----------------------------------
  std::transform(output_value.begin(), output_value.end(),
                 output_value.begin(), tolower);
  //-----------------------------------
  return output_value;
}

template <typename Field>
inline Header::Const_Iterator Header::find(Field&& field) const noexcept {
  if (field.empty()) return map_.end();
  //-----------------------------------
  return
  std::find_if(map_.begin(), map_.end(), [&field](const Header::Map_Type& f){
    return string_to_lower_case(f.first) == string_to_lower_case(field);
  });
}

std::ostream& operator << (std::ostream& output_device, const Header& header) {
  auto iterator = header.map_.cbegin();
  //-----------------------------------
  while (iterator not_eq header.map_.cend()) {
    output_device << iterator->first << ": " << iterator->second << "\r\n";
    ++iterator;
  }
  //-----------------------------------
  return output_device << "\r\n";
}

} //< namespace http

#endif //< HTTP_HEADER_HPP
