
#include <boost/cgi/acgi.hpp>

using namespace cgi::acgi;

int show_default_page(request& req, response& resp)
{
  resp
  << content_type("text/html")
  << "<html>"
     "<head>"
       "<title>Login</title>"
     "</head>"
     "<body>"
       "<center>"
         "<form action='POST'>"
          "<input type='text' name='name' />"
          "<input type='button' name='cmd' value='login' />"
         "</form>"
       "</center>"
     "</body>"
     "</html>";

  resp.send(req.client());
  return req.end(http::ok);
}

void show_already_logged_in_page(request& req, response resp)
{
  resp
  << content_type("text/html")
  << "<html>"
     "<head><title>Redirecting...</title>"
     "<meta http-equiv='refresh' content='5;url="<< req.POST("fwd") <<"' />"
     "</head>"
     "<body>"
     "<center>"
     "You are already logged in. You should be redirected "
     "<a href='"<< req.POST("fwd") <<"'>here</a>"
     " in five seconds."
     "</center>"
     "</body>"
     "</html>";
  
  resp.send(req.client());
  return req.end(http::ok);
}

int show_name_error_page(request& req, response& resp)
{
  resp
  << content_type("text/html")
  << "<html>"
     "<head>"
       "<title>Login</title>"
     "</head>"
     "<body>"
       "<center>"
         "<form action='POST'>"
          "<span class='red'>Your user name must only use letters, numbers or "
          "the underscore character."
          "</span>"
          "<input type='text' name='name' value='"<< req.POST("name") <<"' />"
          "<input type='button' name='cmd' value='login' />"
         "</form>"
       "</center>"
     "</body>"
     "</html>";

  resp.send(req.client());
  return req.end(http::bad_request);
}

int verify_name(std::string& name)
{
  using namespace boost;

  regex re("\\s*([_a-zA-Z0-9]{5,})\\s*");
  cmatch what;

  if (!regex_match(name, what, re))
  {
    name.erase();
    return 0;
  }

  name = what[1];
  return 0;
}

string make_uuid()
{
  return boost::lexical_cast<string>(time());
  // better, for when Boost.Uuid is finished:
  //return boost::uuid::create().to_string();
}

int main()
{
  service s;
  request req(s);

  req.load();

  response resp;

  // If there's already a session id set, warn them and then redirect
  // them to where they would be going anyway.
  if (!req.cookie("uuid").empty()) {
    return show_already_logged_in_page(req, resp);
  }

  // If they haven't asked explicitly to log in, show the default page.
  string cmd(req.POST("cmd"));
  if (cmd.empty() || cmd != "login") {
    return show_default_page(req, resp);
  }

  // If they're name is invalid, inform them.
  string name(req.POST("name"));
  if (!verify_name(name)) {
    return show_name_error_page(req, resp);
  }

  // Else we can assume all is ok.
  // Here we give them a 'universally unique id' and forward them to a
  // cookie checking page.
  resp<< cookie("uuid", make_uuid())
      << location("CheckCookie?fwd=" + req.POST("fwd"));
  resp.send(req.client());

  return req.end(http::ok);
}
