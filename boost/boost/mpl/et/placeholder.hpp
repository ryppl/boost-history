//-----------------------------------------------------------------------------
// boost mpl/et/placeholder.hpp header file
// See http://www.boost.org for updates, documentation, and revision history.
//-----------------------------------------------------------------------------
//
// Copyright (c) 2000-02
// Peter Dimov, Aleksey Gurtovoy
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is hereby granted without fee, 
// provided that the above copyright notice appears in all copies and 
// that both the copyright notice and this permission notice appear in 
// supporting documentation. No representations are made about the 
// suitability of this software for any purpose. It is provided "as is" 
// without express or implied warranty.

#ifndef BOOST_MPL_ET_PLACEHOLDER_HPP_INCLUDED
#define BOOST_MPL_ET_PLACEHOLDER_HPP_INCLUDED

#include "boost/mpl/limits/arity.hpp"

#include "boost/preprocessor/repeat.hpp"
#include "boost/preprocessor/inc.hpp"
#include "boost/preprocessor/cat.hpp"

namespace boost {
namespace mpl {

struct placeholder;

#define BOOST_MPL_AUX_PLACEHOLDER(n, unused) \
struct BOOST_PREPROCESSOR_CAT(_, BOOST_PREPROCESSOR_INC(n)) \
{ \
    typedef BOOST_PREPROCESSOR_CAT(_, BOOST_PREPROCESSOR_INC(n)) type; \
}; \
/**/

// _# placeholders
BOOST_PREPROCESSOR_REPEAT(
      BOOST_MPL_FUNCTION_CLASS_MAX_ARITY
    , BOOST_MPL_AUX_PLACEHOLDER
    , unused
    )

#undef BOOST_MPL_AUX_PLACEHOLDER

} // namespace mpl
} // namespace boost 

#endif // BOOST_MPL_ET_PLACEHOLDER_HPP_INCLUDED
