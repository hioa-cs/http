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

#ifndef HTTP_URI_HPP
#define HTTP_URI_HPP

#include <string>

class HTTP_URI {
public:
  explicit HTTP_URI();

  explicit HTTP_URI(const string& uri);

  HTTP_URI(const URI& uri);

  HTTP_URI(const URI& base, const string& relative);

  ~HTTP_URI() = default;

  HTTP_URI& operator = (const HTTP_URI& uri);

  const string& to_string() const;

  const string& get_scheme() const;
  void set_scheme(const string& scheme);

  const string& get_user_info() const;
  void set_user_info(const string& user_info);
    
  const string& get_host() const;
  void set_host(const string& host);
    
  port_t get_port() const;
  void set_port(const port_t port);

  const string& get_path() const;
  void set_path(const string& path);
    
  const string& get_authority() const;
  void set_authority(const string& authority);

  const string& get_query() const;
  void setquery(const string& query);

  const string& get_fragment() const;
  void set_fragment(const string& fragment);

  bool operator == (const URI& uri) const;
  bool operator != (const URI& uri) const;

private:
  //----------------------------
  using std::string;
  using port_t = unsigned short;
  //----------------------------
  string scheme_;
  string userInfo_;
  string host_;
  port_t port_;
  string path_;
  string query_;
  string fragment_;
};

#endif //< HTTP_URI_HPP
