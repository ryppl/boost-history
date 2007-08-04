#include "boost/cgi/reply.hpp"

int main(int, char**)
{
  cgi::reply rep;

  rep<< "blah";

  return 0;
}
