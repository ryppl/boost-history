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

#ifndef BOOST_IO_FORMAT_TEST_ARRAY_TEST_HPP
#define BOOST_IO_FORMAT_TEST_ARRAY_TEST_HPP

#include <string>
#include <boost/io/format/array.hpp>

///////////////////////////////////////////////////////////////////////////////
// array_test
///////////////////////////////////////////////////////////////////////////////

class array_test
{
public:
  array_test()
  {
    tests.push_back(test);
  }

  static void test(std::ostream &stream)
  {
    using namespace boost::io;

    int array_int_3[3]={1,2,3};
    double array_double_3[3]={1.1,2.2,3.3};
    int array_int_4[4]={1,2,3,4};
    double array_double_4[4]={1.1,2.2,3.3,4.4};

    stream << std::string("Array test\n")
           << std::string("----------\n\n")
           << std::string("Exact type and size\n\n")
           << format<int (&)[3]>("[","]",",")
           << array_int_3 << '\n'
           << array_double_3 << '\n'
           << array_int_4 << '\n'
           << array_double_4 << '\n'
           << format<int (&)[3],char>(false);

    stream << std::string("\nAny type, exact size\n\n")
           << format<_ (&)[3]>("[","]",",")
           << array_int_3 << '\n'
           << array_double_3 << '\n'
           << array_int_4 << '\n'
           << array_double_4 << '\n'
           << format<_ (&)[3],char>(false);

    stream << std::string("\nExact type, any size\n\n")
           << format<int (&)[N]>("[","]",",")
           << array_int_3 << '\n'
           << array_double_3 << '\n'
           << array_int_4 << '\n'
           << array_double_4 << '\n'
           << format<int (&)[N],char>(false);

    stream << std::string("\nAny type and size\n\n")
           << format<_ (&)[N]>("[","]",",")
           << array_int_3 << '\n'
           << array_double_3 << '\n'
           << array_int_4 << '\n'
           << array_double_4 << '\n'
           << format<_ (&)[N],char>(false);

    stream << std::string("\nAny type\n\n")
           << format<_>("[","]",",")
           << array_int_3 << '\n'
           << array_double_3 << '\n'
           << array_int_4 << '\n'
           << array_double_4 << '\n' << '\n'
           << format<_,char>(false);
  }
} array_test_case;

///////////////////////////////////////////////////////////////////////////////
// array_2d_test
///////////////////////////////////////////////////////////////////////////////

class array2D_test
{
public:
  array2D_test()
  {
    tests.push_back(test);
  }

  static void test(std::ostream &stream)
  {
    using namespace boost::io;

    char board[3][3]=
    {
      {'O','X','O'},
      {'X','X','O'},
      {'O','O','X'}
    };

    stream << std::string("Array 2D test\n")
           << std::string("-------------\n\n")
           << format<char (&)[3]>("\n|","|\n","|","\'","\'")
           << format<char (&)[3][3]>("-------------","-------------","-------------")
           << board << '\n' << '\n';
  }
} array2D_test_case;

///////////////////////////////////////////////////////////////////////////////
// array3D_test
///////////////////////////////////////////////////////////////////////////////

class array3D_test
{
public:
  array3D_test()
  {
    tests.push_back(test);
  }

  static void test(std::ostream &stream)
  {
    using namespace boost::io;

    char boards[3][3][3]=
    {
      {
        {'X','O','X'},
        {'O','X','O'},
        {'X','O','X'}
      },
      {
        {'X','X','X'},
        {'X','O','X'},
        {'X','X','X'}
      },
      {
        {'O','X','O'},
        {'X','X','X'},
        {'O','X','O'}
      }
    };

    stream << std::string("Array 3D test\n")
           << std::string("-------------\n\n")
           << format<char (&)[3]>("\n|","|\n","|")
           << format<char (&)[3][3]>("-------","-------","-------")
           << format<char (&)[3][3][3]>("--- Boards ---\n\n","\n\n--- Boards ---","\n\n")
           << boards << '\n' << '\n';
  }
} array3D_test_case;

#endif // BOOST_IO_FORMAT_TEST_ARRAY_TEST_HPP
