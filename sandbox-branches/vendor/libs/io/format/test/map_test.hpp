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

#ifndef BOOST_IO_FORMAT_TEST_MAP_TEST_HPP
#define BOOST_IO_FORMAT_TEST_MAP_TEST_HPP

#include <string>
#include <boost/io/format/std/map.hpp>

///////////////////////////////////////////////////////////////////////////////
// map_test
///////////////////////////////////////////////////////////////////////////////

class map_test
{
public:
  map_test()
  {
    tests.push_back(test);
  }

  static void test(std::ostream &stream)
  {
    using namespace boost::io;

    typedef std::map<int,char> map;
    typedef map::value_type value_type;

    map values;

    values.insert(value_type(1,'A'));
    values.insert(value_type(2,'B'));
    values.insert(value_type(3,'C'));

    stream << std::string("Map test\n")
           << std::string("-----------\n\n")
           << format<value_type>("[","]\n"," => ")
           << format<map>("","","")
           << values << '\n' << '\n';
  }
} map_test_case;

///////////////////////////////////////////////////////////////////////////////
// multimap_test
///////////////////////////////////////////////////////////////////////////////

class multimap_test
{
public:
  multimap_test()
  {
    tests.push_back(test);
  }

  static void test(std::ostream &stream)
  {
    using namespace boost::io;

    typedef std::multimap<int,char> multimap;
    typedef multimap::value_type value_type;

    multimap values;

    values.insert(value_type(1,'A'));
    values.insert(value_type(2,'B'));
    values.insert(value_type(3,'C'));

    stream << std::string("Multimap test\n")
           << std::string("-----------\n\n")
           << format<value_type>("[","]\n"," => ")
           << format<multimap>("","","")
           << values << '\n' << '\n';
  }
} multimap_test_case;

#endif // BOOST_IO_FORMAT_TEST_MAP_TEST_HPP
