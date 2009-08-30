#ifndef BOOST_MPL_PACKAGE_C_HPP_INCLUDED
#define BOOST_MPL_PACKAGE_C_HPP_INCLUDED
//  (C) Copyright Larry Evans 2009.
//
//  Permission to copy, use, modify, sell and distribute this software
//  is granted provided this copyright notice appears in all copies.
//  This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.
//
#include <boost/mpl/package.hpp>
#include <boost/mpl/integral_c.hpp>

namespace boost
{
namespace mpl
{
  template
  < typename Integral
  , Integral... Args
  >
  struct
package_c
  : package<integral_c<Integral,Args>...>
{
    typedef Integral value_type;
    typedef package_c type;
};
      
}}//exit boost::mpl
#endif

