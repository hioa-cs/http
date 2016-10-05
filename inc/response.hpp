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

#ifndef HTTP_RESPONSE_HPP
#define HTTP_RESPONSE_HPP

#include "message.hpp"
#include "status_line.hpp"

namespace http {

/**
 * @brief This class is used to represent
 * an HTTP response message
 */
class Response : public Message {
private:
  //------------------------------
  // Internal class type aliases
  using Code  = status_t;
  //------------------------------
public:
  /**
   * @brief Constructor to set up a response
   * by providing information for the
   * status line of the response message
   *
   * @param code:
   * The status code
   *
   * @param version:
   * The version of the message
   */
  template<typename = void>
  explicit Response(const Code code = OK, const Version version = Version{}) noexcept;

  /**
   * @brief Construct a response message from the incoming character
   * stream of data
   *
   * @param response:
   * The character stream of data
   *
   * @param len:
   * The length of the character stream
   *
   * @param limit:
   * Capacity of how many fields can be added to the header section
   */
  explicit Response(std::string response, const Limit limit = 25);

  /**
   * @brief  Default copy constructor
   */
  Response(const Response&) = default;

  /**
   * @brief Default move constructor
   */
  Response(Response&&) noexcept = default;

  /**
   * @brief Default destructor
   */
  ~Response() noexcept = default;

  /**
   * @brief Default copy assignment operator
   */
  Response& operator = (const Response&) = default;

  /**
   * @brief Default move assignment operator
   */
  Response& operator = (Response&&) = default;

  /**
   * @brief Get the status code of this message
   *
   * @return The status code of this message
   */
  template<typename = void>
  Code status_code() const noexcept;

  /**
   * @brief Change the status code of this message
   *
   * @param code:
   * The new status code to set on this message
   *
   * @return The object that invoked this method
   */
  template<typename = void>
  Response& set_status_code(const Code code) noexcept;

  /**
   * @brief Reset the response message as if it was now
   * default constructed
   *
   * @return The object that invoked this method
   */
  virtual Response& reset() noexcept override;

  /**
   * @brief Get a string representation of this
   * class
   *
   * @return A string representation
   */
  virtual std::string to_string() const override;

  /**
   * @brief Operator to transform this class
   * into string form
   */
  operator std::string () const;
  //-----------------------------------
private:
  //------------------------------
  // Class data members
  std::string response_;
  Status_line status_line_;
  //------------------------------
}; //< class Response

/**--v----------- Implementation Details -----------v--**/

template<typename>
inline Response::Response(const Code code, const Version version) noexcept
  : status_line_{version, code}
{}

inline Response::Response(std::string response, const Limit limit)
  : Message{limit}
  , response_{std::move(response)}
{
  std::experimental::string_view response_view {response_};
  status_line_ = Status_line{response_view};
  header().add_fields(response_view);
  std::experimental::string_view::size_type start_of_body;
  if ((start_of_body = response_view.find("\r\n\r\n")) not_eq std::experimental::string_view::npos) {
    add_body(response_view.substr(start_of_body + 4U));
  } else if ((start_of_body = response_view.find("\n\n")) not_eq std::experimental::string_view::npos) {
    add_body(response_view.substr(start_of_body + 2U));
  }
}

template<typename>
inline Response::Code Response::status_code() const noexcept {
  return static_cast<status_t>(status_line_.code());
}

template<typename>
inline Response& Response::set_status_code(const Code code) noexcept {
  status_line_.set_code(code);
  return *this;
}

inline Response& Response::reset() noexcept {
  Message::reset();
  return set_status_code(OK);
}

inline std::string Response::to_string() const {
  std::ostringstream res;
  //-----------------------------------
  res << status_line_
      << Message::to_string();
  //-----------------------------------
  return res.str();
}

inline Response::operator std::string () const {
  return to_string();
}

/**
 * @brief Convenience function to add a set of header fields to the
 * specified Response object
 *
 * @param response
 * The Response object to add the header fields to
 *
 * @param headers
 * The set of headers to add to the header section of the Response
 * object
 *
 * @return The specified Response object
 */
template<typename = void>
inline Response& operator << (Response& response, const Header_set& headers) {
  for (const auto& field : headers) {
    response.header().add_field(field.first, field.second);
  }
  return response;
}

/**
 * @brief Get a std::unique_ptr to a Response object
 *
 * @param response
 * A character stream of data representing an HTTP response message
 *
 * @return A std::unique_ptr to a Response object
 */
template<typename = void>
inline Response_ptr make_response(std::string response) {
  return std::make_unique<Response>(std::move(response));
}

inline std::ostream& operator << (std::ostream& output_device, const Response& res) {
  return output_device << res.to_string();
}

/**--^----------- Implementation Details -----------^--**/

} //< namespace http

#endif //< HTTP_RESPONSE_HPP
