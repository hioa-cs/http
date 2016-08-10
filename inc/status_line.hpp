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

#include <regex>

#include "version.hpp"
#include "status_codes.hpp"
#include "status_code_constants.hpp"

namespace http {

/**
 * @brief This class respresents a 
 * response message status-line
 */
class Status_line {
public:
  /**
   * @brief Constructor to create the status line
   * by supplying the version of the message
   * and the status code
   *
   * @param version:
   * The version of the message
   *
   * @param code:
   * The status code
   */
  explicit constexpr Status_line(const Version version, const Code code) noexcept;

  /**
   * @brief Constructor to construct a status-line
   * from the incoming character stream of
   * data which is a {std::string} object
   *
   * @tparam T response:
   * The character stream of data
   */
  template
  <
    typename T,
    typename = std::enable_if_t
               <std::is_same
               <std::string, std::remove_const_t
               <std::remove_reference_t<T>>>::value>
  >
  explicit Status_line(T&& response);

  /**
   * @brief Default copy constructor
   */
  Status_line(const Status_line&) noexcept = default;

  /**
   * @brief Default move constructor
   */
  Status_line(Status_line&&) noexcept = default;

  /**
   * @brief Default destructor
   */
  ~Status_line() noexcept = default;

  /**
   * @brief Default copy assignment operator
   */
  Status_line& operator = (const Status_line&) noexcept = default;

  /**
   * @brief Default move assignment operator
   */
  Status_line& operator = (Status_line&&) noexcept = default;

  /**
   * @brief Get the version of the message
   *
   * @return Version of the message
   */
  constexpr Version get_version() const noexcept;

  /**
   * @brief Set the version of the message
   *
   * @param version:
   * Version of the message
   */
  void set_version(const Version version) noexcept;

  /**
   * @brief Get message status code
   *
   * @return Status code of the message
   */
  constexpr Code get_code() const noexcept;

  /**
   * @brief Set the message status code
   *
   * @param code:
   * Status code of the message
   */
  void set_code(const Code code) noexcept;

  /**
   * @brief Get a string representation of
   * this class
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
  //---------------------------
  // Class data members
  Version version_;
  Code    code_;
  //---------------------------
}; //< class Status_line

/**
 * @brief This class is used to represent an error that occurred
 * from within the operations of class Status_line
 */
class Status_line_error : public std::runtime_error {
  using runtime_error::runtime_error;
};

/**--v----------- Implementation Details -----------v--**/

inline constexpr Status_line::Status_line(const Version version, const Code code) noexcept
  : version_{version}
  , code_{code}
{}

template <typename Response, typename>
inline Status_line::Status_line(Response&& response) {
  if (response.empty() or response.size() < 16 /*<-(16) minimum response length */) {
    throw Status_line_error {"Invalid response"};
  }

  bool is_canonical_line_ending {false};

  // Extract {Status-Line} from response
  std::string status_line;
  std::size_t index;

  if ((index = response.find("\r\n")) not_eq std::string::npos) {
    status_line = response.substr(0, index);
    is_canonical_line_ending = true;
  } else if ((index = response.find('\n')) not_eq std::string::npos) {
    status_line = response.substr(0, index);
  } else {
    throw Status_line_error {"Invalid line-ending"};
  }

  const static std::regex status_line_pattern
  {
    "HTTP/(\\d+)\\.(\\d+) " //< Protocol Version {Major.Minor}
    "(\\d{3}) "             //< Response Code
    "[a-z A-Z]+"            //< Response Code Description
  };

  std::smatch m;

  if (not std::regex_match(status_line, m, status_line_pattern)) {
    throw Status_line_error {"Invalid response line: " + status_line};
  }

  version_ = Version(std::stoi(m[1]), std::stoi(m[2]));

  code_ = std::stoi(m[3]);

  // Trim the response for further processing
  if (is_canonical_line_ending) {
    response = response.substr(index + 2);
  } else {
    response = response.substr(index + 1);
  }
}

inline constexpr Version Status_line::get_version() const noexcept {
  return version_;
}

inline void Status_line::set_version(const Version version) noexcept {
  version_ = version;
}

inline constexpr Code Status_line::get_code() const noexcept {
  return code_;
}

inline void Status_line::set_code(const Code code) noexcept {
  code_ = code;
}

inline std::string Status_line::to_string() const {
  std::ostringstream stat_line;
  //---------------------------
  stat_line << version_                << " "
            << code_                   << " "
            << code_description(code_) << "\r\n";
  //---------------------------
  return stat_line.str();
}

inline Status_line::operator std::string () const {
  return to_string();
}

inline std::ostream& operator << (std::ostream& output_device, const Status_line& stat_line) {
  return output_device << stat_line.to_string();
}

/**--^----------- Implementation Details -----------^--**/

} //< namespace http

#endif //< HTTP_STATUS_LINE_HPP
