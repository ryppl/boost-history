//                    -- main.hpp --
//
//           Copyright (c) Darren Garvey 2007.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
////////////////////////////////////////////////////////////////
//
//[acgi_echo
//
// This example simply echoes all variables back to the user. ie.
// the environment and the parsed GET, POST and cookie variables.
// Note that GET and cookie variables come from the environment
// variables QUERY_STRING and HTTP_COOKIE respectively.
//

#include <fstream>
#include <cstdio>
#include <vector>
///////////////////////////////////////////////////////////
#include "boost/cgi/acgi.hpp"

using namespace std;
using namespace boost::acgi;

// The styling information for the page.
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
template<typename OStreamT, typename MapT>
void format_map(OStreamT& os, MapT& m, const std::string& title)
{
  os<< "<div class=\"var_map\">"
         "<div class=\"var_map_title\">"
    <<       title
    <<   "</div>";

  if (m.empty())
    os<< "<div class=\"var_pair\">EMPTY</div>";
  else
    for (typename MapT::const_iterator i = m.begin(); i != m.end(); ++i)
    {
      os<< "<div class=\"var_pair\">"
             "<div class=\"var_name\">"
        <<       i->first
        <<   "</div>"
             "<div class=\"var_value\">"
        <<       i->second
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
  response resp;
  try{
    //std::ofstream of("c:/cc/log/cgi.error");
    //if (!of)
    //  throw std::runtime_error("Couldn't open file for writing");
    //std::cerr.rdbuf() = of.rdbuf();

    service s;
    request req(s);

    try {

      boost::system::error_code ec;
      
      // This spits out a form with a file upload facility (these aren't
      // stored, just parsed. Before parsing the POST'd form data, first
      // check that the content-length isn't too long. We need to load
      // up the request environment to check the content length.
      req.load(parse_env);

      if (req.content_length() > 1000L)
      {
        resp<< content_type("text/html");
        resp<< "You're only allowed to upload 1k of data.\n"
            << "Content-length: " << req.content_length();
        // Exit the application here.
        commit(req, resp);
      }
      
      // If execution reaches here we can parse all other
      // request data (cookies, get, post).
      req.load(parse_all, ec);

      if (ec)
      {
        resp<< content_type("text/html")
            << "Error " << ec.value() << ": " << ec.message() << "<p />"
               "--Original message follows--"
               "<p />";
        return commit(req, resp);
      }

      resp<< content_type("text/html") <<
             "<html>"
             "<head>"
               "<title>CGI Echo Example</title>"
               "<style type=\"text/css\">"
          <<       gCSS_text <<
               "</style>"
             "<head>"
             "<body>"
               "Environment stuffs:<br />"
               "  > content_type: " << req.env["Content_type"] << "<br />"
               "  > script uri: " << req.env["script_uri"] << "<br />"
               "  > count('foo'): " << req.get.count("foo") << "<br />"
               "Environment stuffs:<br />"
               "Request ID = " << req.id() << "<br />"
               "Process ID = " << process_id() << "<br />"
               "<form method=POST enctype='application/x-www-form-urlencoded'>"
                 "<input type=text name=name value='"
          <<         (req.post.count("name") ? req.post["name"] : "")
          <<      "' />"
                 "<br />"
                 "<input type=text name=hello value='"
          <<         (req.post.count("hello") ? req.post["hello"] : "")
          <<      "' />"
                 "<br />"
                 "<input type=file name=user_file />"
                 "<input type=hidden name=cmd value=multipart_test />"
                 "<br />"
                 "<input type=submit value=submit />"
               "</form><p />";

      format_map(resp, req.env, "Environment Variables");
      format_map(resp, req.get, "GET Variables");
      format_map(resp, req.post, "POST Variables");
      format_map(resp, req.cookies, "Cookie Variables");

      if (req.request_method() == "GET")
      {
        resp<<   "<pre>";
        BOOST_FOREACH(char& ch, req.post_buffer())
        {
          resp<< ch;
        }
        resp<<   "</pre>"
               "</body>"
               "</html>";
      }

      return commit(req, resp); // All ok.

    }
    catch(boost::system::system_error& ec)
    { // This is the type of error this library throws.
      response resp;
      resp<< content_type("text/plain") << "Error " << ec.code() << ": "
          << ec.what()
          << http::internal_server_error; // note the status_code
      return commit(req, resp, 1);
    }
    catch(std::exception* e)
    {
      response resp;
      resp<< content_type("text/plain") << "Error: " << e->what()
          << http::internal_server_error;
      return commit(req, resp, 2);
    }

    // The request object will be destroyed before the next exception handlers
    // are reached.

    }catch(boost::system::system_error& ec)
    { // This is the type of error this library throws.
      std::cout
      << content_type("text/plain").content
      << std::endl << std::endl
      << "Error " << ec.code() << ": "
            << ec.what()
            << http::internal_server_error
            << std::endl
      << resp.str(); // note the status_code
      return 2;
  }catch(std::exception& e){
      std::cout
      << content_type("text/plain").content
      << std::endl << std::endl
      << "Exception: " << e.what()<< std::endl
      << resp.str();
      return 3;
  }catch(...){
      std::cout<< content_type("text/plain").content
               << std::endl << std::endl
               << "Unknown error."<< std::endl
               << resp.str();
      return 4;
  }
}
//]

