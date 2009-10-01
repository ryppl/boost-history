//////////////////////////////////////////////////////////////////////////////
// distribution_toolkit::iterator::distribution_function.hpp                //
//                                                                          //
//  (C) Copyright 2009 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_STATISTICS_DETAIL_DISTRIBUTION_TOOLKIT_ITERATOR_DISTRIBUTION_FUNCTION_HPP_ER_2009
#define BOOST_STATISTICS_DETAIL_DISTRIBUTION_TOOLKIT_ITERATOR_DISTRIBUTION_FUNCTION_HPP_ER_2009
#include <boost/iterator/transform_iterator.hpp>
#include <boost/statistics/detail/distribution_toolkit/meta/bind_delegate.hpp>

namespace boost{
namespace statistics{
namespace detail{
namespace distribution_toolkit{
namespace iterator{

template<typename D>
struct meta_distribution_function{

    typedef meta::bind_delegate<D>   bind_deleg_;
    typedef typename bind_deleg_::type  unary_;
    
    template<typename ItX>
    struct apply{
        typedef boost::transform_iterator<unary_,ItX> type;

        template<template<typename> class F>
        static type make(
            const D& dist,
            ItX i_x
        ){
            return make_transform_iterator(
                i_x,
                bind_deleg_::template make<F>(dist)
            );
        }
    };
};


// An iterator that transforms x to f(dist,x), f = F<D>::instance,
// where F is defined in /fun_wrap
//
// TODO enable_if<is_pseudo_scalar_distribution>
template<template<typename> class F,typename D,typename ItX>
typename meta_distribution_function<D>::template apply<ItX>::type
make_distribution_function(
    const D& dist,
    ItX i_x
)
{
    typedef meta_distribution_function<D> meta_;
    typedef typename meta_::template apply<ItX> app_;

    return app_::template make<F>(
        dist,
        i_x
    );
}

}// iterator
}// distribution_toolkit
}// detail
}// statistics
}// boost


#endif
