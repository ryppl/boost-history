//-----------------------------------------------------------------------------
// boost mpl/aux_/arity.hpp header file
// See http://www.boost.org for updates, documentation, and revision history.
//-----------------------------------------------------------------------------
//
// Copyright (c) 2001-02
// Aleksey Gurtovoy
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is hereby granted without fee, 
// provided that the above copyright notice appears in all copies and 
// that both the copyright notice and this permission notice appear in 
// supporting documentation. No representations are made about the 
// suitability of this software for any purpose. It is provided "as is" 
// without express or implied warranty.

#ifndef BOOST_MPL_AUX_ARITY_HPP_INCLUDED
#define BOOST_MPL_AUX_ARITY_HPP_INCLUDED

#include "boost/config.hpp"

namespace boost {
namespace mpl {
namespace aux {

// agurt, 15/mar/02: it's possible to implement the template so that it will 
// "just work" and do not require any specialization, but MWCW 8.0 fails to 
// compile such implementation as well ('arity' is needed only to workaround 
// yet another CW bug - see 'mpl/apply.hpp')
template< typename F, long N >
struct arity
{
    BOOST_STATIC_CONSTANT(long, value = N);
};

} // namespace aux
} // namespace mpl
} // namespace boost

#endif // #ifndef BOOST_MPL_AUX_ARITY_HPP_INCLUDED
