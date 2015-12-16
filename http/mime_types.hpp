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

#ifndef HTTP_MIME_TYPES_HPP
#define HTTP_MIME_TYPES_HPP

#include <string>
#include <unordered_map>

namespace http {
//------------------------------------------------
using Extension       = std::string;
using Mime_Type       = std::string;
using Mime_Type_Table = std::unordered_map<Extension, Mime_Type>;
//------------------------------------------------
const Mime_Type_Table mime_types {
  //< Text mimes
  {"html", "text/html"},
  {"htm" , "text/html"},
  {"js"  , "text/javascript"},
  {"txt" , "text/plain"},
  {"css" , "text/css"},
  {"xml" , "text/xml"},
  
  //< Image mimes
  {"bmp" , "image/bmp"},
  {"gif" , "image/gif"},
  {"png" , "image/png"},
  {"jpg" , "image/jpeg"},
  {"jpeg", "image/jpeg"},
  {"ico" , "image/x-icon"},

  //< Application mimes
  {"json", "application/json"},
  {"bin" , "application/octet-stream"}
}; //< mime_types

inline Mime_Type extension_to_type(const Extension& extension) noexcept {
  auto iter = mime_types.find(extension);
  return (iter not_eq mime_types.end()) ? iter->second : "text/plain";
}

} //< namespace http

#endif //< HTTP_MIME_TYPES_HPP
