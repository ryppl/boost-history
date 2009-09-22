//                    -- main.hpp --
//
//           Copyright (c) Darren Garvey 2007.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
////////////////////////////////////////////////////////////////
//
//[cgi_echo
//
// This example simply echoes all variables back to the user. ie.
// the environment and the parsed GET, POST and cookie variables.
// Note that GET and cookie variables come from the environment
// variables QUERY_STRING and HTTP_COOKIE respectively.
//

#include <boost/cgi/cgi.hpp>

using namespace boost::cgi;

// The styling information for the page, just to make things look nicer.
static const char* gCSS_text =
"body { padding: 0; margin: 3%; border-color: #efe; }"
".var_map_title"
    "{ font-weight: bold; font-size: large; }"
".var_map"
    "{ border: 1px dotted; padding: 2px 3px 2px 3px; margin-bottom: 3%; }"
".var_pair"
    "{ border-top: 1px dotted; overflow: auto; padding: 0; margin: 0; }"
".var_name"
    "{ position: relative; float: left; width: 30%; font-weight: bold; }"
".var_value"
    "{ position: relative; float: left; width: 65%; left: 1%;"
     " border-left: 1px solid; padding: 0 5px 0 5px;"
     " overflow: auto; white-space: pre; }"
;

//
// This function writes the title and map contents to the ostream in an
// HTML-encoded format (to make them easier on the eye).
//
template<typename OStream, typename Request, typename Map>
void format_map(OStream& os, Request& req, Map& m, const std::string& title)
{
  os<< "<div class=\"var_map\">"
         "<div class=\"var_map_title\">"
    <<       title
    <<   "</div>";

  if (m.empty())
    os<< "<div class=\"var_pair\">EMPTY</div>";
  else
    for (typename Map::const_iterator i = m.begin(); i != m.end(); ++i)
    {
      os<< "<div class=\"var_pair\">"
             "<div class=\"var_name\">"
        <<       i->first
        <<   "</div>"
             "<div class=\"var_value\">"
        <<       i->second
             << (req.is_file(i->first) ? " (file)" : "")
        <<   "</div>"
           "</div>";
    }
  os<< "</div>";
}

std::size_t process_id()
{
#if defined(BOOST_WINDOWS)
  return _getpid();
#else
  return getpid();
#endif
}


int main()
{
  // A basic CGI request auto-parses everything (including POST data).
  request req(parse_all);
  response resp;

  // You can also stream text to a response. 
  // All of this just prints out the form 
  resp<< "<html>"
         "<head>"
           "<title>CGI Echo Example</title>"
           "<style type=\"text/css\">"
      <<       gCSS_text <<
           "</style>"
         "<head>"
         "<body>"
           "Request ID = " << req.id() << "<br />"
           "Process ID = " << process_id() << "<br />"
           "<form method=post enctype=\"multipart/form-data\">"
             "<input type=text name=name value='"
      <<         req.post["name"] << "' />"
             "<br />"
             "<input type=text name=hello value='"
      <<         req.post["hello"] << "' />"
             "<br />"
             "<input type=file name=user_file />"
             "<input type=hidden name=cmd value=multipart_test />"
             "<br />"
             "<input type=submit value=submit />"
           "</form><p />";

  format_map(resp, req, req.env, "Environment Variables");
  //format_map(resp, req, req.get, "GET Variables");
  format_map(resp, req, req.form, "Form [" + req.request_method() + "] Variables");
  format_map(resp, req, req.cookies, "Cookie Variables");

  // Note that this (and any other) HTTP header can go either before or after
  // the response contents.
  resp<< content_type("text/html");

  // Send the response to the client that made the request.
  return commit(req, resp);
}
//]

