#include <boost/lambda/lambda.hpp>
#include <algorithm>
#include <iostream>
using namespace std;
using namespace boost::lambda;

int a[] = {1,2,3,4};
void d(int a) {
  cout << a << endl;
}

int main(int argc, char **argv) {
  for_each(a, a+4, (_1, cout << _1 << '\n'));
  return 0;
}

