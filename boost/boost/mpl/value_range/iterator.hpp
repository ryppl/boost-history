//-----------------------------------------------------------------------------
// boost mpl/value_range/iterator.hpp header file
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

#ifndef BOOST_MPL_VALUE_RANGE_ITERATOR_HPP
#define BOOST_MPL_VALUE_RANGE_ITERATOR_HPP

#include "boost/int_t.hpp"
#include "boost/config.hpp"

namespace boost {
namespace mpl {

// TODO - more restricted iterator implementation needed, in sense that it
// should catch errors like incrementing/dereferencing the last iterator in
// the range or decrementing the first one

template<long N>
struct value_range_iterator
{
    BOOST_STATIC_CONSTANT(long, value = N);
    typedef mpl::int_t<N> type;

    typedef value_range_iterator<N + 1> next;
    typedef value_range_iterator<N - 1> prior;
};

struct value_range_end_iterator;

} // namespace mpl
} // namespace boost 

#endif // BOOST_MPL_VALUE_RANGE_ITERATOR_HPP
