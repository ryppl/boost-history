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

#ifndef BOOST_IO_FORMAT_TEST_PAIR_TEST_HPP
#define BOOST_IO_FORMAT_TEST_PAIR_TEST_HPP

#include <string>
#include <boost/io/format/array.hpp>
#include <boost/io/format/std/pair.hpp>

///////////////////////////////////////////////////////////////////////////////
// pair_test
///////////////////////////////////////////////////////////////////////////////

class pair_test
{
public:
  pair_test()
  {
    tests.push_back(test);
  }

  static void test(std::ostream &stream)
  {
    using namespace boost::io;

    typedef std::pair<int,char> pair;

    stream << std::string("Pair test\n")
           << std::string("---------\n\n")
           // << format<pair>("[","]",",")
           << pair(1,'A') << '\n' << '\n';
  }
} pair_test_case;

///////////////////////////////////////////////////////////////////////////////
// array_pair_test
///////////////////////////////////////////////////////////////////////////////

class array_pair_test
{
public:
  array_pair_test()
  {
    tests.push_back(test);
  }

  static void test(std::ostream &stream)
  {
    using namespace boost::io;

    typedef std::pair<int,char> pair;

    pair values[3]=
    {
      pair(1,'A'),
      pair(2,'B'),
      pair(3,'C')
    };

    stream << std::string("Array of pair test\n")
           << std::string("------------------\n\n")
           << format<pair>("[","]",",")
           << format<pair (&)[3]>("(",")"," - ")
           << values << '\n' << '\n';
  }
} array_pair_test_case;

#endif // BOOST_IO_FORMAT_TEST_PAIR_TEST_HPP
