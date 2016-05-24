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
#include <string>
#include <sstream>
#include <iomanip>

namespace http {
namespace time {

//------------------------------------------------
// Get the current time in {Internet Standard Format}
//
// @return The current time as a std::string
//
// @note Returns an empty string if an error occurred
//------------------------------------------------
inline std::string now() {
  auto time_object = std::time(nullptr);
  auto tm          = std::gmtime(&time_object);

  if (tm) {
    std::ostringstream output;
    output << std::put_time(tm, "%a, %d %b %Y %H:%M:%S %Z");
    return output.str();
  }

  return std::string{};
}

} //< namespace time
} //< namespace http
