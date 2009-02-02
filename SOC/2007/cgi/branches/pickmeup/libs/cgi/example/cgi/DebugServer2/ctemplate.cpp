
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
    resp<< header("X-Custom-Header: some value")
        << content_type("text/html")
        << "<html>"
           "<head>"
             "<title>Debug Server</title>"
           "</head>"
           "<body>"
           "<h1>Debugging Server Example</h1>"
           "<p>"
             "The server used in this example will catch exceptions thrown by "
             "your request handler, or a non-zero return value. It will print "
             "some presumably helpful info about what might have caused the "
             "problem - obviously this is just an example, which you'd "
             "probably want to ammend."
           "</p>"
           "<p>"
             "The handler in this example will throw a std::runtime_error if "
             "you pass 'badger=bait!' to the script, for example by following "
             "<a href=\"?badger=bait%21\">this link</a>."
           "</p>"
           "<p>"
             "The handler in this example will also return false if you pass "
             "pass 'spam' to the script, for example by clicking "
             "<a href=\"?spam=1\">here</a>."
           "</p>"
           "<p>"
             "Finally, you can simulate an error and get traceback details "
             "regardless of whether the script completed by passing 'debug=1',"
             " or by following <a href=\"?debug=1\">this link</a>."
           "</p>"               
           "</body>"
           "</html>";
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

