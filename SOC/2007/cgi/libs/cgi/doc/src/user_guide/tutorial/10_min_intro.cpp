#include <string>
#include <boost/cgi/cgi.hpp>
using namespace cgi;

//[main
/*` It is assumed you have included <boost/cgi/cgi.hpp> and are `using namespace cgi`. */
int main(int,char**)
{
  request req;

  /*` At this point, the environment variables are accessible. This includes cookie and form variables too, which by default are all parsed.

 The `reply` class provides a streaming interface for writing replies. You ['can] write to the request object directly, but for now we're going to just use the `reply`, which is simpler. Writing to a `reply` is buffered - whereas writing to the request directly isn't - so if an error occurs, you can simply `clear()` the reply and send an error message, which is much cleaner than sending half a reply to the client, followed by "... Sorry, I just broke!".
  */
  reply rep;

  /*` Say now you want to check if the user has a cookie, "user_name", set. We get at it like this: */
  std::string user_name( req.meta_cookie("user_name") );

  /*` If it's set, we'll be polite and say hello. */
  if (!user_name.empty())
  {
    rep<< "Hello there, " << user_name << ". How are you?";

    /*` That's all we want to say for now, so just send this back and quit. */
    rep.send(req);

    return 0;
  }

  /*` If the cookie isn't set, we will check if the user has posted a __GET__/__POST__ form with their name. */
  user_name = req.meta_form("user_name");

  if (!user_name.empty())
  {
    /*` If they have told us their name, we should set a cookie so we remember it next time. Then we can say hello and exit. There are two ways to set a cookie: either directly using `req.set_cookie("user_name", user_name)` or the method below. Again, the request object isn't buffered, so we are going to keep using the `reply` in case something breaks and we end up not wanting to set the cookie. The cookie we set below will expire when the user closes their browser.
    */
    rep<< cookie("user_name", user_name)
       << "Hello there, " << user_name << ". You're new around here.";

    rep.send(req);
  }

  /*` If we have no idea who they are, we'll send a form asking them for their name. The default `"Content-type"` header is `"text/plain"`, we'll change this to `"text/html"` so the user's browser will display the form. You can do this using `req.set_header("Content-type", "text/html")` or `rep<< header("Content-type", "text/html")`. Since writing with raw strings is error-prone, the shortcut below is available. */
  rep<< content_type("text/html")
     << "Hello there. What's your name?" "<p />"
        "<form method='POST'>"
        "<input type='text' name='user_name' />"
        "<input type='submit' />";

  rep.send(req);

  /*` And that's all we want to do for now, so we can close the program. */
  return 0;
}
//]
