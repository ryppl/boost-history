// Copyright Kevin Sopp 2008 - 2009.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/mp_math/gmp.hpp>
#include <boost/mp_math/integer.hpp>
#include <boost/mpl/vector.hpp>

struct IntTypes
:
  boost::mpl::vector<
    boost::mp_math::integer<
      boost::mp_math::gmp_integer<>
    >,
    boost::mp_math::gmp_integer<>
  >
{};

