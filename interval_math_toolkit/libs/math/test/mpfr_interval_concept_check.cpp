//  Copyright John Maddock 2007-8.
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

//
// This tests two things: that boost::interval meets our
// conceptual requirements, and that we can instantiate
// all our distributions and special functions on this type.
//
#define TEST_MPFR
#define BOOST_MATH_ASSERT_UNDEFINED_POLICY false

#pragma warning(disable:4800)

#include <boost/math/bindings/interval.hpp>
#include <boost/math/bindings/mpfr_interval.hpp>
#include <boost/math/concepts/real_type_concept.hpp>
#include "compile_test/instantiate.hpp"


void foo()
{
   instantiate(boost::math::mpfr_interval());
}

int main()
{
   BOOST_CONCEPT_ASSERT((boost::math::concepts::RealTypeConcept<boost::math::mpfr_interval>));
}

