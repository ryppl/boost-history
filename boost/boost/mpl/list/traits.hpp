//-----------------------------------------------------------------------------
// boost mpl/list/traits.hpp header file
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

#ifndef BOOST_MPL_LIST_TRAITS_HPP
#define BOOST_MPL_LIST_TRAITS_HPP

#include "boost/mpl/list/node.hpp"
#include "boost/config.hpp"

namespace boost {
namespace mpl {

struct list_sequence_tag;

template<typename List>
struct list_traits_tag
{
    typedef typename List::traits_type::tag_type type;
};

template<typename Tag>
struct list_traits
{
    typedef Tag tag_type;
    typedef mpl::null_node< list_sequence_tag, list_traits<Tag> > null_node;
    
    template<typename Node>
    struct is_null
    {
        BOOST_STATIC_CONSTANT(bool, value = Node::is_null);
    };
        
    template<typename Node>
    struct value_type_
    {
        typedef typename Node::value_type type;
    };
       
    template<typename Node>
    struct next_node
    {
        typedef typename Node::next_node type;
    };

    template<typename T, typename NextNode>
    struct make_node
    {
        typedef mpl::list_node< T
                              , NextNode
                              , list_sequence_tag
                              , list_traits<Tag>
                              > type;
    };
};

// an expected "normal" inheritance chain would be 
// struct list_traits : basic_list_traits<Tag>
// but that causes problems with MSVC, so have to inherit in the opposite way
// and redefine the members spoiled by instantiation with Tag == int

template<typename Tag>
struct basic_list_traits : list_traits<int>
{
    typedef Tag tag_type;
    typedef mpl::null_node< list_sequence_tag, list_traits<Tag> > null_node;

    template<typename T, typename NextNode>
    struct make_node
    {
        typedef mpl::list_node< T
                              , NextNode
                              , list_sequence_tag
                              , list_traits<Tag>
                              > type;
    };
};

} // namespace mpl
} // namespace boost 

#endif // BOOST_MPL_LIST_TRAITS_HPP
