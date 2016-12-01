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

#ifndef HTTP_REQUEST_HPP
#define HTTP_REQUEST_HPP

#include "message.hpp"
#include "request_line.hpp"

namespace http {

/**
 * @brief This class is used to represent
 * an HTTP request message
 */
class Request : public Message {
public:
  /**
   * @brief Default constructor
   */
  template<typename = void>
  explicit Request();

  /**
   * @brief Construct a request message from the
   * incoming character stream of data
   *
   * @param request:
   * The character stream of data
   *
   * @param len:
   * The length of the character stream
   *
   * @param limit:
   * Capacity of how many fields can be added to
   * the header section
   */
  explicit Request(std::string request, const Limit limit = 25);

  /**
   * @brief Default copy constructor
   */
  Request(Request&) = default;

  /**
   * @brief Default move constructor
   */
  Request(Request&&) = default;

  /**
   * @brief Default destructor
   */
  ~Request() noexcept = default;

  /**
   * @brief Default copy assignment operator
   */
  Request& operator = (Request&) = default;

  /**
   * @brief Default move assignment operator
   */
  Request& operator = (Request&&) = default;

  /**
   * @brief Get the method of the request message
   *
   * @return The method of the request
   */
  template<typename = void>
  Method method() const noexcept;

  /**
   * @brief Set the method of the request message
   *
   * @param method:
   * The method to set
   *
   * @return The object that invoked this method
   */
  template<typename = void>
  Request& set_method(const Method method) noexcept;

  /**
   * @brief Get read-only reference to the URI of
   * the request message
   *
   * @return Read-only reference to the URI of
   * the request message
   */
  template<typename = void>
  const URI& uri() const noexcept;

  /**
   * @brief Set the URI of the request message
   *
   * @param uri:
   * The URI to set
   *
   * @return The object that invoked this method
   */
  template<typename = void>
  Request& set_uri(const URI& uri);

  /**
   * @brief Get the version of the request message
   *
   * @return The version of the request
   */
  template<typename = void>
  Version version() const noexcept;

  /**
   * @brief Set the version of the request message
   *
   * @param version:
   * The version to set
   *
   * @return The object that invoked this method
   */
  template<typename = void>
  Request& set_version(const Version version) noexcept;

  /**
   * @brief Get the value associated with the name
   * from a query string
   *
   * @param name:
   * The name to find the associated value
   *
   * @return The associated value if name was found,
   * an empty string otherwise
   */
  template<typename = void>
  std::experimental::string_view query_value(const std::experimental::string_view name) noexcept;

  /**
   * @brief Get the value associated with the name
   * from the message body in a post request
   *
   * @param name:
   * The name to find the associated value
   *
   * @return The associated value if name was found,
   * an empty string otherwise
   */
  template<typename = void>
  std::experimental::string_view post_value(const std::experimental::string_view name) const noexcept;

  /**
   * @brief Reset the request message as if it was now
   * default constructed
   *
   * @return The object that invoked this method
   */
  virtual Request& reset() noexcept override;

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
private:
  //----------------------------------------
  // Class data members
  std::string request_;
  Request_line request_line_;
  //----------------------------------------
}; //< class Request

/**--v----------- Implementation Details -----------v--**/

template<typename>
inline Request::Request() {}

inline Request::Request(std::string request, const Limit limit)
  : Message{limit}
  , request_{std::move(request)}
{
  std::experimental::string_view request_view {request_};
  request_line_ = Request_line{request_view};
  header().add_fields(request_view);
  std::experimental::string_view::size_type start_of_body;
  if ((start_of_body = request_view.find("\r\n\r\n")) not_eq std::experimental::string_view::npos) {
    add_body(request_view.substr(start_of_body + 4U));
  } else if ((start_of_body = request_view.find("\n\n")) not_eq std::experimental::string_view::npos) {
    add_body(request_view.substr(start_of_body + 2U));
  }
}

template<typename>
inline Method Request::method() const noexcept {
  return request_line_.method();
}

template<typename>
inline Request& Request::set_method(const Method method) noexcept {
  request_line_.set_method(method);
  return *this;
}

template<typename>
inline const URI& Request::uri() const noexcept {
  return request_line_.uri();
}

template<typename>
inline Request& Request::set_uri(const URI& uri) {
  request_line_.set_uri(uri);
  return *this;
}

template<typename>
inline Version Request::version() const noexcept {
  return request_line_.version();
}

template<typename>
inline Request& Request::set_version(const Version version) noexcept {
  request_line_.set_version(version);
  return *this;
}

template<typename>
inline std::experimental::string_view Request::query_value(const std::experimental::string_view name) noexcept {
  return request_line_.uri().query(name.to_string());
}

template<typename>
inline std::experimental::string_view Request::post_value(const std::experimental::string_view name) const noexcept {
  if ((method() not_eq POST) or name.empty() or body().empty()) {
    return {};
  }
  //---------------------------------
  const auto target = body().find(name);
  //---------------------------------
  if (target == std::experimental::string_view::npos) return {};
  //---------------------------------
  auto focal_point = body().substr(target);
  //---------------------------------
  focal_point = focal_point.substr(0, focal_point.find_first_of('&'));
  //---------------------------------
  const auto lock_and_load = focal_point.find('=');
  //---------------------------------
  if (lock_and_load == std::experimental::string_view::npos) return {};
  //---------------------------------
  return focal_point.substr(lock_and_load + 1);
}

inline Request& Request::reset() noexcept {
  Message::reset();
  return set_method(GET)
        .set_uri("/")
        .set_version(Version{1,1});
}

inline std::string Request::to_string() const {
  std::ostringstream req;
  //-----------------------------
  req << request_line_
      << Message::to_string();
  //------------------------------
  return req.str();
}

inline Request::operator std::string () const {
  return to_string();
}

/**
 * @brief Get a std::unique_ptr to a Request object
 *
 * @param request
 * A character stream of data representing an HTTP request message
 *
 * @return A std::unique_ptr to a Request object
 */
template<typename = void>
inline Request_ptr make_request(std::string request) {
  return std::make_unique<Request>(std::move(request));
}

inline std::ostream& operator << (std::ostream& output_device, const Request& req) {
  return output_device << req.to_string();
}

/**--^----------- Implementation Details -----------^--**/

} //< namespace http

#endif //< HTTP_REQUEST_HPP
