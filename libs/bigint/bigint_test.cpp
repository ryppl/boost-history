// (C) Copyright Ronald Garcia 2002. Permission to copy, use, modify, sell and
// distribute this software is granted provided this copyright notice appears
// in all copies. This software is provided "as is" without express or implied
// warranty, and with no claim as to its suitability for any purpose.

//
// bigint_test.cpp - test cases for bigint.
//


#include "boost/bigint.hpp"
#include "boost/lexical_cast.hpp"
#include <cassert>
#include <sstream>

int main() {
  using boost::bigint;

  std::ostringstream os;
  bigint x1("10001");
  os << x1;
  assert(os.str() == "10001");

  bigint x2(x1);
  assert(x1 == x2);
 
  bigint x3("10001");
  assert(x2 == x3);

  bigint x4; // = 0
  assert(x2 != x4);

  bigint x5("0");
  assert(x4 == x5);

  assert(bigint().is_zero());
  assert(bigint("0").is_zero());
  assert(bigint("10001").positive());
  assert(bigint("-50005").negative());
  assert(!bigint("50").is_zero());
  assert(!bigint().negative());
  assert(!bigint().positive());
  assert(!bigint("50005").negative());
  assert(!bigint("-50005").positive());

  assert(x1 + x1 == bigint("20002"));


  assert( bigint("-10002") < bigint("-10001") );
  assert( bigint("-10001") < bigint("10001") );
  assert( bigint("-10001") < bigint("1") );
  assert( !(bigint("1") < bigint("-10001")) );
  
  

  // implementation detail: bigint is stored in base 10000.
  bigint x6("9999");
  assert(x6 + x1 == bigint("20000"));

  {
    bigint x1("100000006");
    bigint x2("300070005");
    assert(x1-x2 == bigint("-200069999"));
  }


  {
    bigint x("1234567");
    assert(x*x == bigint("1524155677489"));
  }

  {
    bigint x("1524155677489");
    assert(x/bigint("1234567") == bigint("1234567"));
  }

  {
    bigint x("1524155677490");
    assert((x %= bigint("1234567")) == bigint(1));
  }

  assert( bigint(1234567) == bigint("1234567") );


  assert( ++bigint("5") == bigint("6"));
  assert( --bigint("5") == bigint("4"));


  {
    std::istringstream is("1234567");
    bigint x;
    is >> x;
    assert(x == bigint("1234567"));
  }


  assert(!bigint(0));
  assert(!!bigint(5));

  assert(+bigint(5) == bigint(5));


  {
  int x = boost::lexical_cast<int>(bigint(5));
  assert(x == 5);
  }

  {
    try {
      int x = boost::lexical_cast<int>(bigint("1524155677490"));
      assert(0); // shouldn't get here!
      x = 0; // shouldn't get here either!
    } catch (boost::bad_lexical_cast&) {
    }
  }

  {
    bigint x(5);
    bigint y(6);
    swap(x,y);
    assert(x == 6);
    assert(y == 5);
  }

  {
    std::istringstream i("555");
    bigint x;
    i >> x;
    assert(x == 555);
  }

  {
    std::istringstream i("-555");
    bigint x;
    i >> x;
    assert(x == -555);
  }

  {
    // check for an error
    std::istringstream i("x555");
    bigint x;
    i >> x;
    assert(i.fail());
  }

}
