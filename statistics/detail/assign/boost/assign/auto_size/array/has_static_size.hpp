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
#include <boost/type_traits/detail/yes_no_type.hpp>

namespace boost{
namespace assign{
namespace detail{
namespace auto_size{

    template<typename T>
    struct has_static_size{
        typedef typename T::size_type size_type;
        typedef typename type_traits::yes_type yes;
        typedef typename type_traits::no_type no;
        typedef const size_type* sig1;
        typedef const size_type sig2; // for boost::array<>

        template<typename U,sig1>
        struct sfinae1 { };

        template<typename U,sig2>
        struct sfinae2 { };

        //template<typename U> static yes test(sfinae1<U, &U::static_size> *);
        template<typename U> static yes test(sfinae2<U, U::static_size> *);
        template<typename U> static no test(...);

        BOOST_STATIC_CONSTANT(
        	bool, 
            value = sizeof( test<T>(0) ) == sizeof(yes)
        );
        
        typedef boost::mpl::bool_<value> type;
   };

}// auto_size
}// detail
}// assign
}// boost

#endif

    