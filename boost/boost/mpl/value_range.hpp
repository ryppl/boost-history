//-----------------------------------------------------------------------------
// boost mpl/value_range.hpp header file
// See http://www.boost.org for updates, documentation, and revision history.
//-----------------------------------------------------------------------------
//
// Copyright (c) 2001
// Aleksey Gurtovoy
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is hereby granted without fee, 
// provided that the above copyright notice appears in all copies and 
// that both the copyright notice and this permission notice appear in 
// supporting documentation. No representations are made about the 
// suitability of this software for any purpose. It is provided "as is" 
// without express or implied warranty.

// (C) Copyright Jeremy Siek and David Abrahams 2000-2001. Permission to copy,
// use, modify, sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided "as is"
// without express or implied warranty, and with no claim as to its suitability
// for any purpose.

#ifndef BOOST_MPL_VALUE_RANGE_HPP
#define BOOST_MPL_VALUE_RANGE_HPP

#include "boost/mpl/value_range/tags.hpp"
#include "boost/mpl/value_range/algorithm.hpp"
#include "boost/config.hpp"

namespace boost {
namespace mpl {

template<long Start, long Finish, class Tag = mpl::half_open_range_tag>
struct value_range
{
    typedef mpl::value_range_sequence_tag sequence_category;
    typedef Tag tag_type;
    BOOST_STATIC_CONSTANT(long, start  = Start);
    BOOST_STATIC_CONSTANT(long, finish = Finish);
};

} // namespace mpl
} // namespace boost 

#endif // BOOST_MPL_VALUE_RANGE_HPP
