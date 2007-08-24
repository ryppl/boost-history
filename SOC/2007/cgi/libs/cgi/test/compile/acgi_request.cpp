//#include "boost/cgi/io_service.hpp"
#include <boost/cgi/acgi.hpp>
//#include "boost/cgi/cgi/acgi_request.hpp"

int main()
{
  cgi::acgi::service service;
  cgi::acgi::request req(service);

  return 0;
}
