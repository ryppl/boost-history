
#include <boost/cgi/acgi.hpp>

using namespace cgi::acgi;

int main()
{
  service s;
  request req(s);

  req.load();

  response resp;

  resp<< cookie("uuid");
  boost::system::error_code ec;
  resp<< location(req.form["fwd"]);

  return commit(req, resp);
}

