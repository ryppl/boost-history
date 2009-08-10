#include <string>
#include <iostream>
#include <boost/regex.hpp>

using namespace std;
using namespace boost;

regex *re = new regex(".*ball.*");

int main(int argc, char **argv) {
  string s = "/sports/basketball";
  regex rn(re->str(),re->flags());
  if (regex_match(s,rn) == true) {
    std::cout << "matched\n";
  } else
    std::cout << "not matched\n";
  return 0;
}
