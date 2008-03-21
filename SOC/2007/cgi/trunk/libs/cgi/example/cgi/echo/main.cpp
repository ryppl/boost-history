//                    -- main.hpp --
//
//           Copyright (c) Darren Garvey 2007.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
////////////////////////////////////////////////////////////////
//
// This example simply echoes all variables back to the user. ie.
// the environment and the parsed GET, POST and cookie variables.
// Note that GET and cookie variables come from the environment
// variables QUERY_STRING and HTTP_COOKIE respectively.
//

#include <boost/cgi/acgi.hpp>

using namespace std;
using namespace boost::acgi;

// This function writes the title and map contents to the ostream in an
// HTML-encoded format (to make them easier on the eye).
template<typename MapT, typename OStreamT>
void show_map_contents(MapT& m, OStreamT& os, const std::string& title)
{
  os<< "<h3>" << title << "</h3>";
  if (m.empty()) os<< "NONE<br />";
  for (typename MapT::iterator i = m.begin(); i != m.end(); ++i)
  {
    os<< "<b>" << i->first << "</b> = <i>" << i->second << "</i><br />";
  }
}

int main()
{
  service s;
  request req(s);

  req.load(true); // The 'true' means parse STDIN data too.

  response resp;

  show_map_contents(req.env(), resp, "Environment Variables");
  show_map_contents(req.GET(), resp, "GET Variables");
  show_map_contents(req.POST(), resp, "POST Variables");
  show_map_contents(req.cookie(), resp, "Cookie Variables");

  // Note that this (and any other) HTTP header can go either before or after
  // the response contents.
  resp<< content_type("text/html");

  // Send the response to the client associated with the request.
  resp.send(req.client());

  return 0;
}
