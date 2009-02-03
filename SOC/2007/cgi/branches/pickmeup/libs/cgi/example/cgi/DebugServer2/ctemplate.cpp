
#include <iostream>
#include <boost/cgi/cgi.hpp>
#include <boost/cgi/utility.hpp>
#include <google/template.h>
#include "TracebackServer.hpp"
#include <cmath>

using boost::cgi::form;
using boost::cgi::request;
using boost::cgi::response;
using boost::cgi::header;
using boost::cgi::content_type;


int cgi_handler(request& req, response& resp)
{
    /*
    for(long i=10000000; i != 0; --i)
    {
        std::sqrt(123.456L); // waste time
    }
    */
    
    // Load the HTML template and strip all whitespace...
    google::Template* tmpl = google::Template::GetTemplate("../templates/default_view.html", google::STRIP_WHITESPACE);
    // Since this is a really basic template, this is all we need to load. We can expand it into a string.
    std::string output;
    tmpl->Expand(&output, NULL);
    resp<< header("X-Custom-Header: some value")
        << content_type("text/html")
        << output;

    if (req[form]["badger"] == "bait!")    throw std::runtime_error("AOUHFAEOUHAEOUHAEOUHOUH!!!!!!");
    else if (has_key(req[form], "spam"))   return 33;
    return 0;
}

int main(int, char**)
{
    TracebackServer server;;
    server.run(&cgi_handler);

    return 0;
}

