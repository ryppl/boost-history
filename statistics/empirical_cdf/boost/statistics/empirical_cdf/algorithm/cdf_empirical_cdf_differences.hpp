//////////////////////////////////////////////////////////////////////////////
// empirical_cdf::algorithm::cdf_empirical_cdf_differences.hpp              //
//                                                                          //
//  (C) Copyright 2009 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_STATISTICS_EMPIRICAL_CDF_ALGORITHM_CDF_EMPIRICAL_CDF_DIFFERENCES_HPP_ER_2009
#define BOOST_STATISTICS_EMPIRICAL_CDF_ALGORITHM_CDF_EMPIRICAL_CDF_DIFFERENCES_HPP_ER_2009
#include <vector>
#include <string>
#include <stdexcept>
#include <algorithm>
#include <ext/algorithm>
#include <boost/range.hpp>

#include <boost/iterator/iterator_traits.hpp>

#include <boost/typeof/typeof.hpp>
#include <boost/scalar_dist/fun_wrap/cdf.hpp>
#include <boost/scalar_dist/meta/bind_delegate.hpp>
#include <boost/scalar_dist/algorithm/transform.hpp>

#include <boost/lambda/bind.hpp>
#include <boost/lambda/lambda.hpp>
//#include <boost/lambda/detail/ret.hpp>
#include <boost/statistics/empirical_cdf/algorithm/transform.hpp>

namespace boost{
namespace statistics{
namespace empirical_cdf{

    // Computes {G_i-F_n(x[i]):i=0,...,n-1}, 
    // sorted sample.
    //
    // Requirements:
    // *[b_cdf,e_cdf) = {F(x[i]): x[i]<=x[i+1], i=0,...,n-1 }
    template<typename InIt,typename OutIt>
    OutIt cdf_empirical_cdf_differences(
        InIt b_cdf, 
        InIt e_cdf, 
        OutIt out
    );

    // Same as cdf_empirical_cdf_differences but computes the cdf on the fly
    //
    // Requirements:
    // *[b_x,e_x) = {x[i]: x[i]<=x[i+1], i=0,...,n-1 }
    template<typename D,typename InIt,typename OutIt>
    OutIt cdf_empirical_cdf_differences(
        const D& dist, //e.g. normal_distribution<double>
        InIt b_x,
        InIt e_x,
        OutIt out
    );

    // Implementation //

    template<typename InIt,typename OutIt>
    OutIt
    cdf_empirical_cdf_differences(
        InIt b_cdf,  
        InIt e_cdf,  
        OutIt out
    ){
        typedef typename iterator_value<InIt>::type val_;

        BOOST_AUTO(
            fun,
            boost::lambda::ret<val_>(
                boost::lambda::_2 - boost::lambda::_1
            )            
        );

        return empirical_cdf::transform<val_>(
            b_cdf,
            e_cdf,
            out,
            fun
        );
    }

    template<typename D,typename InIt,typename OutIt>
    OutIt
    cdf_empirical_cdf_differences(
        const D& dist,
        InIt b_x,
        InIt e_x,
        OutIt out
    ){
        if(!is_sorted(b_x,e_x)){
            static const char* msg 
                = "cdf_empirical_cdf_differences : [b_x,e_x) not sorted";
            throw std::runtime_error(
                msg
            );
        }
        typedef math::bind_delegate<D>                  meta_;
        typedef typename math::bind_delegate<D>::type   deleg_;
        deleg_ d = meta_::template make<math::fun_wrap::cdf_>(dist);

        return cdf_empirical_cdf_differences(
            make_transform_iterator(b_x,d),
            make_transform_iterator(e_x,d),
            out
        );
    }

}// empirical_cdf
}// statistics
}// boost

#endif
