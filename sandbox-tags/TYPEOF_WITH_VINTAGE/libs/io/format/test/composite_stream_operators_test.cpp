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

typedef void (*test_function_ptr)(std::ostream &);

std::vector<test_function_ptr> tests;

#include <libs/io/format/test/array_test.hpp>
#include <libs/io/format/test/pair_test.hpp>
#include <libs/io/format/test/complex_test.hpp>
#include <libs/io/format/test/vector_test.hpp>
#include <libs/io/format/test/list_test.hpp>
#include <libs/io/format/test/deque_test.hpp>
#include <libs/io/format/test/set_test.hpp>
#include <libs/io/format/test/map_test.hpp>
#include <libs/io/format/test/bitset_test.hpp>
#include <libs/io/format/test/dynamic_bitset_test.hpp>

int main()
{
  using namespace boost::io;

  // Unset default

  format<_>("","","").set_default_format();

  std::ostream &stream=std::cout;

  std::for_each(tests.begin(),tests.end(),
    boost::bind(boost::apply<void>(),_1,boost::ref(stream)));
}
