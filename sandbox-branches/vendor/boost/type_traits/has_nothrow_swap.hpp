
// (C) Copyright Eric Friedman 2002.
// Permission to copy, use, modify, sell and distribute this software is 
// granted provided this copyright notice appears in all copies. This software 
// is provided "as is" without express or implied warranty, and with no claim 
// as to its suitability for any purpose.
//
// See http://www.boost.org for most recent version including documentation.

#ifndef BOOST_TT_HAS_NOTHROW_SWAP_HPP_INCLUDED
#define BOOST_TT_HAS_NOTHROW_SWAP_HPP_INCLUDED

#include "boost/type_traits/is_scalar.hpp"

// should be the last #include
#include "boost/type_traits/detail/bool_trait_def.hpp"

namespace boost {

BOOST_TT_AUX_BOOL_TRAIT_DEF1(has_nothrow_swap,T,::boost::is_scalar<T>::value)

} // namespace boost

#include "boost/type_traits/detail/bool_trait_undef.hpp"

#endif // BOOST_TT_HAS_NOTHROW_SWAP_HPP_INCLUDED
