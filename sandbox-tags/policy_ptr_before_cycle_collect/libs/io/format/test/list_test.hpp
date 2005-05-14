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

#ifndef BOOST_IO_FORMAT_TEST_LIST_TEST_HPP
#define BOOST_IO_FORMAT_TEST_LIST_TEST_HPP

#include <string>
#include <boost/io/format/sequence.hpp>
#include <boost/io/format/std/list.hpp>

///////////////////////////////////////////////////////////////////////////////
// list_test
///////////////////////////////////////////////////////////////////////////////

class list_test
{
public:
  list_test()
  {
    tests.push_back(test);
  }

  static void test(std::ostream &stream)
  {
    using namespace boost::io;

    typedef std::list<char> list;

    list values;

    values.push_back('A');
    values.push_back('B');
    values.push_back('C');

    stream << std::string("List test\n")
           << std::string("-----------\n\n")
           << format<list>("[","]",",")
           << values << '\n' << '\n';
  }
} list_test_case;

#endif // BOOST_IO_FORMAT_TEST_LIST_TEST_HPP
