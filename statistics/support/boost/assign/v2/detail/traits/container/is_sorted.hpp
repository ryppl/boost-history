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
#include <boost/mpl/bool.hpp>
#include <boost/type_traits/remove_cv.hpp>
#include <boost/assign/v2/detail/fwd/container.hpp>
#include <boost/assign/v2/detail/traits/container/forward_to_value.hpp>

namespace boost{
namespace assign{
namespace v2{
namespace container_tag{
	struct sorted{};
}// container_tag
namespace container_traits{

	template<typename V>
    struct is_sorted_impl : boost::mpl::false_{};
    
    template<typename T,typename C,typename A>
    struct is_sorted_impl< std::set<T,C,A> > : boost::mpl::true_{};

    template<typename K,typename T,typename C,typename A>
    struct is_sorted_impl< std::map<K,T,C,A> > : boost::mpl::true_{};

    template<typename V>
    struct is_sorted : forward_to_value<
    	is_sorted_impl,
    	typename boost::remove_cv<V>::type
    >{};

}// container_traits
}// v2
}// assign
}// boost

#endif
