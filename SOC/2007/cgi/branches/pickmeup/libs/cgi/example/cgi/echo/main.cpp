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

//
// This function writes the title and map contents to the ostream in an
// HTML-encoded format (to make them easier on the eye).
//
template<typename OStreamT, typename MapT>
void show_map_contents(OStreamT& os, MapT& m, const std::string& title)
{
  os<< "<h3>" << title << "</h3>";
  
  if (m.empty())
    os<< "NONE<br />";
  else
    for (typename MapT::const_iterator i = m.begin(); i != m.end(); ++i)
      os<< "<b>" << i->first << "</b> = <i>" 
                 << i->second << "</i><br />";
}

int main()
{
  request req(parse_all); // A basic CGI request auto-parses everything (including POST data).
  response resp;

  resp<< "Request id = " << req.id() << "<p/>";

  show_map_contents(resp, req.env, "Environment Variables");
  show_map_contents(resp, req.form, "Form [" + req.request_method() + "] Variables");
  show_map_contents(resp, req.cookies, "Cookie Variables");

  // Note that this (and any other) HTTP header can go either before or after
  // the response contents.
  resp<< content_type("text/html");

  // Send the response to the client associated with the request.
  resp.send(req.client());

  return 0;
}
//]

