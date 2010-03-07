//////////////////////////////////////////////////////////////////////////////
// assign::detail::auto_size::static_size.hpp                               //
//                                                                          //
//  (C) Copyright 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_DETAIL_AUTO_SIZE_STATIC_SIZE_ER_2010_HPP
#define BOOST_ASSIGN_DETAIL_AUTO_SIZE_STATIC_SIZE_ER_2010_HPP

namespace boost{
namespace assign{
namespace detail{
namespace auto_size{

    template<typename T>
    struct static_size{

        typedef typename T::size_type size_type;
        
        BOOST_STATIC_CONSTANT(
            size_type, 
            value = T::static_size
        );
   };

}// auto_size
}// detail
}// assign
}// boost

#endif

    