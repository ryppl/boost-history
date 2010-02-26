//                     -- fastcgi_quickstart.cpp --
//
//            Copyright (c) Darren Garvey 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
////////////////////////////////////////////////////////////////

//[fastcgi_quickstart

#include <iostream> // for std::cerr

/*<
A catch-all header is available which includes all of the headers you should
need for CGI.

For the sake of clarity we alias the `boost::fcgi` namespace rather than
dumping all of the library names with a `using namespace`. This way, you can
see what comes from the library.
>*/
#include <boost/cgi/fcgi.hpp>
namespace cgi = boost::fcgi;

/*<
The first thing to do is write a handler function which takes a request and a
response and does all request-specific work. Later, we will look at writing
the code that calls this function.
>*/
int handle_request(fcgi::request& req, fcgi::response& resp)
{
/*<
In our request handler, we will assume that the request has been fully-parsed
and we can access all of the request data. The request data is available using
`std::map<>`-like public members of a `fcgi::request`.[footnote
The data is stored internally in a single `fusion::vector<>`]

A FastCGI request has several types of variables available. These are listed
in the table below, assuming that `req` is an instance of `fcgi::request`:

[table
  [[Source] [Variable] [Description]]
  [
    [Environment] [`req.env`] [The environment of a FastCGI request contains
    most of the information you will need to handle a request. There is a basic
    set of common environment variables that you can expect to be set by most
    HTTP servers around. A list of them is available on the __TODO__ (link)
    variables page.]
  ]
  [
    [GET] [`req.get`] [The variables passed in the query string of an HTTP GET
    request.]
  ]
  [
    [POST] [`req.post`] [The HTTP POST data that is sent in an HTTP request's
    body. File uploads are not stored in this map.]
  ]
  [
    [Cookies] [`req.cookies`] [Cookies are sent in the HTTP_COOKIE environment
    variable. These can store limited amounts session information on the client's
    machine, such as database session ids or tracking information.]
  ]
  [
    [File Uploads] [`req.uploads`] [File uploads, sent in an HTTP POST where
    the body is MIME-encoded as multipart/form-data. Uploaded files are read
    onto the server's file system. The value of an upload variable is the path
    of the file.]
  ]
  [
    [Form] [`req.form`] [The form variables are either the GET variables or
    the POST variables, depending on the request method of the request.]
  ]
]

Let's assume you now want to check if the user has a cookie, "user_name",
set. We can check if a user has a cookie set like this:
>*/
  if (req.cookies.count("user_name"))
  {
/*<
First, we need to be able to clear the cookie we are setting. We will reset
the cookie if the user navigates to `"/path/to/script?reset=1"`.

The `reset` variable in the query string is a GET variable. The request data
is accessed through a proxy class which works just like a `std::map<>` with
some extra features.

One of them is `pick`, which looks up a key in the map and returns the value
if it is found. Otherwise it returns a default value, which is the second
argument. 

The default value can be any type that supports
[@http://boost.org/libs/lexical_cast Boost.Lexical_cast]. If the key isn't
found in the map, or the value cannot be cast to the type of the default
value, the default is returned.
>*/
    if (req.get.pick<std::string>("reset", "") == "1")
    {
      resp<< cgi::cookie("user_name") /*<
Set a cookie with no value to delete it.
>*/
          << cgi::redirect(req, req.script_name()) /*<
The `cgi::redirect` free function returns a `"Location"` header that will
redirect the user to the specified URL. This URL can be a relative or absolute
but an absolute URL is always returned. To perform an internal redirect, use
`cgi::location` instead.
>*/
          << cgi::content_type("text/plain");
    }
    else
    {
      std::string user_name( req.cookies["user_name"] );
/*<
Looking up a request cookie in `req.cookies` really returns a `cgi::cookie`.
The line above works though because a `cgi::cookie` is implicitly convertible
to a `std::string`.

The lookup is case-insensitive, so "USER_NAME" and "User_Name" would be
equivalent lookup keys.

If the cookie is set, we'll be polite and say hello.
>*/
      if (!user_name.empty())
      {
        resp<< cgi::content_type("text/html")
            << "<p>Hello there, " << req.cookies["user_name"]
            << ". How are you?</p>"
            << "<a href=\"" << req.script_name() << "?reset=1\">Reset</a>";
      }
    }
  } else
  if (req.form.count("user_name"))
  {
/*<
If the cookie isn't set, we will check if the user has posted a __GET__/
__POST__ form with their name.
>*/
    std::string user_name (req.form["user_name"]);
/*<
If they have told us their name, we should set a cookie so we remember it next
time. Then we can say hello and exit.

There are two ways to set a cookie: either directly using
`req.set_cookie("user_name", user_name)` or the method shown. You can also
send an expiry date and a path for the cookie.[footnote 
See [@http://tools.ietf.org/html/rfc822 RFC822] for more.
]
Note that if you set a cookie with no value, the cookie will be deleted.

Again, the request object isn't buffered, so we are going to keep using the
`response` in case something breaks and we end up not wanting to set the
cookie. The cookie we set below will expire when the client closes their
browser.

This time, we shall send a Date header. If we do this (ie. send a header
ourselves), we must also set the Content-type header, like below.
>*/
    resp<< cgi::cookie("user_name", user_name)
        << cgi::header("Date", "Tue, 15 Nov 1994 08:12:31 GMT")
        << cgi::content_type("text/html")
        << "Hello there, " << user_name << ". You're new around here."
        << "user_name.length() = " << user_name.length() ;
  }
  else
  {
/*<
Now, if we have no idea who they are, we'll send a form asking them for their
name. As the default `"Content-type"` header is `"text/plain"`, we'll change
this to `"text/html"` so the user's browser will display the HTML form. You
can do this using
    `set_header(req,  "Content-type", "text/html")`
or
    `resp<< header("Content-type", "text/html")`.
Since writing with raw strings is error-prone, the shortcut below is available.
>*/
  resp<< cgi::content_type("text/html")
      << "Hello there. What's your name?" "<p />"
         "<form method='POST'>"
         "<input type='text' name='user_name' />"
         "<input type='submit' />";

  }
/*<
Finally, send the response back and close the request.
>*/
  return cgi::commit(req, resp);
}

