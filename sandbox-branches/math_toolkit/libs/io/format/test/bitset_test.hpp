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

#ifndef BOOST_IO_FORMAT_TEST_BITSET_TEST_HPP
#define BOOST_IO_FORMAT_TEST_BITSET_TEST_HPP

#include <string>
#include <boost/io/format/std/bitset.hpp>

///////////////////////////////////////////////////////////////////////////////
// bitset_test
///////////////////////////////////////////////////////////////////////////////

class bitset_test
{
public:
  bitset_test()
  {
    tests.push_back(test);
  }

  static void test(std::ostream &stream)
  {
    using namespace boost::io;

    typedef std::bitset<8> bitset;

    bitset value;

    value[0]=1;
    value[1]=0;
    value[2]=1;
    value[3]=0;
    value[4]=1;
    value[5]=1;
    value[6]=0;
    value[7]=0;

    stream << std::string("Bitset test\n")
           << std::string("-----------\n\n")
           << format<bitset>("[","]",",")
           << wrap(value) << '\n' << '\n'; 
  }

} bitset_test_case;

#endif // BOOST_IO_FORMAT_TEST_BITSET_TEST_HPP
