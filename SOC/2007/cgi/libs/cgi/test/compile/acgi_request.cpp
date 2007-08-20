//#include "boost/cgi/io_service.hpp"
#include "boost/cgi/cgi/cgi_service.hpp"
#include "boost/cgi/cgi/acgi_request.hpp"

int main()
{
  cgi::acgi_service service;
  cgi::acgi_request req(service);

  return 0;
}
