//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_TRAITS_CONTAINER_HAS_STATIC_SIZE_ER_2010_HPP
#define BOOST_ASSIGN_V2_TRAITS_CONTAINER_HAS_STATIC_SIZE_ER_2010_HPP
#include <boost/assign/v2/detail/fwd/ptr_container.hpp>
#include <boost/mpl/bool.hpp>

namespace boost{
namespace assign{
namespace v2{
namespace container_traits{

	template<typename T>
	struct has_static_size{
        typedef typename T::size_type size_type;
        typedef char yes;
        typedef char (&no)[2];
		typedef const size_type sig;

        template<typename U,sig> struct sfinae { };
		template<typename U> static yes test(sfinae<U, U::static_size>*);
        template<typename U> static no test(...);

        BOOST_STATIC_CONSTANT(
            bool,
            value = sizeof( test<T>(0) ) == sizeof( yes )
         );

         typedef boost::mpl::bool_<value> type;
    }; 
            
	template<typename T,std::size_t N,typename C>
    struct has_static_size< boost::ptr_array<T, N, C> > : boost::mpl::true_{};

}// container_traits
}// v2
}// assign
}// boost 

#endif
