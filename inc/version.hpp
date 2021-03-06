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

#ifndef HTTP_VERSION_HPP
#define HTTP_VERSION_HPP

#include <string>
#include <sstream>

namespace http {

/**
 * @brief This class represents the version
 * of an HTTP message
 */
class Version {
public:
  /**
   * @brief Constructor
   *
   * @param major:
   * The major version number

   * @param minor:
   * The minor version number
   */
  explicit constexpr Version(const unsigned major = 1, const unsigned minor = 1) noexcept;

  /**
   * @brief Default destructor
   */
  ~Version() noexcept = default;

  /**
   * @brief Default copy constructor
   */
  Version(const Version&) noexcept = default;

  /**
   * @brief Default move constructor
   */
  Version(Version&&) noexcept = default;

  /**
   * @brief Default assignment operator
   */
  Version& operator = (const Version&) noexcept = default;

  /**
   * @brief Default move assignment operator
   */
  Version& operator = (Version&&) noexcept = default;

  /**
   * @brief Get the major version number
   *
   * @return The major version number
   */
  constexpr unsigned get_major() const noexcept;

  /**
   * @brief Set the major version number
   *
   * @param major:
   * The major version number
   */
  void set_major(const unsigned major) noexcept;

  /**
   * @brief Get the minor version number
   *
   * @return The minor version number
   */
  constexpr unsigned get_minor() const noexcept;

  /**
   * @brief Set the minor version number
   *
   * @param minor:
   * The minor version number
   */
  void set_minor(const unsigned minor) noexcept;

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
  //-----------------------------------
  // Class data members
  unsigned major_;
  unsigned minor_;
  //-----------------------------------
}; //< class Version

/**--v----------- Implementation Details -----------v--**/

inline constexpr Version::Version(const unsigned major, const unsigned minor) noexcept
  : major_{major}
  , minor_{minor}
{}

inline constexpr unsigned Version::get_major() const noexcept {
  return major_;
}

inline void Version::set_major(const unsigned major) noexcept {
  major_ = major;
}

inline constexpr unsigned Version::get_minor() const noexcept {
  return minor_;
}

inline void Version::set_minor(const unsigned minor) noexcept {
  minor_ = minor;
}

inline std::string Version::to_string() const {
  std::ostringstream ver_data;
  //----------------------------
  ver_data << "HTTP/" << major_
           << "."     << minor_;
  //-----------------------------
  return ver_data.str();
}

inline Version::operator std::string () const {
  return to_string();
}

/**
 * @brief Operator to stream the contents of this class
 * into the specified output device
 *
 *
 * @param output_device:
 * The output device to stream the contents from an instance of this
 * class into
 *
 * @param version:
 * An instance of this class
 *
 * @return Reference to the specified output device
 */
inline std::ostream& operator << (std::ostream& output_device, const Version& version) {
  return output_device << version.to_string();
}

/**
 * @brief Operator to check for equality
 */
inline bool operator == (const Version& lhs, const Version& rhs) noexcept {
  return lhs.get_major() == rhs.get_major()
         and
         lhs.get_minor() == rhs.get_minor();
}

/**
 * @brief Operator to check for inequality
 */
inline bool operator != (const Version& lhs, const Version& rhs) noexcept {
  return not (lhs == rhs);
}

/**
 * @brief Operator to check for less than relationship
 */
inline bool operator < (const Version& lhs, const Version& rhs) noexcept {
  return lhs.get_major() < rhs.get_major()
         or
         lhs.get_minor() < rhs.get_minor();
}

/**
 * @brief Operator to check for greater than relationship
 */
inline bool operator > (const Version& lhs, const Version& rhs) noexcept {
  return lhs.get_major() > rhs.get_major()
         or
         lhs.get_minor() > rhs.get_minor();
}

/**
 * @brief Operator to check for less than or equal to relationship
 */
inline bool operator <= (const Version& lhs, const Version& rhs) noexcept {
  return (lhs < rhs) or (lhs == rhs);
}

/**
 * @brief Operator to check for greater than or equal to relationship
 */
inline bool operator >= (const Version& lhs, const Version& rhs) noexcept {
  return (lhs > rhs) or (lhs == rhs);
}

/**--^----------- Implementation Details -----------^--**/

} //< namespace http

#endif //< HTTP_VERSION_HPP
