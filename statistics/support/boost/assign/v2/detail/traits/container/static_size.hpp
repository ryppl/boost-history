//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_TRAITS_CONTAINER_STATIC_SIZE_ER_2010_HPP
#define BOOST_ASSIGN_V2_TRAITS_CONTAINER_STATIC_SIZE_ER_2010_HPP
#include <boost/mpl/size_t.hpp>
#include <boost/assign/v2/detail/fwd/container.hpp>
#include <boost/assign/v2/detail/fwd/ptr_container.hpp>

namespace boost{
namespace assign{
namespace v2{
namespace container_traits{

	template<typename V>
    struct static_size{
    
    	// Needed as a meta-function (indirection) bec. ptr_array
        // does not expose static_size.
    
    };

	template<typename T,std::size_t N>
    struct static_size< boost::array<T,N> >
    	: ::boost::mpl::size_t<
    		boost::array<T,N>::static_size
    	>{};

	template<typename T,std::size_t N,typename C>
    struct static_size< boost::ptr_array<T, N, C> >
    	: ::boost::mpl::size_t<
    		N
    	>{};

}// container_traits
}// v2
}// assign
}// boost

#endif
