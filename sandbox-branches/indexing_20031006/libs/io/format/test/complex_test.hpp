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

#ifndef BOOST_IO_FORMAT_TEST_COMPLEX_TEST_HPP
#define BOOST_IO_FORMAT_TEST_COMPLEX_TEST_HPP

#include <string>
#include <boost/io/format/std/complex.hpp>

///////////////////////////////////////////////////////////////////////////////
// complex_test
///////////////////////////////////////////////////////////////////////////////

class complex_test
{
public:
  complex_test()
  {
    tests.push_back(test);
  }

  static void test(std::ostream &stream)
  {
    using namespace boost::io;

    typedef std::complex<double> complex;

    stream << std::string("Complex test\n")
           << std::string("------------\n\n")
           << format<complex>("[","]",",")
           << wrap(complex(1.23,2.34)) << '\n' << '\n';
  }
} complex_test_case;

#endif // BOOST_IO_FORMAT_TEST_COMPLEX_TEST_HPP
