#include <string>
#include <boost/system/error_code.hpp>
#include "../../../../boost/cgi/cgi.hpp"

int main()
{
  try{
  cgi::cgi_request req;

  std::string buf("Content-type: text/html\r\n\r\nHello there, universe!");
  boost::system::error_code ec;

  req.write_some(cgi::buffer(buf.c_str(), buf.size()), ec);

  buf = req.meta_get("blah");
  req.write_some(cgi::buffer(buf.c_str(), buf.size()), ec);

  } catch( boost::system::error_code& ec ) {
    std::cerr<< "error: " << ec.message() << std::endl;
}
  return 0;
}
