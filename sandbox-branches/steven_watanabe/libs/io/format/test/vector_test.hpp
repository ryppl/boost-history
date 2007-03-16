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

#ifndef BOOST_IO_FORMAT_TEST_VECTOR_TEST_HPP
#define BOOST_IO_FORMAT_TEST_VECTOR_TEST_HPP

#include <string>
#include <boost/io/format/sequence.hpp>
#include <boost/io/format/std/pair.hpp>
#include <boost/io/format/std/vector.hpp>

///////////////////////////////////////////////////////////////////////////////
// vector_test
///////////////////////////////////////////////////////////////////////////////

class vector_test
{
public:
  vector_test()
  {
    tests.push_back(test);
  }

  static void test(std::ostream &stream)
  {
    using namespace boost::io;

    typedef std::vector<char> vector;

    vector values;

    values.push_back('A');
    values.push_back('B');
    values.push_back('C');

    stream << std::string("Vector test\n")
           << std::string("-----------\n\n")
           << format<vector>("[","]",",")
           << values << '\n' << '\n';
  }
} vector_test_case;

///////////////////////////////////////////////////////////////////////////////
// vector_pair_test
///////////////////////////////////////////////////////////////////////////////

class vector_pair_test
{
public:
  vector_pair_test()
  {
    tests.push_back(test);
  }

  static void test(std::ostream &stream)
  {
    using namespace boost::io;

    typedef std::pair<int,char> pair;
    typedef std::vector<pair> vector_pair;

    vector_pair values;

    values.push_back(pair(1,'A'));
    values.push_back(pair(2,'B'));
    values.push_back(pair(3,'C'));

    stream << std::string("Vector of pair test\n")
           << std::string("-------------------\n\n")
           << format<pair>("[","]",",")
           << format<vector_pair>("","","\n")
           << values << '\n' << '\n';
  }
} vector_pair_test_case;

///////////////////////////////////////////////////////////////////////////////
// vector_vector_test
///////////////////////////////////////////////////////////////////////////////

class vector_vector_test
{
public:
  vector_vector_test()
  {
    tests.push_back(test);
  }

  static void test(std::ostream &stream)
  {
    using namespace boost::io;

    typedef std::vector<char> vector;
    typedef std::vector<vector> vector_vector;

    char board[9]=
    {
      'X','O','X',
      'O','X','O',
      'X','O','X'
    };

    vector line1(board,board+3);
    vector line2(board+3,board+6);
    vector line3(board+6,board+9);

    vector_vector board_vector;

    board_vector.push_back(line1);
    board_vector.push_back(line2);
    board_vector.push_back(line3);

    stream << std::string("Vector of vector test\n")
           << std::string("---------------------\n\n")
           << format<vector>("\n|","|\n","|","\'","\'")
           << format<vector_vector>("-------------","-------------","-------------")
           << board_vector << '\n' << '\n';

    stream << std::string("Sequence\n")
           << std::string("--------\n")
           << sequence<vector>(board+0,board+3) << '\n' << '\n';
  }
} vector_vector_test_case;

#endif // BOOST_IO_FORMAT_TEST_VECTOR_TEST_HPP
