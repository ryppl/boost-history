// (C) Copyright Ronald Garcia 2002. Permission to copy, use, modify, sell and
// distribute this software is granted provided this copyright notice appears
// in all copies. This software is provided "as is" without express or implied
// warranty, and with no claim as to its suitability for any purpose.

//
// bigint_test.cpp - test cases for bigint.
//

#ifdef USE_GMP
#include "boost/bigint_gmp.hpp"
#else
#include "boost/bigint.hpp"
#endif

#include "boost/lexical_cast.hpp"
#include <cassert>
#include <sstream>

int main() {
  using boost::bigint;

  {
    std::ostringstream os;
    bigint x1("100012000230003");
    os << x1;
    assert(os.str() == "100012000230003");
  }


  // RG - factor this big chunk of tests into little tests.
  {
    bigint x1("10001");
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
  
  
    // RG - better tests. Maybe make a table of factorials and run factorial.
    // Maybe run some common summations and compare to the closed form
    // formulae for them
    // implementation detail: bigint is stored in base 10000.
    bigint x6("9999");
    assert(x6 + x1 == bigint("20000"));
  }


  {
    bigint const x1("100000006");
    bigint const x2("300070005");
    assert(x1-x2 == bigint("-200069999"));
  }


  {
    bigint const x("1234567");
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

  // RG - test construction with all sorts of integral types at their 
  // limits: signed and unsigned short, long, int.


  assert(bigint(0) == bigint("0"));
  assert(bigint(5) == bigint("5"));

  assert(!bigint(0));
  assert(!!bigint(5));

  assert(+bigint(5) == bigint(5));


  // Use lexical cast to convert from bigint to other integral types.
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

  // Test swap;
  {
    bigint x(5);
    bigint y(6);
    swap(x,y);
    assert(x == 6);
    assert(y == 5);

    bigint(7).swap(x);
    assert(x == 7);
  }

  // Test out basic streaming
  {
    std::istringstream is("1234567");
    bigint x;
    is >> x;
    assert(x == bigint("1234567"));
  }

  // Eat whitespace!
  {
    std::istringstream i("  555");
    bigint x;
    i >> std::skipws >> x;
    assert(x == 555);
  }

  // Don't eat whitespace!
  {
    std::istringstream i("  555");
    bigint x;
    i >> std::noskipws >> x;
    assert(i.fail());
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

  {
    // check that fill character is reset.
    std::ostringstream o;
    o << std::setfill('x') << bigint(55555) << std::setw(5) << "x";
    assert (o.str() == "55555xxxxx");
  }

  // Use lexical cast to convert from a string of numbers to a bigint.
  // An error in lexical_cast may throw a bad_lexical_cast object.
  try {
    bigint ll = boost::lexical_cast<boost::bigint>("123");
    assert(ll == 123);
  } catch (boost::bad_lexical_cast&) {
    throw;
  }

  // division tests: 
  // y / x = q,
  // y % x = r,
  // where y = qx + r,
  // q = float(y)/float(x) rounded towards 0
  // 0 <= |r| < |q|
  // (this is called truncation division)
  assert(bigint(11) / bigint(3) == bigint(3));
  assert(bigint(11) / bigint(-3) == bigint(-3));
  assert(bigint(-11) / bigint(3) == bigint(-3));
  assert(bigint(-11) / bigint(-3) == bigint(3));

  assert(bigint(11) % bigint(3) == bigint(2));
  assert(bigint(11) % bigint(-3) == bigint(2));
  assert(bigint(-11) % bigint(3) == bigint(-2));
  assert(bigint(-11) % bigint(-3) == bigint(-2));

  // hexadecimal and octal output tests
  {
    std::ostringstream hexstr;
    hexstr << std::hex << bigint(2147483647);
    assert(hexstr.str() == "7fffffff");

    std::ostringstream uphexstr;
    uphexstr << std::hex << std::uppercase << bigint(2147483647);
    assert(uphexstr.str() == "7FFFFFFF");

    std::ostringstream octstr;
    octstr << std::oct << bigint(2147483647);
    assert(octstr.str() == "17777777777");
  }


  // hexadecimal and octal construction tests.
  assert(bigint("7fffffff",bigint::hexadecimal) == 2147483647);
  assert(bigint("17777777777",bigint::octal) == 2147483647);
  assert(bigint("7fffffff",bigint::hexadecimal) ==
         bigint("7FFFFFFF",bigint::hexadecimal));

  // hexadecimal and octal stream input tests.
  {
    bigint h;
    std::istringstream hexstr("7fffffff");
    hexstr >> std::hex >> h;
    assert(h == 2147483647);
    
    bigint o;
    std::istringstream octstr("17777777777");
    octstr >> std::oct >> o;
    assert(o == 2147483647);
  }

}
