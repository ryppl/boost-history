
#include <boost/cgi/acgi.hpp>

using namespace boost::acgi;

int main()
{
  service s;
  request req(s);

  req.load();

  response resp;

  if (!req.cookie("uuid").empty())
  { // The cookie has been set correctly!
    resp<< location(req.form("fwd"));
  }else
  {
    resp
    << content_type("text/html")
    << "<html>"
       "<head>"
         "<title>Error</title>"
       "</head>"
       "<body>"
       "<center>"
         "<p>You have cookies disabled. They are required for logging in.</p>"
				 "<a href='http://www.google.com/search?q=enabling cookies'>Google it</a>"
				 " if you're stuck, or return to "
         "<a href='" << req.env("referrer") << "'>where you came from</a>"
       "</center>"
       "</body>"
       "</html>";
  }

  resp.send(req.client());

  return req.end(http::ok);
}

