// Copyright David Abrahams 2004. Distributed under the Boost
// Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/has_swap.hpp>
#include <boost/static_assert.hpp>
#include <algorithm> // get std::swap in the mix

// Not worth testing in this case
#ifndef BOOST_NO_ARGUMENT_DEPENDENT_LOOKUP
namespace boost
{
  struct X {};
  struct Y {};
  void swap(X&, X&);
}

namespace other
{
  struct X {};
  struct Y {};
  void swap(X&, X&);
}

BOOST_STATIC_ASSERT(boost::has_swap<boost::X>::value);
BOOST_STATIC_ASSERT(boost::has_swap<other::X>::value);

BOOST_STATIC_ASSERT(!boost::has_swap<boost::Y>::value);
BOOST_STATIC_ASSERT(!boost::has_swap<other::Y>::value);
#endif 
