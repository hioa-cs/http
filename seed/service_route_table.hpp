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

#ifndef SERVICE_ROUTE_TABLE_HPP
#define SERVICE_ROUTE_TABLE_HPP

inline http::Router init_routes() {
  using namespace std;
  using namespace http;
  //--------------------------------------------------------------------
  Router routes;
  //--------------------------------------------------------------------
  routes.on_get("/"s, [](const auto& req, auto& res) {
    res.add_header(header_fields::Response::Server, "IncludeOS/v0.7.0"s)
       .add_header(header_fields::Entity::Content_Type, "text/html; charset=utf-8"s)
       .add_body("<h1>WELCOME TO IncludeOS</h1>"s);
  });
  //--------------------------------------------------------------------
  routes.on_get("/listing.html"s, [](const auto&, auto& res){
    res.add_header(header_fields::Response::Server, "IncludeOS/v0.7.0"s)
       .add_header(header_fields::Entity::Content_Type, "text/html; charset=utf-8"s)
       .add_body("<h1>IncludeOS Main Developers</h1>"
                 "<ul>"
                 "<li>Alfred Bratterud</li>"
                 "<li>Andreas Åkesson</li>"
                 "<li>Alf-Andre Walla</li>"
                 "<li>Rico Antonio Felix</li>"
                 "</ul>"s);
  });
  //--------------------------------------------------------------------
  routes.on_post("/", [](const auto&, auto& res){
    res.add_header(header_fields::Response::Server, "IncludeOS/v0.7.0"s)
       .add_header(header_fields::Entity::Content_Type, "text/plain"s)
       .add_body("NO POSTING TODAY!!!"s);
  });
  //--------------------------------------------------------------------
  routes.on_head("/c++.pdf"s, [](const auto&, auto& res){
    res.add_header(header_fields::Response::Server, "IncludeOS/v0.7.0"s)
       .add_header("File-Size"s, "6.8MB"s);
  });
  //--------------------------------------------------------------------
  return routes;
}

#endif //< SERVICE_ROUTE_TABLE_HPP
