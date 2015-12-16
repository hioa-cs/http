// This file is a part of the IncludeOS unikernel - www.includeos.org
//
// Copyright 2015 Oslo and Akershus University College of Applied Sciences
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

#ifndef HTTP_REQ_LINE_HPP
#define HTTP_REQ_LINE_HPP

#include "http_methods.hpp"
#include "http_uri.hpp"

class HTTP_Request_Line {
public:
  explicit HTTP_Request_Line(const std::string& request) :
    method_{request.substr(0, request.find(' '))}
  {}

  const HTTP_METHODS::Method& get_method() const noexcept {
    return method_;
  }


private:
  HTTP_METHODS::Method method_;
};

#endif //< HTTP_REQ_LINE_HPP
