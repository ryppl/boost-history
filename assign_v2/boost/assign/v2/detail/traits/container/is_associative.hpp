//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_TRAITS_CONTAINER_IS_ASSOCIATIVE_ER_2010_HPP
#define BOOST_ASSIGN_V2_TRAITS_CONTAINER_IS_ASSOCIATIVE_ER_2010_HPP
#include <boost/mpl/or.hpp>
#include <boost/assign/v2/detail/traits/container/is_map.hpp>
#include <boost/assign/v2/detail/traits/container/is_set.hpp>

namespace boost{
namespace assign{
namespace v2{
namespace container_tag{
    struct associative{};
}// container_tag
namespace container_traits{

    template<typename V>
    struct is_associative : ::boost::mpl::or_<
        container_traits::is_set<V>,
        container_traits::is_map<V>
    >{};

}// container_traits
}// v2
}// assign
}// boost

#endif
