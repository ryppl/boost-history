//////////////////////////////////////////////////////////////////////////////
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

#ifndef BOOST_IO_FORMAT_TEST_SET_TEST_HPP
#define BOOST_IO_FORMAT_TEST_SET_TEST_HPP

#include <string>
#include <boost/io/format/std/set.hpp>

///////////////////////////////////////////////////////////////////////////////
// set_test
///////////////////////////////////////////////////////////////////////////////

class set_test
{
public:
  set_test()
  {
    tests.push_back(test);
  }

  static void test(std::ostream &stream)
  {
    using namespace boost::io;

    typedef std::set<char> set;

    set values;

    values.insert('A');
    values.insert('B');
    values.insert('C');

    stream << std::string("Set test\n")
           << std::string("-----------\n\n")
           << format<set>("[","]",",")
           << values << '\n' << '\n';
  }
} set_test_case;

///////////////////////////////////////////////////////////////////////////////
// multiset_test
///////////////////////////////////////////////////////////////////////////////

class multiset_test
{
public:
  multiset_test()
  {
    tests.push_back(test);
  }

  static void test(std::ostream &stream)
  {
    using namespace boost::io;

    typedef std::multiset<char> multiset;

    multiset values;

    values.insert('A');
    values.insert('B');
    values.insert('C');

    stream << std::string("Multiset test\n")
           << std::string("-----------\n\n")
           << format<multiset>("[","]",",")
           << values << '\n' << '\n';
  }
} multiset_test_case;

#endif // BOOST_IO_FORMAT_TEST_SET_TEST_HPP
