// Copyright Kevin Sopp 2009.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MP_MATH_INTEGER_MULTIPRECISION_INTEGER_TAG_HPP
#define BOOST_MP_MATH_INTEGER_MULTIPRECISION_INTEGER_TAG_HPP

namespace boost {
namespace mp_math {

// This exists so that we can remove the
// template<class ApInt> integer_type(const ApInt&)
// constructor from the overload set.

struct multiprecision_integer_tag;

} // namespace mp_math
} // namespace boost

#endif

