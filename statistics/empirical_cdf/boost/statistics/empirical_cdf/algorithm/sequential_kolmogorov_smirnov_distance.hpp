//////////////////////////////////////////////////////////////////////////////
// empirical_cdf::algorithm::sequential_kolmogorov_smirnov_distance.hpp     //
//                                                                          //
//  (C) Copyright 2009 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_STATISTICS_EMPIRICAL_CDF_ALGORITHM_SEQUENTIAL_KOLMOGOROV_SMIRNOV_DISTANCE_HPP_ER_2009
#define BOOST_STATISTICS_EMPIRICAL_CDF_ALGORITHM_SEQUENTIAL_KOLMOGOROV_SMIRNOV_DISTANCE_HPP_ER_2009
#include <vector>
#include <string>
#include <stdexcept>
#include <algorithm>
#include <boost/format.hpp>
#include <boost/range.hpp>
#include <boost/iterator/iterator_traits.hpp>

#include <boost/statistics/empirical_cdf/algorithm/cdf_empirical_cdf_differences.hpp>

namespace boost{
namespace statistics{
namespace empirical_cdf{

    // Computes the ks-distance for each of 
    // i = 0        i=1                i=k-1
    // [x[0],x[m]), [x[0],x[2m]), ..., [x[0],x[n]), m = n/k
    //
    // Requirements:
    // [b_x,e_x) is the empirical sample as originally drawn i.e. NOT SORTED
    //
    template<typename D,typename It,typename ItO>
    ItO sequential_kolmogorov_smirnov_distance(
        const D& dist,
        It b_x,
        It e_x,
        typename iterator_difference<It>::type k,
        ItO i_o
    );

    // Implementation //

    // for each b in {b_x+dx[0],b_x+dx[1],...,b_x+dx[n-1]}
    // computes the distance to the empir cdf [b,e_x)
    template<typename D,typename It,typename ItO>
    ItO sequential_kolmogorov_smirnov_distance(
        const D& dist,
        It b_x,
        It e_x, 
        It b_dx,
        It e_dx,
        typename iterator_difference<It>::type k,
        ItO i_o
    )
    {
        typedef std::string                             str_;
        typedef typename iterator_difference<It>::type  diff_;
        typedef typename iterator_value<It>::type       val_;
        typedef std::vector<val_>                       vals_;
        typedef typename range_iterator<vals_>::type    it_val_;
        if( b_x == e_x ){ return i_o; }
        diff_ diff = std::distance( b_x, e_x);
        if(diff % k != 0){ 
            static const str_ msg = 
                str_("sequential_kolmogorov_smirnov_distance") +
                "diff = %1% not multiple of k = %2%."
            ;
            format f(msg); f % diff % k;
            throw std::runtime_error(f.str());  
        }
        vals_ cdfs(diff);
        vals_ r_x; r_x.reserve(diff);
        diff_ delta = diff / k;
        
        It i_x = b_x;
        while(i_x<e_x){
            std::advance(i_x,delta);
            r_x.clear();
            std::copy(
                b_x,
                i_x,
                std::back_inserter(r_x)
            );
            std::sort(boost::begin(r_x),boost::end(r_x));
            it_val_ e_cdf = cdf_empirical_cdf_differences(
                dist,
                boost::begin(r_x),
                boost::end(r_x),
                boost::begin(cdfs)
            );
            *i_o = (
                *(
                    std::max_element(
                        boost::begin(cdfs),
                        e_cdf
                    )
                )
            );
            ++i_o;
        }
        return i_o;
    }
    
}// empirical_cdf
}// statistics
}// boost

#endif
