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

#include <ctime>
#include <experimental/string_view>
#include <iomanip>
#include <sstream>
#include <string>

namespace http {
namespace time {

/**
 * @brief Get the time in {Internet Standard Format} from
 * a {time_t} object 
 *
 * @param time:
 * The time_t object to get the time from
 *
 * @return The time in {Internet Standard Format} from
 * a {time_t} object
 *
 * @note Returns an empty string if an error occurred
 */
inline std::string from_time_t(const std::time_t time) {
  auto tm = std::gmtime(&time);

  if (tm) {
    std::ostringstream output;
    output << std::put_time(tm, "%a, %d %b %Y %H:%M:%S %Z");
    return output.str();
  }

  return std::string {};
}

/**
 * @brief Get a {time_t} object from a {std::string} representing
 * timestamps specified in RFC 2616 §3.3
 *
 * @param time:
 * The {std::string} representing the timestamp
 *
 * @return A {time_t} object from {time}
 *
 * @note Returns a default initialized {time_t} object if an error occurred
 */
inline std::time_t to_time_t(const std::experimental::string_view time) {
  std::tm tm {};

  if (time.empty()) goto error;

  // Format: Sun, 06 Nov 1994 08:49:37 GMT
  if (strptime(time.data(), "%a, %d %b %Y %H:%M:%S %Z", &tm) not_eq nullptr) {
    return std::mktime(&tm);
  }

  // Format: Sunday, 06-Nov-94 08:49:37 GMT
  if (strptime(time.data(), "%a, %d-%b-%y %H:%M:%S %Z", &tm) not_eq nullptr) {
    return std::mktime(&tm);
  }

  // Format: Sun Nov 6 08:49:37 1994
  if(strptime(time.data(), "%a %b %d %H:%M:%S %Y", &tm) not_eq nullptr) {
    return std::mktime(&tm);
  }

  error:
  	return std::time_t {};
}

/**
 * @brief Get the current time in {Internet Standard Format}
 *
 * @return The current time as a {std::string}
 *
 * @note Returns an empty string if an error occurred
 */
inline std::string now() {
  return from_time_t(std::time(nullptr));
}

} //< namespace time
} //< namespace http
