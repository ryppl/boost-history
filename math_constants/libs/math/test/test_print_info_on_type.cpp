// Copyright John Maddock 2010.

// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/math/constants/info.hpp>
#include <boost/math/concepts/real_concept.hpp>


int main()
{
   boost::math::constants::print_info_on_type<float>();
   boost::math::constants::print_info_on_type<double>();
   boost::math::constants::print_info_on_type<long double>();
   boost::math::constants::print_info_on_type<boost::math::concepts::real_concept>();

   return 0;
}

