//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_TRAITS_CONTAINER_IS_SORTED_ER_2010_HPP
#define BOOST_ASSIGN_V2_TRAITS_CONTAINER_IS_SORTED_ER_2010_HPP
#include <boost/assign/v2/detail/traits/container/is.hpp>

namespace boost{
namespace assign{
namespace v2{
namespace container_tag{
    struct sorted{};
}// container_tag
namespace container_traits_aux{

    template<typename V>
    struct is_sorted : ::boost::mpl::false_{};
    
    template<typename T,typename C,typename A>
    struct is_sorted< std::set<T,C,A> > : ::boost::mpl::true_{};

    template<typename K,typename T,typename C,typename A>
    struct is_sorted< std::map<K,T,C,A> > : ::boost::mpl::true_{};

}// container_traits_aux


    BOOST_ASSIGN_V2_TRAITS_CONTAINER_IS(sorted)

}// v2
}// assign
}// boost

#endif
