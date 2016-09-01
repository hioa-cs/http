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

#ifndef HTTP2_FRAME_HEADER_HPP
#define HTTP2_FRAME_HEADER_HPP

#include <cstdint>
#include <ostream>
#include <stdexcept>

namespace http2 {

/**
 * @brief This enum consist of the mappings between
 * frame labels to their respective codes
 */
enum class Type : uint8_t {
  DATA,
  HEADERS,
  PRIORITY,
  RST_STREAM,
  SETTINGS,
  PUSH_PROMISE,
  PING,
  GOAWAY,
  WINDOW_UPDATE,
  CONTINUATION,
}; //< enum Type

/**
 * @brief This enum consist of the valid flags that can be set
 * on a frame
 */
enum Flag : uint8_t {
  NONE        = 0x00,
  ACK         = 0x01,
  END_STREAM  = 0x01,
  END_HEADERS = 0x04,
  PADDED      = 0x08,
  PRIORITY    = 0x20
}; //< enum Flag

/**
 * @brief This class is used to represent a frame header
 * in the HTTP/2 protocol
 */
class Frame_header {
public:
  /**
   * @brief Constructor
   *
   * @param length
   * The size of the frame's payload

   * @param type
   * The type of frame

   * @param flags
   * The flags to set on the frame

   * @param id
   * The id of the associated stream
   *
   * @note Throws {Frame_header_error} if length > 16777215 (protocol's max payload size)
   * @note Throws {Frame_type_error} if an unknown type is specified
   */
  Frame_header(const uint32_t length, const Type     type,
               const uint8_t  flags,  const uint32_t sid);

  /**
   * @brief Get the size of the payload
   *
   * @return The size of the payload
   */
  uint32_t length() const noexcept
  { return length_; }

  /**
   * @brief Set the size of the payload
   *
   * @note Throws {Frame_header_error} if length > 16777215 (protocol's max payload length)
   *
   * @param length
   * The size of the frame's payload
   *
   * @return The object that invoked this method
   */
  Frame_header& set_length(const uint32_t length);

  /**
   * @brief Get the type of frame
   *
   * @return The type of frame
   */
  Type type() const noexcept
  { return type_; }

  /**
   * @brief Set the type of frame
   *
   * @param type
   * The type of frame
   *
   * @note Throws {Frame_type_error} if an unknown type is specified
   *
   * @return The object that invoked this method
   */
  Frame_header& set_type(const Type type) noexcept
  { validate_frame_type(type); type_ = type; return *this; }

  /**
   * @brief Get the flags set on the frame
   *
   * @return The flags set on the frame
   */
  uint8_t flags() const noexcept
  { return flags_; }

  /**
   * @brief Set the flags set on the frame
   *
   * @param flags
   * The flags to set on the frame
   *
   * @return The object that invoked this method
   */
  Frame_header& set_flags(const uint8_t flags) noexcept
  { flags_ = flags; return *this; }

  /**
   * @brief Get the id of the stream this frame
   * is associated with
   *
   * @return The id of the stream this frame
   * is associated with
   */
  uint32_t sid() const noexcept
  { return sid_; }

  /**
   * @brief Set the SID of the stream this frame
   * is associated with
   *
   * @param sid
   * The frame's associated stream id
   *
   * @return The object that invoked this method
   */
  Frame_header& set_sid(const uint32_t sid) noexcept
  { sid_ = (sid & 0x7fffffff); return *this; }
private:
  //------------------------------
  // Class data members
  uint32_t length_;
  uint32_t sid_;
  uint8_t  flags_;
  Type     type_;

  static constexpr uint32_t MAX_FRAME_SIZE {16777215};
  //------------------------------

  /**
   * @brief Make sure that the frame type is valid
   *
   * @param type
   * The type to validate
   *
   * @return The object that invoked this method
   */
  Frame_header& validate_frame_type(const Type type);
}; //< class Frame_header

/**
 * @brief This class is used to represent an error when an
 * unknown frame type is encountered
 */
class Frame_type_error : public std::runtime_error {
  using runtime_error::runtime_error;
};

/**
 * @brief This class is used to represent an error that occurred
 * from within the operations of class Frame_header
 */
class Frame_header_error : public std::runtime_error {
  using runtime_error::runtime_error;
};

/**--v----------- Implementation Details -----------v--**/

inline Frame_header::Frame_header(const uint32_t length, const Type     type,
                                  const uint8_t  flags,  const uint32_t sid)
{
  set_length(length).validate_frame_type(type).set_sid(sid);

  type_  = type;
  flags_ = flags;
}

inline Frame_header& Frame_header::set_length(const uint32_t length)
{
  if (length > MAX_FRAME_SIZE) {
    throw Frame_header_error {"The length specified exceeds the protocol's max limit"};
  }

  length_ = length;

  return *this;
}


inline Frame_header& Frame_header::validate_frame_type(const Type type) {
  switch (type) {
    case Type::DATA:
    case Type::HEADERS:
    case Type::PRIORITY:
    case Type::RST_STREAM:
    case Type::SETTINGS:
    case Type::PUSH_PROMISE:
    case Type::PING:
    case Type::GOAWAY:
    case Type::WINDOW_UPDATE:
    case Type::CONTINUATION:  return *this;
    default: throw Frame_type_error {"Unkown frame type"};
  }
}

/**
 * @brief Operator to stream a frame header type into the specified
 * output device
 *
 * @param output_device:
 * The output device to stream a frame header type into
 *
 * @param type:
 * A frame header type
 *
 * @return Reference to the specified output device
 */
inline std::ostream& operator << (std::ostream& output_device, const Type type) {
  switch (type) {
    case Type::DATA:          return output_device << "DATA";
    case Type::HEADERS:       return output_device << "HEADERS";
    case Type::PRIORITY:      return output_device << "PRIORITY";
    case Type::RST_STREAM:    return output_device << "RST_STREAM";
    case Type::SETTINGS:      return output_device << "SETTINGS";
    case Type::PUSH_PROMISE:  return output_device << "PUSH_PROMISE";
    case Type::PING:          return output_device << "PING";
    case Type::GOAWAY:        return output_device << "GOAWAY";
    case Type::WINDOW_UPDATE: return output_device << "WINDOW_UPDATE";
    case Type::CONTINUATION:  return output_device << "CONTINUATION";
    default: throw Frame_type_error {"Unkown frame type"};
  }
}

/**--^----------- Implementation Details -----------^--**/

} //< namespace http2

#endif //< HTTP2_FRAME_HEADER_HPP
