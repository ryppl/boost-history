//////////////////////////////////////////////////////////////////////////////
// distribution::toolkit::data::generate2_n_sample_cdf.hpp                   //
//                                                                          //
//  (C) Copyright 2009 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_STATISTICS_DETAIL_DISTRIBUTION_TOOLKIT_DATA_GENERATE2_N_SAMPLE_CDF_HPP_ER_2009
#define BOOST_STATISTICS_DETAIL_DISTRIBUTION_TOOLKIT_DATA_GENERATE2_N_SAMPLE_CDF_HPP_ER_2009
#include <boost/statistics/detail/distribution_toolkit/data/generate_n_sample_cdf.hpp>
#include <boost/statistics/detail/distribution_toolkit/random/generator.hpp>

namespace boost{

        // Adds n elements randomly drawn from dist, together with their cdf 
        // to the sample maintained in sc
        template<typename T,typename N,typename U,typename D>
        typename enable_if<
            statistics::detail::distribution::toolkit
                ::meta::is_scalar_distribution<D>,
                void
        >::type
        generate2_n(
            statistics::detail::distribution::toolkit::sample_cdf<T>& sc,
            N n, 
            U& urng,
            D dist
        ){
            return boost::generate_n(
                sc,
                n,
                boost::make_random_generator(urng,dist),
                dist
            );
        }

}

#endif
