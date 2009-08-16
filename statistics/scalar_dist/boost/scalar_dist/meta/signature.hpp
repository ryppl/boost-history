//////////////////////////////////////////////////////////////////////////////
// scalar_dist::meta::signature.hpp                                         //
//                                                                          //
//  (C) Copyright 2009 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_SCALAR_DIST_META_SIGNATURE_HPP_ER_2009
#define BOOST_SCALAR_DIST_META_SIGNATURE_HPP_ER_2009

namespace boost{
namespace math{

    // Signature for f(dist,x), f = pdf, cdf etc.
    template<typename D>
    struct signature{
        typedef typename D::value_type value_type;

        typedef value_type type(
            const D&,
            const value_type&
        );
    };


}// math
}// boost

#endif