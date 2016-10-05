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

#ifndef HTTP_HEADER_FIELDS_HPP
#define HTTP_HEADER_FIELDS_HPP

#include <experimental/string_view>

namespace http {
namespace header_fields {
//------------------------------------------------
using Field = const std::experimental::string_view;
//------------------------------------------------
//------------------------------------------------
Field accept              {"Accept"};
Field accept_charset      {"Accept-Charset"};
Field accept_encoding     {"Accept-Encoding"};
Field accept_ranges       {"Accept-Ranges"};
Field accept_language     {"Accept-Language"};
Field allow               {"Allow"};
Field age                 {"Age"};
Field authorization       {"Authorization"};
Field connection          {"Connection"};
Field cookie              {"Cookie"};
Field content_encoding    {"Content-Encoding"};
Field content_language    {"Content-Language"};
Field content_length      {"Content-Length"};
Field content_location    {"Content-Location"};
Field content_md5         {"Content-MD5"};
Field content_range       {"Content-Range"};
Field content_type        {"Content-Type"};
Field date                {"Date"};
Field etag                {"ETag"};
Field expect              {"Expect"};
Field expires             {"Expires"};
Field from                {"From"};
Field host                {"Host"};
Field http2_settings      {"HTTP2-Settings"};
Field if_match            {"If-Match"};
Field if_modified_since   {"If-Modified-Since"};
Field if_none_match       {"If-None-Match"};
Field if_range            {"If-Range"};
Field if_unmodified_since {"If-Unmodified-Since"};
Field last_modified       {"Last-Modified"};
Field location            {"Location"};
Field max_forwards        {"Max-Forwards"};
Field proxy_authorization {"Proxy-Authorization"};
Field proxy_authenticate  {"Proxy-Authenticate"};
Field range               {"Range"};
Field referer             {"Referer"};
Field retry_after         {"Retry-After"};
Field server              {"Server"};
Field set_cookie          {"Set-Cookie"};
Field te                  {"TE"};
Field upgrade             {"Upgrade"};
Field user_agent          {"User-Agent"};
Field vary                {"Vary"};
Field www_authenticate    {"WWW-Authenticate"};
} //< namespace header_fields
} //< namespace http

#endif //< HTTP_HEADER_FIELDS_HPP
