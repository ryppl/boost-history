//-----------------------------------------------------------------------------
// boost mpl/list/pop_front.hpp header file
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

#ifndef BOOST_MPL_LIST_POP_FRONT_HPP
#define BOOST_MPL_LIST_POP_FRONT_HPP

#include "boost/mpl/pop_front.hpp"
#include "boost/mpl/list/traits.hpp"

namespace boost {
namespace mpl {

template<>
struct pop_front_algorithm_traits<list_sequence_tag>
{
    template<typename List> struct algorithm
    {
        typedef typename mpl::list_traits_tag<List>::type tag_;
        typedef typename mpl::list_traits<tag_>           traits_;

        typedef typename traits_::template next_node<List>::type sequence;
    };
};

} // namespace mpl
} // namespace boost

#endif // BOOST_MPL_LIST_POP_FRONT_HPP
