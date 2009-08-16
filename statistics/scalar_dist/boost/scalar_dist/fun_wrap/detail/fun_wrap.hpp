//////////////////////////////////////////////////////////////////////////////
// scalar_dist::fun_wrap::detail::fun_wrap.hpp                              //
//                                                                          //
//  (C) Copyright 2009 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_SCALAR_DIST_FUN_WRAP_DETAIL_FUN_WRAP_HPP_ER_2009
#define BOOST_SCALAR_DIST_FUN_WRAP_DETAIL_FUN_WRAP_HPP_ER_2009
#include <boost/scalar_dist/meta/pointer.hpp>

namespace boost{
namespace math{
namespace detail{

    template<typename D,typename pointer<D>::type f>
    struct fun_wrap{
        typedef typename pointer<D>::type type;
        static type instance;
    };
    
    template<typename D,typename pointer<D>::type f>
    typename fun_wrap<D,f>::type
    fun_wrap<D,f>::instance = f;
    
}// detail
}// math
}// boost

#endif
