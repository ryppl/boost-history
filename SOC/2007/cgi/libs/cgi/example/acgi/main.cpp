#include <boost/cgi/acgi.hpp>

int main()
{
  cgi::cgi_service service;
  cgi::acgi_request req(service);

  std::string buf("Content-type:text/html\r\n\r\nHello there, Universe.");
  cgi::write(req, cgi::buffer(buf.c_str(), buf.size()));

  return 0;
}
