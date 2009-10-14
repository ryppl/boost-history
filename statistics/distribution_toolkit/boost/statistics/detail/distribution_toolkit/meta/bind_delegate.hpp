//////////////////////////////////////////////////////////////////////////////
// distribution::toolkit::meta::bind_delegate.hpp                            //
//                                                                          //
//  (C) Copyright 2009 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_STATISTICS_DETAIL_DISTRIBUTION_TOOLKIT_META_BIND_DELEGATE_HPP_ER_2009
#define BOOST_STATISTICS_DETAIL_DISTRIBUTION_TOOLKIT_META_BIND_DELEGATE_HPP_ER_2009
#include <boost/function.hpp>
#include <boost/lambda/bind.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/type_traits/function_traits.hpp> 
#include <boost/statistics/detail/distribution_toolkit/meta/signature.hpp>
#include <boost/statistics/detail/distribution_toolkit/meta/delegate.hpp>

namespace boost{
namespace statistics{
namespace detail{
namespace distribution{
namespace toolkit{

namespace meta{

    template<typename D>
    struct bind_delegate{
    
        typedef signature<D>                              sig_;
        typedef delegate<D>                               deleg_;
        typedef function_traits<typename sig_::type>      traits_;
        typedef typename traits_::result_type             result_;
        typedef typename traits_::arg2_type               arg2_;
        typedef result_ sig2_( arg2_ );
        typedef function<sig2_>                           type;
        
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

}// meta



    template<template<typename> class F,typename D>
    typename meta::bind_delegate<D>::type
    make_bind_delegate(
        const D& dist
    )
    {
        typedef meta::bind_delegate<D> meta_;
        return meta_::template make<F>(dist);
    }
    

}// distribution
}// toolkit
}// detail
}// statistics
}// boost

#endif