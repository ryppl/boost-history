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

#ifdef __INTEL_COMPILER
#pragma warning(disable: 304 383 444 981 1418 1419)
#endif

#include <iostream>
#include <ostream>
#include <vector>
#include <algorithm>
#include <boost/bind.hpp>
#include <boost/bind/apply.hpp>
#include <boost/ref.hpp>

typedef void (*test_function_wide_ptr)(std::wostream &);

std::vector<test_function_wide_ptr> tests;

#include <libs/io/format/test/array_wide_test.hpp>

int main()
{
  std::wostream &stream=std::wcout;

  std::for_each(tests.begin(),tests.end(),
    boost::bind(boost::apply<void>(),_1,boost::ref(stream)));
}
