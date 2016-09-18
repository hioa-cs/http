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
   *
   * Constructs a request message as follows:
   *
   * <GET / HTTP/1.1>
   */
  explicit Request() = default;

  /**
   * @brief Construct a request message from the
   * incoming character stream of data which is
   * a {std::string} object
   *
   * @tparam T request:
   * The character stream of data
   *
   * @param limit:
   * Capacity of how many fields can be added to
   * the header section
   */
  template
  <
    typename T,
    typename = std::enable_if_t
               <std::is_same
               <std::string, std::remove_const_t
               <std::remove_reference_t<T>>>::value>
  >
  explicit Request(T&& request, const Limit limit = 25);

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
  Method method() const noexcept;

  /**
   * @brief Set the method of the request message
   *
   * @param method:
   * The method to set
   *
   * @return The object that invoked this method
   */
  Request& set_method(const Method method);

  /**
   * @brief Get read-only reference to the URI of
   * the request message
   *
   * @return Read-only reference to the URI of
   * the request message
   */
  const URI& uri() const noexcept;

  /**
   * @brief Set the URI of the request message
   *
   * @param uri:
   * The URI to set
   *
   * @return The object that invoked this method
   */
  Request& set_uri(const URI& uri);

  /**
   * @brief Get the version of the request message
   *
   * @return The version of the request
   */
  Version version() const noexcept;

  /**
   * @brief Set the version of the request message
   *
   * @param version:
   * The version to set
   *
   * @return The object that invoked this method
   */
  Request& set_version(const Version version) noexcept;

  /**
   * @brief Get the value associated with the name
   * field from a query string
   *
   * @tparam T name:
   * The name to find the associated value
   *
   * @return The associated value if name was found,
   * an empty string otherwise
   */
  template
  <
    typename T,
    typename = std::enable_if_t
               <std::is_same
               <std::string, std::remove_const_t
               <std::remove_reference_t<T>>>::value>
  >
  const std::string& query_value(T&& name) noexcept;

  /**
   * @brief Get the value associated with the name
   * field from the message body in a post request
   *
   * @tparam T name:
   * The name to find the associated value
   *
   * @return The associated value if name was found,
   * an empty string otherwise
   */
  template
  <
    typename T,
    typename = std::enable_if_t
               <std::is_same
               <std::string, std::remove_const_t
               <std::remove_reference_t<T>>>::value>
  >
  std::string post_value(T&& name) const noexcept;

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
  Request_line request_line_;
  //----------------------------------------
}; //< class Request

/**--v----------- Implementation Details -----------v--**/

template <typename Ingress, typename>
inline Request::Request(Ingress&& request, const Limit limit)
  : Message{limit}
  , request_line_{request}
{
  add_headers(request);
  std::size_t start_of_body;
  if ((start_of_body = request.find("\r\n\r\n")) not_eq std::string::npos) {
    add_body(request.substr(start_of_body + 4));
  } else if ((start_of_body = request.find("\n\n")) not_eq std::string::npos) {
    add_body(request.substr(start_of_body + 2));
  }
}

inline Method Request::method() const noexcept {
  return request_line_.get_method();
}

inline Request& Request::set_method(const Method method) {
  request_line_.set_method(method);
  return *this;
}

inline const URI& Request::uri() const noexcept {
  return request_line_.get_uri();
}

inline Request& Request::set_uri(const URI& uri) {
  request_line_.set_uri(uri);
  return *this;
}

inline Version Request::version() const noexcept {
  return request_line_.get_version();
}

inline Request& Request::set_version(const Version version) noexcept {
  request_line_.set_version(version);
  return *this;
}

template <typename Name, typename>
inline const std::string& Request::query_value(Name&& name) noexcept {
  return request_line_.get_uri().query(std::forward<Name>(name));
}

template <typename Name, typename>
inline std::string Request::post_value(Name&& name) const noexcept {
  if ((method() not_eq POST) || get_body().empty() || name.empty()) {
    return std::string{};
  }
  //---------------------------------
  auto target = get_body().find(std::forward<Name>(name));
  //---------------------------------
  if (target == std::string::npos) return std::string{};
  //---------------------------------
  auto focal_point = get_body().substr(target);
  //---------------------------------
  focal_point = focal_point.substr(0, focal_point.find_first_of('&'));
  //---------------------------------
  auto lock_and_load = focal_point.find('=');
  //---------------------------------
  if (lock_and_load == std::string::npos) return std::string{};
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

inline Request_ptr make_request(buffer_t buf, const size_t len) {
  return std::make_unique<Request>(std::string{reinterpret_cast<char*>(buf.get()), len});
}

inline std::ostream& operator << (std::ostream& output_device, const Request& req) {
  return output_device << req.to_string();
}

/**--^----------- Implementation Details -----------^--**/

} //< namespace http

#endif //< HTTP_REQUEST_HPP
