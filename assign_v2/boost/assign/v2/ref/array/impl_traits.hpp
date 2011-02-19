//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_REF_ARRAY_IMPL_TRAITS_ER_2010_HPP
#define BOOST_ASSIGN_V2_REF_ARRAY_IMPL_TRAITS_ER_2010_HPP
#include <boost/array.hpp>
#include <boost/assign/v2/ref/wrapper.hpp>
#include <boost/assign/v2/ref/array/size_type.hpp> 
 
namespace boost{
namespace assign{
namespace v2{
namespace ref{
namespace array_aux{ 

    template<size_type N, typename T, typename Tag = ref::assign_tag::copy>
    struct impl_traits
    {
        typedef T value_type;
        typedef Tag assign_tag;
        typedef typename ref::wrapper<assign_tag, value_type> wrapper_type;
        typedef boost::array<wrapper_type,N> array_type;
    };

}// array_aux
}// ref
}// v2
}// assign
}// boost

#endif
