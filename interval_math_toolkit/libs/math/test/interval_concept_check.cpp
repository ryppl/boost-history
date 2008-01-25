//  Copyright John Maddock 2007-8.
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

//
// This tests two things: that boost::interval meets our
// conceptual requirements, and that we can instantiate
// all our distributions and special functions on this type.
//
#define TEST_INTERVAL
#define BOOST_MATH_ASSERT_UNDEFINED_POLICY false

#include <boost/math/bindings/interval.hpp>
#include <boost/math/concepts/real_type_concept.hpp>
#include "compile_test/instantiate.hpp"
#include "handle_test_result.hpp" // for interval_type


void foo()
{
   instantiate(interval_type());
}

int main()
{
   BOOST_CONCEPT_ASSERT((boost::math::concepts::RealTypeConcept<double>));
   BOOST_CONCEPT_ASSERT((boost::math::concepts::RealTypeConcept<interval_type>));
}

