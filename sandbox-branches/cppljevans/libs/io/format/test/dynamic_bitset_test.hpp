///////////////////////////////////////////////////////////////////////////////
// Output operators and manipulators for composite objects
//
// Copyright Terje Slettebø 2003.
//
// Contributions by Vladimir Prus, Paul A. Bristow and Jason House.
//
// Permission to use, copy, modify, and distribute this software for any
// purpose is hereby granted without fee, provided that this copyright and
// permissions notice appear in all copies and derivatives.
//
// This software is provided "as is" without express or implied warranty.
///////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_IO_FORMAT_TEST_DYNAMIC_BITSET_TEST
#define BOOST_IO_FORMAT_TEST_DYNAMIC_BITSET_TEST

#include <string>
#include <boost/io/format/boost/dynamic_bitset.hpp>

///////////////////////////////////////////////////////////////////////////////
// dynamic_bitset_test
///////////////////////////////////////////////////////////////////////////////

class dynamic_bitset_test
{
public:
  dynamic_bitset_test()
  {
    tests.push_back(test);
  }

  static void test(std::ostream &stream)
  {
    using namespace boost::io;

    typedef boost::dynamic_bitset<> dynamic_bitset;

    dynamic_bitset value(8);

    value[0]=1;
    value[1]=0;
    value[2]=1;
    value[3]=0;
    value[4]=1;
    value[5]=1;
    value[6]=0;
    value[7]=0;

    stream << std::string("Dynamic bitset test\n")
           << std::string("-------------------\n\n")
           << format<dynamic_bitset>("[","]",",")
           << wrap(value) << '\n' << '\n'; 
  }

} dynamic_bitset_test_case;

#endif // BOOST_IO_FORMAT_TEST_DYNAMIC_BITSET_TEST
