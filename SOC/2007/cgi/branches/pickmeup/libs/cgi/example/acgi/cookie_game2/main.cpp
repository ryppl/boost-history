//                     -- main.hpp --
//
//           Copyright (c) Darren Garvey 2007.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////
//[acgi_cookie_game2
//
// Cookie Test With cTemplate
// --------------------------
//
// This file uses Google cTemplate [1] to show the benefits of using an HTML
// template engine. Using cTemplate to separate how you show the response and
// how you figure out what to respond with, is keeping to the MVC paradigm.
// Read up on that if you're not familiar; if you already are, you can
// probably stop scowling at the last cookie_game example now.
//
// [1] - http://code.google.com/p/google-ctemplate/
//
#include <boost/cgi/acgi.hpp>
#include <boost/cgi/utility.hpp>
#include <google/template.h>
#include <boost/throw_exception.hpp>
#include <boost/system/system_error.hpp>
#include <boost/filesystem.hpp>
//]

/**
 * The following example has a few stages.
 * It is best understood by compiling and testing it, and then looking at
 * the source code.
 */

//[main

using namespace boost::acgi;
namespace fs = boost::filesystem;

// The types we use. Only here because this is an example.

// Uses cTemplate, from Google. It's simple and powerful.
typedef google::Template stencil_type;
// You will usually load a template and then populate variables in it
// using a TemplateDictionary.
typedef google::TemplateDictionary dictionary_type;
// The acgi and fcgi parts of the CGI library use a `service` class to 
// manage asynchronous dispatching (eg. async I/O). If you're not interested
// in async I/O, you can just use the plain cgi stuff (which is the same as
// acgi, but without the *a*sync bits).
// If you're unsure, you can use acgi without having to really do anything with
// the service - it's only used on two lines in this example. In one of them
// it is constructed...
typedef service service_type;
// ... and on the other it's used to construct the `request`.
typedef request request_type;
// The `response` will make your life easier (and more efficient).
typedef response response_type;

// These are some of the functions / types / enums used in this example.
using boost::acgi::has_key;
using boost::acgi::cookie;
using boost::acgi::header;
using boost::acgi::redirect;
using boost::acgi::parse_all;
using boost::acgi::form;
using boost::acgi::cookies;
using boost::acgi::content_type;

// This function just makes it easier to change the templating engine. It's
// only here to keep the cTemplate code out of the core of this example...
stencil_type* get_stencil(std::string const& filename)
{
  if (!fs::exists(filename))
    throw std::runtime_error(std::string("Template file not found: '") + fs::path(filename).string() + "'");
  return google::Template::GetTemplate(filename, google::STRIP_WHITESPACE);
}

// Show the data in the passed map, updating the passed dictionary.
template<typename MapT, typename Dict>
void print_formatted_data(MapT& data, Dict& dict)
{
  Dict* subd = dict.AddSectionDictionary("DATA_MAP");
  if (data.empty())
    subd->ShowSection("EMPTY");
  else
    for(typename MapT::const_iterator iter=data.begin(), end = data.end(); iter != end; ++iter)
    {
      Dict* row_dict = subd->AddSectionDictionary("ROW");
      row_dict->SetValue("NAME", iter->first.c_str());
      row_dict->SetValue("VALUE", iter->second);
      row_dict->ShowSection("ROW");
    }
}


int main()
{
  try {
    service_type srv;
    request_type req(srv);

    // Load up the request data
    req.load(parse_all);

    response_type resp;

    // Check if we are resetting the user.
    if (has_key(req.form, "reset") && req.form["reset"] == "true")
    {
      resp<< cookie("name") // delete the 'name' cookie.
          << redirect(req, req.script_name()); // redirect them.
      resp.send(req.client());
      return 0;
    }

    if (has_key(req.form, "name"))
    {
      // If requested by the user, delete the cookie.
      if (has_key(req.form, "del"))
        resp<< cookie(req.form["name"]);
      else // Set the cookie.
        resp<< cookie(req.form["name"], req.form["value"]);
      resp<< redirect(req, req.script_name());
      // Exit here.
      return_(resp, req, http::ok);
    }

    dictionary_type dict("cookie-game dict");

    // First, see if they have a cookie set
    if (has_key(req.cookies, "name"))
      dict.SetValueAndShowSection("USER_NAME", req.cookies["name"], "HAS_NAME_IN_COOKIE_true");
    else
      dict.ShowSection("HAS_NAME_IN_COOKIE_false");

    print_formatted_data(req.cookies, dict);

    dict.SetValue("SCRIPT_NAME", req.script_name());
    // get_value is defined in boost/cgi/util/
    // Looks up the key in the map, returns a default value if the key 
    // isn't found.
    dict.SetValue("COOKIE_NAME", get_value(req.form, "name", ""));
    dict.SetValue("COOKIE_VALUE", req.form["value"]);

    // Load the HTML stencil now from the index.html file.
    stencil_type* stencil = get_stencil("../templates/index.html");

    // Expand the stencil with the the given dictionary into `output`.
    std::string output;
    stencil->Expand(&output, &dict);

    // Add the template to the response.
    resp<< content_type("text/html")
        << output;

    // Send the response to the requestor and return control.
    return_(resp, req, http::ok);

  }catch(boost::system::system_error& err){
    std::cerr<< "System Error: [" << err.code() << "] - " << err.what() << std::endl;
  }catch(std::exception const& e){
    std::cerr<< "Exception: [" << typeid(e).name() << "] - " << e.what() << std::endl;
  }catch(...){
    std::cerr<< "boom<blink>!</blink>";
  }
}
//]

