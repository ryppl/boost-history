//////////////////////////////////////////////////////////////////////////////
//  acc_ecdf                                                                //
//                                                                          //
//  Copyright (C) 2005 Eric Niebler                                         //
//  Copyright (C) 2011 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ACCUMULATORS_STATISTICS_ECDF_AUX_KOLMOGOROV_SMIRNOV_SIMULATOR_ER_2011_HPP
#define BOOST_ACCUMULATORS_STATISTICS_ECDF_AUX_KOLMOGOROV_SMIRNOV_SIMULATOR_ER_2011_HPP
#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics/ecdf/aux_/ignore.hpp>
#include <boost/accumulators/statistics/ecdf/aux_/incremental_simulator.hpp>
#include <boost/accumulators/statistics/ecdf/aux_/kolmogorov_smirnov_output.hpp>
#include <boost/accumulators/statistics/ecdf/kolmogorov_smirnov_statistic.hpp>
#include <boost/accumulators/statistics_fwd.hpp>

namespace boost{
//[syntax_ecdf_aux_kolmogorov_smirnov_simulator
namespace accumulators{
namespace ecdf{
namespace aux_{
namespace result_of{

    template<typename Gen, typename D, typename T = typename D::value_type>
    struct kolmogorov_smirnov_simulator/*<-*/
    {
        typedef typename Gen::distribution_type random_;
        typedef typename random_::result_type sample_type; 

        typedef ecdf::tag::kolmogorov_smirnov ks_tag_;
        typedef boost::accumulators::stats<ks_tag_> features_;
    
        typedef accumulators::accumulator_set<
            sample_type, 
            features_
        > accumulator_set;

        typedef kolmogorov_smirnov_output<D, T> functor;
        
        typedef incremental_simulator<Gen, accumulator_set, functor> type;

        static type call(Gen gen, D dist)
        {
            accumulator_set set;
            functor f( dist );
            return type( gen, set, f );
        }
    
    }/*->*/;

}// result_of

    template<typename Gen, typename D>
    typename result_of::kolmogorov_smirnov_simulator<Gen, D>::type
    kolmogorov_smirnov_simulator(Gen gen, D dist)/*<-*/
    {
        typedef result_of::kolmogorov_smirnov_simulator<Gen, D> caller_;
        return caller_::call( gen, dist );
    }BOOST_ACCUMULATORS_ECDF_IGNORE(/*->*/;/*<-*/)/*->*/

}// aux_
}// ecdf
}// accumulators
//]
}// boost

#endif // BOOST_ACCUMULATORS_STATISTICS_ECDF_AUX_KOLMOGOROV_SMIRNOV_SIMULATOR_ER_2011_HPP
