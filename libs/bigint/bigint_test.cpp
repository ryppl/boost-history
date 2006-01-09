// Use, modification and distribution is subject to the Boost Software 
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  Bigint Library
//  Authors: Ronald Garcia
//           Andrew Lumsdaine

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
#include <iomanip>
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
    // check eof-ness
    bigint x;
    std::istringstream i1("555");
    i1 >> x;
    assert(!i1.fail());
    assert(i1.eof());
    std::istringstream i2("555 ");
    assert(!i2.eof() && !i2.fail()) ;
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

  // test out showpos
  {
    bigint x(78888888);
    std::ostringstream o1;
    o1 << std::showpos << x;
    assert(o1.str() == "+78888888");

    bigint y(-78888888);
    std::ostringstream o2;
    o2 << std::showpos << y;
    assert(o2.str() == "-78888888");

  }

  // test out showbase
  {
    bigint x(2147483647);
    std::ostringstream h1, h2, o1;
    h1 << std::hex << std::showbase << x;
    assert(h1.str() == "0x7fffffff");
    h2 << std::hex << std::showbase << std::uppercase << x;
    assert(h2.str() == "0X7FFFFFFF");
    o1 << std::oct << std::showbase << x;
    assert(o1.str() == "017777777777");
  }

  {
    // check width and fill
    std::ostringstream o1;
    o1 << std::setfill('x') << std::setw(10) << bigint(55555);
    assert (o1.str() == "xxxxx55555");

    std::ostringstream o2;
    o2 << std::setfill('x') << std::setw(10) << std::left << bigint(55555);
    assert (o2.str() == "55555xxxxx");

    std::ostringstream o3;
    o3 << std::setw(3) << bigint(12345);
    assert (o3.str() == "12345");
  }

  {
    // exception test. user fail exception should not go off
    std::istringstream is("555");
    is.exceptions(std::ios_base::failbit);
    bigint x;
    try {
      is >> x;
    } catch (std::ios_base::failure&) {
      assert(0); // shouldn't get here
    }
  }

  {
    // eof exception SHOULD go off.
    std::istringstream is("555");
    is.exceptions(std::ios_base::eofbit);
    bigint x;
    try {
      is >> x;
      assert(0);
    } catch (std::ios_base::failure&) { }
  }
   
  // bad_value exception SHOULD happen
  try {
    bigint x("555y");
  } catch (bigint::bad_value& q) {
    assert(q.what() == std::string("Bad value: 555y"));
  }

  // bad_value is-a std::runtime exception.
  try {
    bigint x("555y");
  } catch (std::runtime_error& q) {
    assert(q.what() == std::string("Bad value: 555y"));
  }


  {
    // test out reading hex numbers with a base starter.
    bigint x("0x20",bigint::hexadecimal);
    assert(x == 32);
  }

  {
    // test out reading oct numbers with a base starter.
    bigint x("020",bigint::octal);
    assert(x == 16);
  }

  {
    // test out streaming in hex numbers with a base starter.
    bigint x;
    std::istringstream is("0x20");
    is >> std::hex >> x;
    assert(x == 32);
  }

  {
    // test out streaming in oct numbers with a base starter.
    bigint x;
    std::istringstream is("020");
    is >> std::oct >> x;
    assert(x == 16);
  }

  {
    // Don't read invalid digits!
    bigint x;
    std::istringstream is("0208");
    is >> std::oct >> x;
    assert(x == 16);
    is >> std::dec >> x;
    assert(x == 8);
  }

  // Test out bad reads of numbers (watch out for eofs!)
  {
    bigint x;
    std::istringstream i1("-");
    std::istringstream i2("0x");
    std::istringstream i3("-0x");
    std::istringstream i4("");

    i1 >> x;
    assert(i1.fail());
    i2 >> std::hex >> x;
    assert(i2.fail());
    i3 >> std::hex >> x;
    assert(i3.fail());
    i4 >> x;
    assert(i4.fail());
  }

  // Tests given by Weipeng Liu
  {
    bigint i = 1001;
    bigint j = 10000;
    assert(i<j);
  }
  {
    bigint i = 1;
    bigint j = -1;
    assert(i*j == -1);
  }

}
