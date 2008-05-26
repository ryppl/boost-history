///////////////////////////////////////////////////////////////////////////////
// acvf_analysis.hpp
//
//  Copyright 2008 Erwann Rogard. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#include <iostream>
#include <algorithm>
#include <iterator>
#include <functional>
#include <stdexcept>
#include <boost/assert.hpp>
#include <boost/assign/std/vector.hpp>
#include <boost/parameter/parameters.hpp>
#include <boost/parameter/keyword.hpp>
#include <boost/bind.hpp>
#include <boost/ref.hpp>
#include <boost/assign/std/vector.hpp>
#include <boost/range/iterator_range.hpp>

#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics/stats.hpp>
#include <boost/accumulators/statistics/delay.hpp>
#include <boost/accumulators/statistics/acvf_moving_average.hpp>
#include <boost/accumulators/statistics/acvf.hpp>
#include <boost/accumulators/statistics/acf.hpp>
#include <boost/accumulators/statistics/percentage_effective_sample_size.hpp>
#include <boost/accumulators/statistics/standard_error_autocorrelated.hpp>
#include <boost/accumulators/statistics/standard_error_iid.hpp>

#ifndef BOOST_ACCUMULATORS_STATISTICS_ACVF_ANALYSIS_HPP_ER_2008_04
#define BOOST_ACCUMULATORS_STATISTICS_ACVF_ANALYSIS_HPP_ER_2008_04
namespace boost{namespace accumulators{
namespace statistics{

    class acvf_analysis{
        typedef boost::accumulators::default_delay_discriminator     delaydisrc;
        typedef boost::accumulators::accumulator_set<
        double, boost::accumulators::stats<
            boost::accumulators::tag::mean,
            boost::accumulators::tag::acf<>,
            boost::accumulators::tag::integrated_acvf<>,
            boost::accumulators::tag::percentage_effective_sample_size<>,
            boost::accumulators::tag::standard_error_autocorrelated<>,
            boost::accumulators::tag::standard_error_iid<>
            >
        >  acc_type;

    public://TODO define copy and assign
        acvf_analysis(std::size_t max_lag)
        :K(max_lag),
        acc(boost::accumulators::tag::delay<>::cache_size=(K+1)){};

        template<typename R>
        void operator()(
            const R& range,//TODO specify range, container?
            std::size_t offset,
            std::size_t stride){

            // an iterator range?
            typedef typename R::const_iterator const_iter_type;
            typedef typename R::size_type size_type;
            const_iter_type i = boost::begin(range);
            const_iter_type e = boost::end(range);
            if(std::distance(i,e)>offset){
                std::advance(i,offset);
                //this has the effect of rounding to smallest
                std::size_t d = (std::distance(i,e)-1)/stride;
                d *= stride;
                e = i; std::advance(e,d+1);

                while(i<e){
                    acc(*i);
                    std::advance(i,stride);
                }
//            for_each(
//                boost::begin(range),
//                boost::end(range),
//                boost::bind<void>(boost::ref(acc),_1)
//            );
            }else{
                std::runtime_error("acvf_analysis");
            }
        }
        void print(std::ostream& os)const
        {
            using namespace boost::accumulators;
            os  << "->count: " << count(acc)
                << std::endl
                << "->estimated acf: ";
                    copy(
                        begin(acf<delaydisrc>(acc)),
                        end(acf<delaydisrc>(acc)),
                        std::ostream_iterator<double>(os," ")
                    );
            os  << std::endl
                << "->estimated var: "
                << integrated_acvf<delaydisrc>(acc) << std::endl
                << "->estimated ess%: "
                << percentage_effective_sample_size<delaydisrc>(acc)
                << std::endl
                << "->mean: " << mean(acc) << std::endl
                << "->estimated standard error assuming iid: "
                << standard_error_iid<delaydisrc>(acc) << std::endl
                << "->estimated standard error assuming acf is zero after lag "
                << K << ": "
                << standard_error_autocorrelated<delaydisrc>(acc) << std::endl;
        };
    private:

        std::size_t K;
        acc_type acc;
    };

//TODO
//    std::ostream& operator<<(std::ostream& os,const acvf_analysis& a){
//        a.print(os);
//        return os;
//    };

}
}}
#endif
