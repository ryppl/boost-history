//-----------------------------------------------------------------------------
// boost mpl/list/iterator.hpp header file
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

#ifndef BOOST_MPL_LIST_ITERATOR_HPP
#define BOOST_MPL_LIST_ITERATOR_HPP

#include "boost/mpl/list/traits.hpp"
#include "boost/mpl/select_type.hpp"
#include "boost/config.hpp"

namespace boost {
namespace mpl {

template<typename List> 
struct forward_list_iterator;
struct forward_list_end_iterator;

template<typename List>
struct list_iterator
{
    typedef typename mpl::list_traits_tag<List>::type tag_;
    typedef mpl::list_traits<tag_>                    traits_;
    typedef typename traits_::template is_null<List>  is_null_;
    typedef typename mpl::select_type_t<
                  is_null_
                , forward_list_end_iterator
                , forward_list_iterator<List>
                >::type type;
};

template<typename List>
struct list_end_iterator
{
    typedef forward_list_end_iterator type;
};

#if defined(BOOST_MSVC)
// yet other workaround of MSVC early template instantiation bug
template<>
struct list_iterator<int>
{
    struct type { typedef type next; };
};
#endif

template<typename List>
struct forward_list_iterator
    : mpl::list_traits< typename mpl::list_traits_tag<List>::type >
            ::template value_type_<List>
{
    typedef typename mpl::list_traits_tag<List>::type tag_;
    typedef mpl::list_traits<tag_>                    traits_;
    
    typedef typename list_iterator<
                typename traits_::template next_node<List>::type
                >::type next;
};

struct forward_list_end_iterator
{
    typedef forward_list_end_iterator next;
};

} // namespace mpl
} // namespace boost 

#endif // BOOST_MPL_LIST_ITERATOR_HPP