/*<
We now have a request handler in all of it's contrived glory.

The program's `main` function needs to parse the request, call the request
handler defined above, and finally send the response.
>*/
int main(int, char**)
{
  cgi::service service;
/*<
An `Acceptor` handles accepting requests and little else.  
>*/
  cgi::acceptor acceptor(service, 8010);
/*<
The `response` class provides a streaming interface for writing replies. You
can write to the request object directly, but for now we're going to just
use the `response`, which works well for most situations.

Writing to a `response` is buffered. If an error occurs, you can simply
`clear()` the response and send an error message instead. Buffered writing
may not always suit your use-case (eg. returning large files), but when memory
is not at a real premium, buffering the response is highly preferable.

Not only does buffering avoid network latency issues, but being able to cancel
the response and send another is much cleaner than sending half a response,
followed by "...Ooops". A `cgi::response` is not tied to a request, so the
same response can be reused across multiple requests.

When sending a response that is large relative to the amount of memory
available to the program, you may want to write unbuffered.
>*/
  cgi::response response;
  int status;
  
/*<
Keep accepting requests until the handler returns an error.
>*/
  do {
/*<
The function `boost::fcgi::acceptor::accept` has a few overloads. The one used
here takes a function or function object with the signature:
``
boost::function<int (boost::fcgi::request&)>
``
ie. A function that takes a reference to a `request` and returns an `int`.
The returned `int` should be non-zero if the request was handled with
an error.

Since our handler has been defined to take references to a `request` and a
`response`, we can use [@http://boost.org/libs/bind Boost.Bind] to wrap our
function to give it the signature we need. See the documentation of Boost.Bind
and [@http://boost.org/libs/function Boost.Function] for more information .
>*/
    status = acceptor.accept(boost::bind(&handle_request, _1, boost::ref(response)));
    if (status) {
        std::cerr
            << "Request handled with error. Exit code: " << status << std::endl
            << "Response body follows: " << std::endl
            << response << std::endl;
    }
    response.clear();
  } while (!status);
  return status;
}

//]
