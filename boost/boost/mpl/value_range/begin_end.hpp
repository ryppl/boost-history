//-----------------------------------------------------------------------------
// boost mpl/value_range/begin_end.hpp header file
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

#ifndef BOOST_MPL_VALUE_RANGE_BEGIN_END_HPP
#define BOOST_MPL_VALUE_RANGE_BEGIN_END_HPP

#include "boost/mpl/value_range/tags.hpp"
#include "boost/mpl/value_range/iterator.hpp"
#include "boost/mpl/begin_end.hpp"

namespace boost {
namespace mpl {

// half open range begin/end traits

template<>
struct begin_algorithm_traits<mpl::half_open_range_tag>
{
    template<typename Range> struct algorithm
    {
        typedef typename mpl::value_range_iterator<
                  Range::start
                  > iterator;
    };
};

template<>
struct end_algorithm_traits<mpl::half_open_range_tag>
{
    template<typename Range> struct algorithm
    {
        typedef typename mpl::value_range_iterator<
                  Range::finish - 1
                  > iterator;
    };
};


// closed range begin/end traits

template<>
struct begin_algorithm_traits<mpl::closed_range>
{
    template<typename Range> struct algorithm
    {
        typedef typename mpl::value_range_iterator<
                  Range::start
                  > iterator;
    };
};

template<>
struct end_algorithm_traits<mpl::closed_range>
{
    template<typename Range> struct algorithm
    {
        typedef typename mpl::value_range_iterator<
                  Range::finish
                  > iterator;
    };
};

} // namespace mpl
} // namespace boost 

#endif // BOOST_MPL_VALUE_RANGE_BEGIN_END_HPP
