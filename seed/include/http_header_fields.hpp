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

#ifndef HTTP_HEADER_FIELDS_HPP
#define HTTP_HEADER_FIELDS_HPP

#include <string>

namespace HTTP_HEADER_FIELDS {
//-------------------------------
using Field = const std::string;
//-------------------------------
Field Accept              {"Accept"};
Field Accept_Charset      {"Accept-Charset"};
Field Accept_Encoding     {"Accept-Encoding"};
Field Accept_Language     {"Accept-Language"};
Field Authorization       {"Authorization"};
Field Expect              {"Expect"};
Field From                {"From"};
Field Host                {"Host"};
Field If_Match            {"If-Match"};
Field If_Modified_Since   {" If-Modified-Since"};
Field If_None_Match       {"If-None-Match"};
Field If_Range            {"If-Range"};
Field If_Unmodified_Since {"If-Unmodified-Since"};
Field Max_Forwards        {"Max-Forwards"};
Field Proxy_Authorization {"Proxy-Authorization"};
Field Range               {"Range"};
Field Referer             {"Referer"};
Field TE                  {"TE"};
Field User_Agent          {"User-Agent"};
//-------------------------------
} //< namespace HTTP_HEADER_FIELDS

#endif //< HTTP_HEADER_FIELDS_HPP