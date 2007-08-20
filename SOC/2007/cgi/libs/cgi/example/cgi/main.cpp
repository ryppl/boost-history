#include <string>
#include <boost/system/error_code.hpp>
#include "../../../../boost/cgi/cgi.hpp"

int main()
{
  try{
  cgi::cgi_request req;

  std::cerr<< "About to write header";

  std::string buf("Content-type: text/html\r\n\r\nHello there, universe!");
  boost::system::error_code ec;

  cgi::write(req, cgi::buffer(buf.c_str(), buf.size()));

  buf = req.meta_get("blah");
  cgi::write(req, cgi::buffer(buf.c_str(), buf.size()));

  } catch( boost::system::error_code& ec ) {
    std::cerr<< "error: " << ec.message() << std::endl;
}
  return 0;
}
