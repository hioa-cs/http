# http

A simplified implementation of `HTTP` for [IncludeOS](https://github.com/hioa-cs/IncludeOS)

Outside the context of the `IncludeOS` environment, the main objects of focus are `Request` and `Response`

Example usage:
```
Response process_request(const Request& req) {

  Response res;

  switch (req.method()) {
    case GET:
      if (req.uri().path() == "/") {
        res.add_header(header_fields::Entity::Content_Type, extension_to_type("txt"))
           .add_body("Welcome"s);
        break;
      }
      else if (req.uri().path() == "/grades.html") {
        res.add_header(header_fields::Entity::Content_Type, extension_to_type("html"))
           .add_body("<h2>Grades</h2>"
                     "<ol>"
                     "<li>Alf-Andre</li>"
                     "<li>Andreas</li>"
                     "<li>Rico</li>"
                     "</ol>"s);
        break;
      }

    case HEAD:
      if (req.uri().path() == "/Unikernels.pdf") {
        res.add_header("X-File-Size"s, "6.2MB");
        break;
      }
      else if (req.uri().path() == "/C++.pdf") {
        res.add_header("X-File-Size"s, "10.4MB");
        break;
      }

    default: res.set_status_code(Not_Found);
  }

  return res;
}

int main() {
  while (true) {
    auto conn = connections.front();
    connections.pop();
    std::string data  = conn.read(1024);
    Response response = process_request(Request{data});
    conn.write(response);
    connections.push(conn);
  }
}
```

This is a quick high level overview of the package. More documentation will be written as it mature over time...