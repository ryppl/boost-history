//////////////////////////////////////////////////////////////////////////////
// scalar_dist::meta::delegate.hpp                                          //
//                                                                          //
//  (C) Copyright 2009 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_SCALAR_DIST_META_DELEGATE_HPP_ER_2009
#define BOOST_SCALAR_DIST_META_DELEGATE_HPP_ER_2009
#include <boost/function.hpp>
#include <boost/scalar_dist/meta/signature.hpp>

namespace boost{
namespace math{

    template<typename D>
    struct delegate{
        typedef typename signature<D>::type     sig_;
        typedef boost::function<sig_>           type;

        template<template<typename> class F> // F in fun_wrap
        static type make(){ return type(F<D>::instance); }
    };


}// math
}// boost

#endif