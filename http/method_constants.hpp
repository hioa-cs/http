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

#ifndef HTTP_METHOD_CONSTANTS_HPP
#define HTTP_METHOD_CONSTANTS_HPP

#define HTTP_OPTIONS 0x0000
#define HTTP_GET     0x0001
#define HTTP_HEAD    0x0002
#define HTTP_POST    0x0003
#define HTTP_PUT     0x0004
#define HTTP_DELETE  0x0005
#define HTTP_TRACE   0x0006
#define HTTP_CONNECT 0x0007
#define HTTP_PATCH   0x0008
#define HTTP_INVALID 0xffff

#endif //< HTTP_METHOD_CONSTANTS_HPP
