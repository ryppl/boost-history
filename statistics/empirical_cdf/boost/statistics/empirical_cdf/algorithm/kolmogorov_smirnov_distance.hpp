//////////////////////////////////////////////////////////////////////////////
// empirical_cdf::algorithm::kolmogorov_smirnov_distance.hpp                //
//                                                                          //
//  (C) Copyright 2009 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_STATISTICS_EMPIRICAL_CDF_ALGORITHM_KOLMOGOROV_SMIRNOV_DISTANCE_HPP_ER_2009
#define BOOST_STATISTICS_EMPIRICAL_CDF_ALGORITHM_KOLMOGOROV_SMIRNOV_DISTANCE_HPP_ER_2009
#include <vector>
#include <cmath>
#include <string>
#include <boost/range.hpp>
#include <boost/iterator/iterator_traits.hpp>
#include <boost/function.hpp>
#include <boost/format.hpp>
#include <boost/range.hpp>
#include <boost/statistics/empirical_cdf/algorithm/cdf_empirical_cdf_differences.hpp>


namespace boost{
namespace statistics{
namespace empirical_cdf{


    // *[b_x,e_x) is a random sample (not sorted)
    template<typename D,typename InIt>
    typename iterator_value<InIt>::type
    kolmogorov_smirnov_distance(
        const D& dist, // e.g. D == normal_distribution<double>
        InIt b_x,
        InIt e_x
        Out b_cdf_diff
    );

    // Implementation //
    
    template<typename D,typename InIt,typename Out>
    typename iterator_value<InIt>::type
    kolmogorov_smirnov_distance(
        const D& dist,
        InIt b_x,
        InIt e_x,
        Out b_cdf_diff,
    ){
        if(!is_sorted(b_x,e_x)){
            static const char* msg 
                = "kolmogorov_smirnov_distance : [b_x,e_x) not sorted";
            throw std::runtime_error(
                msg
            );
        }

        it_val_ e_cdf_diff = cdf_empirical_cdf_differences(
            dist,
            b_x,
            e_x,
            b_cdf_diff
        );
        return(
            *(
                std::max_element(
                    make_transform_iterator(b_cdf_diff,fun)
                    make_transform_iterator(e_cdf_diff,fun)
                )
            )
        );

    }

}// empirical_cdf
}// statistics
}// boost

#endif
