// Copyright David Abrahams 2008. Distributed under the Boost
// Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/move/detail/has_swap_overload.hpp>
#include <boost/mpl/assert.hpp>
#include <complex>
#include <vector>

namespace my
{
  struct X
  {
      friend void swap(X&, X&) {}
  };

  struct Y {};

  struct Z
    : std::complex<double>
  {
      friend void swap(Z&, Z&) {}
  };
};

int main()
{
    BOOST_MPL_ASSERT((boost::detail::has_swap_overload<my::X>));
    BOOST_MPL_ASSERT_NOT((boost::detail::has_swap_overload<my::Y>));
    
    // Wrong answer expected here; std is an associated namespace so
    // we have to be conservative
    BOOST_MPL_ASSERT_NOT((boost::detail::has_swap_overload<my::Z>));
    
    BOOST_MPL_ASSERT_NOT((boost::detail::has_swap_overload<std::complex<double> >));

    // The library has a specialization for this one.
    BOOST_MPL_ASSERT((boost::detail::has_swap_overload<std::vector<int> >));
}
