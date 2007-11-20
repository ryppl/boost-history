//                    -- main.hpp --
//
//           Copyright (c) Darren Garvey 2007.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
////////////////////////////////////////////////////////////////
//
// The simplest CGI program, outputs only "Hello there, universe."
//

#include <boost/cgi/acgi.hpp>

using namespace std;
using namespace cgi::acgi;

int main()
{
  service srv;
  request req(srv);
  response resp;

  resp<< content_type("text/html")
      << "Hello there, universe.";

  resp.send(req.client());

  return 0;
}