//                     -- main.hpp --
//
//           Copyright (c) Darren Garvey 2007.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
////////////////////////////////////////////////////////////////
//
//[acgi_cookie_game
//
// Cookie Tests
// ------------
//
// This simple example shows the user their cookies and lets them set and
// delete them.
//
#include <boost/cgi/acgi.hpp>
#include <boost/cgi/utility.hpp>

// The styling information for the page.
static const char* gCSS_text =
"body { padding: 0; margin: 3%; border-color: #efe; }"
".var_map_title"
    "{ font-weight: bold; font-size: large; }"
".var_map"
    "{ border: 1px dotted; padding: 2px 3px 2px 3px; margin-bottom: 3%; }"
".pair"
    "{ border-top: 1px dotted; overflow: auto; padding: 0; margin: 0; }"
/*
".name"
    "{ position: relative; float: left; width: 30%; font-weight: bold; }"
".value"
    "{ position: relative; float: left; width: 65%; left: 1%;"
     " border-left: 1px solid; padding: 0 5px 0 5px;"
     " overflow: auto; white-space: pre; }"
*/
".info"
"{"
    "border-color: #ca3766;"
    "border-width: 1px 0 1px 0;"
    "border-style: solid;"
    "padding: 2px 8px 2px 8px;"
    "margin: 1em"
"}"
"#response"
"{"
    "text-align: center;"
    "padding: 20px 30px 20px 30px;"
    "margin: 1em;"
    "border-width: 2px 0 2px 0;"
    "border-style: solid;"
    "border-color: #ca3766;"
"}"
".var_map_title"
"{"
    "font-weight: bold;"
    "font-size: large;"
"}"
".var_map"
"{"
    "border: 1px dotted;"
    "padding: 2px 3px 2px 3px;"
    "margin: 15px 0 15px 0;"
"}"
".var_pair"
"{"
    "border-top: 1px dotted;"
    "overflow: auto;"
    "padding: 0;"
    "margin: 0;"
"}"
".var_name"
"{"
    "position: relative;"
    "float: left;"
    "width: 30%;"
    "font-weight: bold;"
"}"
".var_value"
"{"
    "position: relative;"
    "float: left;"
    "width: 65%;"
    "left: 1%;"
    "border-left: 1px solid;"
    "padding: 0 5px 0 5px;"
    "overflow: auto;"
    "white-space: pre;"
"}"
".nvpair"
"{"
    "list-style-type: none;"
"}"
"form"
"{"
//    "width: 450px;"
"}"
"input"
"{"
"border-width: 0 0 1px 0;"
"border-style: solid;"
"border-color: #444;"
"padding: 2px 3px 2px 3px;"
"margin-left: 1em;"
"background: #FaFbFd;"
"}"
"input[type=submit]"
"{"
    "margin-left: 35%;"
"}"
".name, .value"
"{"
    "display: inline;"
    "width: 20%;"
    "float: left;"
"}"
".name"
"{"
    "text-align: right;"
    "padding-right: 1em;"
    "clear: left;"
"}"
".value"
"{"
    "text-align: left;"
    "padding-left: 1em;"
"}"
".clear { clear: both; }"
".cookies { margin: 10px 0 25px 0; }"
"#postform"
"{" 
    "visibility: hidden;"
    "display: none;"
"}"
;

static const char* gJS_text =
"function switch_method(ident) {"
    "var f = document.getElementById(ident);"
    "if (f.method == 'post') f.method = 'GET';"
    "else f.method = 'POST';"
"}"
"function switch_value(elem) {"
    "if (elem.value == 'Switch to POST') {"
        "elem.value = 'Switch to GET';"
    "}else{"
        "elem.value = 'Switch to POST';"
    "}"
"}"
"function switch_form(elem) {"
    "var g = document.getElementById('getform');"
    "var p = document.getElementById('postform');"
    "if (elem.value == 'Switch to POST') {"
        "p.style.visibility = 'visible';"
        "p.style.display = 'block';"
        "g.style.visibility = 'hidden';"
        "g.style.display = 'none';"
        "elem.value = 'Switch to GET';"
    "} else {"
        "g.style.visibility = 'visible';"
        "g.style.display = 'block';"
        "p.style.visibility = 'hidden';"
        "p.style.display = 'none';"
        "elem.value = 'Switch to POST';"
    "}"
"}"
;

/**
 * The following example has a few stages.
 * It is best understood by compiling and testing it, and then looking at
 * the source code.
 */

using namespace boost::acgi;

template<typename Response, typename MapT>
void print_formatted_data(Response& resp, MapT& data)
{
    resp<< "<div class=\"cookies\">";
    if (data.empty())
        resp<< "<span class=\"value\">None found</span>";
    else
    {
      resp<< "<ul class=\"nvpair\">";
      for(typename MapT::const_iterator iter=data.begin(), end = data.end();
          iter != end; ++iter)
      {
        resp<< "<li class=\"name\">"
            <<     iter->first
            << "</li>"
               "<li class=\"value\">"
            <<     iter->second
            << "</li>";
      }
      resp<< "</ul>"
             "<div class=\"clear\"></div>";
    }
    resp<< "</div>";
}

int main()
{
  request req;

  // Load up the request data
  req.load(parse_all);

  response resp;
  resp<< content_type("text/plain");

  if (req.form.count("reset") && req.form["reset"] == "true")
  {
    resp<< cookie("name")
        << redirect(req, req.script_name()); // redirect them.
    return commit(req, resp);
  }

  if (req.form.count("name"))
  {
    if (req.form.count("del"))
      resp<< cookie(req.form["name"]);
    else
      resp<< cookie(req.form["name"], req.form["value"]);
    resp<< redirect(req, req.script_name());
    return commit(req, resp);
  }

  resp<< content_type("text/html")
      << "<html>"
         "<head>"
         "<style type=\"text/css\">"
      <<   gCSS_text
      << "</style>"
         "<script type=\"text/javascript\" language=\"Javascript\">"
      <<   gJS_text
      << "</script>"
         "</head>"
         "<body>";

  // First, see if they have a cookie set
  if (req.cookies.count("name"))
      resp<< "<h1>Hello again " << req.cookies["name"] << "</h1>"
          << "<p><a href='?reset=true'><input type='submit' value='Reset' /></a></p>";
  else
    resp<< "<h1>Hello there.</h1>";

  resp<< "<p>"
            "You can add cookies using the form below. If you add a cookie "
            "value for 'name', it will show up above."
         "</p>"
          "<p>Here is list of the cookies you currently have:</p>";
  
  print_formatted_data(resp, req.cookies);
  
  resp<< "<form method=get action='" << req.script_name() << "' id=getform>"
           "<label for='name' class='name'>Name:</label>"
           "<input id='name' name='name' class='value' type='text' value='" 
              << req.form["name"] << "'>"
           "</input>"
           "<label for='value' class='name'>Value:</label>"
           "<input id='value' name='value' class='value' type='text' value='"
              << req.form["value"] << "'>"
           "</input>"
           "<label for='del' class='name'>Delete this cookie?</label>"
           "<input id='del' name='del' class='value' type='checkbox'></input>"
           "<div class='clear'></div>"
           "<input type='submit'></input>"
         "</form>"
         "<input type='submit' "
             "onclick='switch_method(\"getform\"); "
                      "switch_value(this); return false;' "
                      "value='Switch to POST'>"
         "</input>"
         "</body>"
         "</html>";

  return commit(req, resp);
}
//]

