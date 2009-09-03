//////////////////////////////////////////////////////////////////////////////
// scalar_dist::fun_wrap::cdf.hpp                                          //
//                                                                          //
//  (C) Copyright 2009 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_SCALAR_DIST_FUN_WRAP_CDF_HPP_ER_2009
#define BOOST_SCALAR_DIST_FUN_WRAP_CDF_HPP_ER_2009
#include <boost/scalar_dist/fun_wrap/detail/fun_wrap.hpp>

namespace boost{
namespace math{
 
    template<typename T,typename D> T cdf(const D& d,const T& x);

    namespace fun_wrap{
        template<typename D> struct cdf_
            : detail::fun_wrap<D,math::cdf>{};
    }

}// math
}// boost

#endif