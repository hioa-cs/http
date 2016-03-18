# http

A simplified implementation of `HTTP` for [IncludeOS](https://github.com/hioa-cs/IncludeOS)

Outside the context of the `IncludeOS` environment, the main objects of focus are `Request` and `Response`

Example usage:
```
Response process_request(const Request& req) {

  Response res;

  switch (http_method_code(req.method())) {
    case HTTP_GET:
      if (req.uri() == "/") {
        res.add_header(header_fields::Entity::Content_Type, extension_to_type("txt"))
           .add_body("Welcome"s);
        break;
      }
      else if (req.uri() == "/grades.html") {
        res.add_header(header_fields::Entity::Content_Type, extension_to_type("html"))
           .add_body("<h2>Grades</h2>"
                     "<ol>"
                     "<li>Alf-Andre</li>"
                     "<li>Andreas</li>"
                     "<li>Rico</li>"
                     "</ol>"s);
        break;
      }

    case HTTP_HEAD:
      if (req.uri() == "/Unikernels.pdf") {
        res.add_header("X-File-Size"s, "6.2MB");
        break;
      }
      else if (req.uri() == "/C++.pdf") {
        res.add_header("X-File-Size"s, "10.4MB");
        break;
      }

    default: res.set_status_code(status_t::Not_Found);
  }

  return res;
}

int main() {
	while (true) {
		if (conn.has_data()) {
			std::string data  = conn.read(1024);
			Response response = process_request(Request{data});
			conn.write(response);
	  }
  }
}
```

This is a quick high level overview of the package. More documentation will be written as it mature over time...