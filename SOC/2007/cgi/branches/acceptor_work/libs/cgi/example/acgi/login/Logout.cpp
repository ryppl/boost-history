
#include <boost/cgi/acgi.hpp>

using namespace cgi::acgi;

int main()
{
  service s;
  request req(s);

  req.load();

  response resp;

  resp<< cookie("uuid")
      << location(req.form("fwd"));

  resp.send(req.client());
  return req.end(http::ok);
}

