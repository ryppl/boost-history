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
#include <boost/range.hpp>
#include <boost/iterator/iterator_traits.hpp>
#include <boost/statistics/empirical_cdf/algorithm/sequential_kolmogorov_smirnov_distance.hpp>

namespace boost{
namespace statistics{
namespace empirical_cdf{

    // *[b_x,e_x) represents a random sample (not necessarily sorted)
    template<typename D,typename InIt>
    typename iterator_value<InIt>::type
    kolmogorov_smirnov_distance(
        const D& dist, // e.g. D == normal_distribution<double>
        InIt b_x,
        InIt e_x
    );

    // Implementation //
    
    template<typename D,typename InIt>
    typename iterator_value<InIt>::type
    kolmogorov_smirnov_distance(
        const D& dist,
        InIt b_x,
        InIt e_x
    ){
        typedef typename iterator_value<InIt>::type val_;
        typedef std::vector<val_> vals_;
        vals_ vals(1); 
        sequential_kolmogorov_smirnov_distance(
            dist,
            b_x,
            e_x,
            1,
            boost::begin(vals)
        );        
        return vals.back();
    }

}// empirical_cdf
}// statistics
}// boost

#endif
