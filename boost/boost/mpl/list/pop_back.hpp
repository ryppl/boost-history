//-----------------------------------------------------------------------------
// boost mpl/list/pop_back.hpp header file
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

#ifndef BOOST_MPL_LIST_POP_BACK_HPP
#define BOOST_MPL_LIST_POP_BACK_HPP

#include "boost/mpl/pop_back.hpp"
#include "boost/mpl/list/traits.hpp"

namespace boost {
namespace mpl {

namespace detail {

template<typename List>
struct list_pop_back_algorithm;

template<bool>
struct list_pop_back_algorithm_part1
{
    template<typename List> struct part2
    {
        typedef typename mpl::list_traits_tag<List>::type       tag;
        typedef typename mpl::list_traits<tag>                  traits;
        typedef typename traits::template next_node<List>::type next_node;
        typedef typename traits::template make_node<
                          typename traits::template value_type_<List>::type
                        , typename list_pop_back_algorithm<next_node>::sequence
                        >::type type;
    };
};

template<>
struct list_pop_back_algorithm_part1<true>
{
    template<typename List> struct part2
    {
        typedef typename mpl::list_traits_tag<List>::type tag;
        typedef typename mpl::list_traits<tag>            traits;
        typedef typename traits::null_node type;
    };
};

template<typename List>
struct list_pop_back_algorithm
{
    typedef typename mpl::list_traits_tag<List>::type        tag_;
    typedef typename mpl::list_traits<tag_>                  traits_;
    typedef typename traits_::template next_node<List>::type next_node_;
    typedef typename traits_::template is_null<next_node_>   is_null_;
    typedef typename mpl::detail::list_pop_back_algorithm_part1< 
                (is_null_::value)
                >::template part2<List>::type sequence;
};

} // namespace detail


template<>
struct pop_back_algorithm_traits<list_sequence_tag>
{
    template<typename List> struct algorithm
        : mpl::detail::list_pop_back_algorithm<List>
    {
    };
};

} // namespace mpl
} // namespace boost

#endif // BOOST_MPL_LIST_POP_BACK_HPP
