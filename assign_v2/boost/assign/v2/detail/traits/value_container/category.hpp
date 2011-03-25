//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_TRAITS_VALUE_CONTAINER_CATEGORY_ER_2010_HPP
#define BOOST_ASSIGN_V2_TRAITS_VALUE_CONTAINER_CATEGORY_ER_2010_HPP
#include <boost/mpl/bool.hpp>
#include <boost/assign/v2/detail/traits/value_container/fwd.hpp>

namespace boost{
namespace assign{
namespace v2{
namespace container_aux{

    // FIFO
    template<typename C> struct is_fifo : ::boost::mpl::false_{};

    template<typename T, typename A>
    struct is_fifo< std::queue<T, A> > : ::boost::mpl::true_{};

    // LIFO
    template<typename V>
    struct is_lifo : ::boost::mpl::false_{};

    template<typename T, typename A>
    struct is_lifo< std::stack<T,A> > : ::boost::mpl::true_{};

    // IS_SORTED
    template<typename V>
    struct is_sorted : ::boost::mpl::false_{};

    template<typename T, typename C, typename A>
    struct is_sorted< std::set<T, C, A> > : ::boost::mpl::true_{};

    template<typename K, typename T, typename C, typename A>
    struct is_sorted< std::map<K, T, C, A> > : ::boost::mpl::true_{};
    
    // IS_ARRAY
    template<typename C> struct is_array : ::boost::mpl::false_{};
    template<typename T, std::size_t sz> struct is_array<
        boost::array<T, sz>
    > : ::boost::mpl::true_{};


}// container_aux
}// v2
}// assign
}// boost

#endif // BOOST_ASSIGN_V2_TRAITS_VALUE_CONTAINER_CATEGORY_ER_2010_HPP
