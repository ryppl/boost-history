//-----------------------------------------------------------------------------
// boost mpl/value_range/contains.hpp header file
// See http://www.boost.org for updates, documentation, and revision history.
//-----------------------------------------------------------------------------
//
// Copyright (c) 2000-01
// Aleksey Gurtovoy
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is hereby granted without fee, 
// provided that the above copyright notice appears in all copies and 
// that both the copyright notice and this permission notice appear in 
// supporting documentation. No representations are made about the 
// suitability of this software for any purpose. It is provided "as is" 
// without express or implied warranty.

#ifndef BOOST_MPL_VALUE_RANGE_CONTAINS_HPP
#define BOOST_MPL_VALUE_RANGE_CONTAINS_HPP

#include "boost/mpl/contains.hpp"
#include "boost/mpl/front.hpp"
#include "boost/mpl/back.hpp"
#include "boost/mpl/value_range/tags.hpp"
#include "boost/mpl/logical/not.hpp"
#include "boost/mpl/int_t.hpp"
#include "boost/config.hpp"

namespace boost {
namespace mpl {

template<>
struct contains_algorithm_traits<mpl::value_range_sequence_tag>
{
    template<typename Range, typename N> struct algorithm
    {
        typedef typename mpl::logical_not< mpl::logical_or<
                    mpl::int_t<(N::value < mpl::front<Range>::value)>
                  , mpl::int_t<(mpl::back<Range>::value < N::value)>
                  > >::type type;
        
        BOOST_STATIC_CONSTANT(bool, value = type::value);
    };
};

} // namespace mpl
} // namespace boost 

#endif // BOOST_MPL_VALUE_RANGE_CONTAINS_HPP
