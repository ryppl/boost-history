//////////////////////////////////////////////////////////////////////////////
// scalar_dist::meta::bind_delegate.hpp                                     //
//                                                                          //
//  (C) Copyright 2009 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_SCALAR_DIST_META_BIND_DELEGATE_HPP_ER_2009
#define BOOST_SCALAR_DIST_META_BIND_DELEGATE_HPP_ER_2009
#include <boost/function.hpp>
#include <boost/lambda/bind.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/type_traits/function_traits.hpp> 
#include <boost/scalar_dist/meta/signature.hpp>
#include <boost/scalar_dist/meta/delegate.hpp>

namespace boost{
namespace math{

    template<typename D>
    struct bind_delegate{
    
        typedef math::signature<D>                              sig_;
        typedef math::delegate<D>                               deleg_;
        typedef function_traits<typename sig_::type>            traits_;
        typedef typename traits_::result_type                   result_;
        typedef typename traits_::arg2_type                     arg2_;
        typedef result_ sig2_( arg2_ );
        typedef function<sig2_>                                 type;
        
        template<template<typename> class F>
        static type make(const D& dist){
            return type(
                boost::lambda::bind(
                    deleg_::template make<F>(),
                    dist,
                    boost::lambda::_1
                )
            );
        }
    };

}// math
}// boost

#endif