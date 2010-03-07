//////////////////////////////////////////////////////////////////////////////
// assign::detail::auto_size::has_static_size.hpp                           //
//                                                                          //
//  (C) Copyright 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_DETAIL_AUTO_SIZE_HAS_STATIC_SIZE_ER_2010_HPP
#define BOOST_ASSIGN_DETAIL_AUTO_SIZE_HAS_STATIC_SIZE_ER_2010_HPP
#include <boost/mpl/bool.hpp>

namespace boost{
namespace assign{
namespace detail{
namespace auto_size{

	// TODO 
    template<typename T>
    struct has_static_size{
/*
       typedef typename T::size_type size_type;
       typedef char yes;
       typedef char (&no)[2];
       typedef const size_type* sig;
//       typedef size_type (const T::*sig)();

       template<typename U,sig>
       struct sfinae { };

       template<typename U> static yes test(sfinae<U, &U::static_size> *);
       //template<typename U> static yes test(sfinae<U,&U::size> *);
       template<typename U> static no test(...);

		BOOST_STATIC_CONSTANT(
        	bool, 
            value = sizeof( test<T>(0) ) == sizeof(yes)
        );
        
        typedef boost::mpl::bool_<value> type;
*/
   };
}
}
}

	template<typename T,std::size_t N>
    struct array;

namespace assign{
namespace detail{
namespace auto_size{

    // temporary fix
    template<typename T,std::size_t N>
    struct has_static_size<boost::array<T,N> > : boost::mpl::bool_<true>{};


}// auto_size
}// detail
}// assign
}// boost

#endif

    