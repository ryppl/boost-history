//-----------------------------------------------------------------------------
// boost mpl/list/begin_end.hpp header file
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

#ifndef BOOST_MPL_LIST_BEGIN_END_HPP
#define BOOST_MPL_LIST_BEGIN_END_HPP

#include "boost/mpl/begin_end.hpp"
#include "boost/mpl/list/traits.hpp"
#include "boost/mpl/list/iterator.hpp"

namespace boost {
namespace mpl {

template<>
struct begin_algorithm_traits<list_sequence_tag>
{
    template<class List> struct algorithm
    {
        typedef typename mpl::list_iterator<List>::type iterator;
    };
};

template<>
struct end_algorithm_traits<list_sequence_tag>
{
    template<class List> struct algorithm
    {
        typedef typename mpl::list_end_iterator<List>::type iterator;
    };
};

} // namespace mpl
} // namespace boost 

#endif // BOOST_MPL_LIST_BEGIN_END_HPP
