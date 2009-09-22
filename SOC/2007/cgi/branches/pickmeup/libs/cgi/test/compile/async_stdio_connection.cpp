#include "boost/cgi/connections/async_stdio.hpp"

int main()
{
  cgi::common::io_service ios;
  cgi::common::connections::async_stdio conn(ios);

  return 0;
}
