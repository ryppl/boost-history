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
#include <boost/type_traits/function_traits.hpp>
#include <boost/lambda/bind.hpp>
#include <boost/lambda/lambda.hpp>
#include <ext/algorithm>
#include <boost/format.hpp>
#include <boost/range.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/numeric/conversion/converter.hpp>
#include <boost/iterator/counting_iterator.hpp>
#include <boost/iterator/transform_iterator.hpp>
#include <boost/iterator/iterator_traits.hpp>
#include <boost/scalar_dist/fun_wrap/cdf.hpp>
#include <boost/scalar_dist/meta/bind_delegate.hpp>
#include <boost/scalar_dist/algorithm/transform.hpp>

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
        typedef typename iterator_value<InIt>::type value_t;
        typedef typename iterator_difference<InIt>::type diff_t;
        diff_t diff = std::distance(b_cdf,e_cdf);
        value_t n = numeric::converter<value_t,diff_t>::convert(diff);
        typedef numeric::converter<value_t,unsigned> conv_;

        return std::transform(
            counting_iterator<unsigned>(0),
            counting_iterator<unsigned>(diff),
            b_cdf,
            out,
            // TODO lambda expression might be cleaner:
            bind<value_t>( 
                &fabs,
                bind<value_t>(
                    std::minus<value_t>(),
                    bind<value_t>(
                        std::divides<value_t>(),
                        bind<value_t>(
                            numeric::converter<value_t,unsigned>(),
                            _1
                        ),
                        n
                    ),
                    _2
                )
            )
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
