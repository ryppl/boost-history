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

#ifndef BOOST_IO_FORMAT_TEST_ARRAY_WIDE_TEST_HPP
#define BOOST_IO_FORMAT_TEST_ARRAY_WIDE_TEST_HPP

#include <string>
#include <boost/io/format/array.hpp>

///////////////////////////////////////////////////////////////////////////////
// array2D_wide_test
///////////////////////////////////////////////////////////////////////////////

class array2D_wide_test
{
public:
  array2D_wide_test()
  {
    tests.push_back(test);
  }

  static void test(std::wostream &stream)
  {
    using namespace boost::io;

    wchar_t board[3][3]=
    {
      {L'O',L'X',L'O'},
      {L'X',L'X',L'O'},
      {L'O',L'O',L'X'}
    };

  stream << std::wstring(L"Array wide character test\n")
         << std::wstring(L"-------------------------\n\n")
         << format<wchar_t (&)[3]>(L"\n|",L"|\n",L"|")
         << format<wchar_t (&)[3][3]>(L"-------",L"-------",L"-------")
         << board << '\n' << '\n';
  }
} array2D_wide_test_case;

#endif // BOOST_IO_FORMAT_TEST_ARRAY_WIDE_TEST_HPP
