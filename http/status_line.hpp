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

#ifndef HTTP_STATUS_LINE_HPP
#define HTTP_STATUS_LINE_HPP

#include <ostream>

#include "version.hpp"
#include "status_codes.hpp"

namespace http {

//------------------------------
// This class respresents a 
// response message status-line
//------------------------------
class Status_Line {
private:
  //----------------------------
  // Internal class type aliases
  //----------------------------
  using Code = int;
  //----------------------------
public:
  //----------------------------
  // Constructor to create the status line
  // by supplying the version of the message
  // and the status code
  //
  // @param version - The version of the message
  // @param code    - The status code
  //----------------------------
  explicit constexpr Status_Line(const Version& version, const Code code) noexcept;

  //-----------------------------------
  // Constructor to construct a status-line
  // from the incoming character stream of
  // data which is a <std::string> object
  //
  // @tparam (std::string) response - The character stream of
  //                                 data
  //-----------------------------------
  template <typename Response>
  explicit Status_Line(Response&& response);

  //----------------------------
  // Default destructor
  //----------------------------
  ~Status_Line() noexcept = default;

  //----------------------------
  // Get the version of the message
  //
  // @return - Version of the message
  //----------------------------
  constexpr const Version& get_version() const noexcept;

  //----------------------------
  // Set the version of the message
  //
  // @param version - Version of the message
  //----------------------------
  void set_version(const Version& version) noexcept;

  //----------------------------
  // Get message status code
  //
  // @return - Status code of the message
  //----------------------------
  constexpr Code get_code() const noexcept;

  //----------------------------
  // Set the message status code
  //
  // @param code - Status code of the message
  //----------------------------
  void set_code(const Code code) noexcept;

  //----------------------------
  // Get a string representation of
  // this class
  //
  // @return - A string representation
  //----------------------------
  std::string to_string() const;

  //----------------------------
  // Operator to transform this class
  // into string form
  //----------------------------
  operator std::string () const;
  //---------------------------
private:
  //---------------------------
  // Class data members
  //---------------------------
  Version version_;
  Code    code_;

  //---------------------------
  // Deleted move and copy operations
  //---------------------------
  Status_Line(const Status_Line&) = delete;
  Status_Line(Status_Line&&) = delete;

  //----------------------------
  // Deleted move and copy assignment operations
  //----------------------------
  Status_Line& operator = (const Status_Line&) = delete;
  Status_Line& operator = (Status_Line&&) = delete;
}; //< class HTTP_Status_Line

inline constexpr Status_Line::Status_Line(const Version& version, const Code code) noexcept:
  version_{version},
  code_{code}
{}

template <typename Response>
Status_Line::Status_Line(Response&& response) {
  std::string start {response.substr(response.find_first_not_of("\f\t\v "))};
  //-----------------------------------
  std::string sl {start.substr(0, start.find("\r\n"))};
  //-----------------------------------
  auto version_data = sl.substr(sl.find_first_of("/") + 1);
  //-----------------------------------
  std::string major {version_data.substr(0, version_data.find("."))};
  std::string minor {version_data.substr(version_data.find(".") + 1),
                                         version_data.find_first_of(' ')};
  //-----------------------------------
  unsigned maj = static_cast<unsigned>(std::stoul(major));
  unsigned min = static_cast<unsigned>(std::stoul(minor));
  //-----------------------------------
  version_ = Version{maj, min};
  //-----------------------------------
  auto code = sl.substr(sl.find_first_of(' ') + 1, 3 /*<-(3) number of digits in code */);
  //-----------------------------------
  code_ = static_cast<int>(std::stoul(code));
  //-----------------------------------
  response = response.substr(response.find_first_of("\r\n") + 2);
}

inline constexpr const Version& Status_Line::get_version() const noexcept {
  return version_;
}

inline void Status_Line::set_version(const Version& version) noexcept {
  version_ = version;
}

inline constexpr Status_Line::Code Status_Line::get_code() const noexcept {
  return code_;
}

inline void Status_Line::set_code(const Code code) noexcept {
  code_ = code;
}

inline std::string Status_Line::to_string() const {
  return *this;
}

inline Status_Line::operator std::string () const {
  std::ostringstream status_info;
  //---------------------------
  status_info << version_.to_string()    << " "
              << code_                   << " "
              << code_description(code_) << "\r\n";
  //---------------------------
  return status_info.str();
}

inline std::ostream& operator << (std::ostream& output_device, const Status_Line& stat_line) {
  return output_device << stat_line.to_string();
}

} //< namespace http

#endif //< HTTP_STATUS_LINE_HPP
