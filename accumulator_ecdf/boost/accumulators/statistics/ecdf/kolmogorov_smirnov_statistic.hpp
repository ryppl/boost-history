///////////////////////////////////////////////////////////////////////////////
//  accumulator_ecdf                                                         //
//                                                                           //
//  Copyright (C) 2005 Eric Niebler                                          //
//  Copyright (C) 2011 Erwann Rogard                                         //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
///////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ACCUMULATORS_STATISTICS_KOLMOGOROV_SMIRNOV_STATISTIC_HPP_ER_2011
#define BOOST_ACCUMULATORS_STATISTICS_KOLMOGOROV_SMIRNOV_STATISTIC_HPP_ER_2011
#include <cstddef>
#include <cmath>
#include <boost/accumulators/framework/extractor.hpp>
#include <boost/accumulators/framework/accumulator_base.hpp>
#include <boost/accumulators/framework/parameters/sample.hpp>
#include <boost/accumulators/framework/parameters/accumulator.hpp>
#include <boost/accumulators/framework/depends_on.hpp>
#include <boost/accumulators/statistics/count.hpp>
#include <boost/accumulators/statistics/ecdf/aux_/ignore.hpp>
#include <boost/accumulators/statistics/ecdf/count.hpp>
#include <boost/accumulators/statistics/stats.hpp>
#include <boost/numeric/conversion/converter.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/mpl/apply.hpp>
#include <boost/mpl/if.hpp>
#include <boost/foreach.hpp>
#include <boost/range.hpp>
#include <boost/parameter/binding.hpp>
#include <boost/type_traits.hpp>

namespace boost{ 
//[syntax_ecdf_kolmogorov_smirnov_statistic
namespace accumulators{
namespace ecdf{
//<-
namespace impl{

    template<typename Sample>
    class kolmogorov_smirnov 
        : public accumulator_base
    {
        
        typedef std::size_t size_;
        typedef dont_care dont_care_;

        public:

        kolmogorov_smirnov(dont_care_){};

        typedef size_ size_type;
        typedef Sample sample_type;
        typedef void result_type;

        void operator()(dont_care_)const{}

        template<typename Args>
        result_type result(dont_care_) const
        {}

    };
    
}// impl
//->
namespace tag
{
    struct kolmogorov_smirnov_statistic
      : depends_on<
          ecdf::tag::ordered_sample,
          accumulators::tag::count
    >
    {/*<-*/
        struct impl{
            template<typename Sample, typename Weight>
            struct apply{
                typedef ecdf::impl::kolmogorov_smirnov<Sample> type;        
            };
        };
    /*->*/};
    
}// tag
namespace result_of{

    template<
        typename AccumulatorSet, typename Distribution
        , typename T = typename Distribution::value_type
    >
    struct kolmogorov_smirnov_statistic/*<-*/
    {
        typedef T type;
    }/*->*/;

}// result_of
namespace extract{

    template<typename Result, typename AccumulatorSet, typename Distribution>
    typename result_of::template 
        kolmogorov_smirnov_statistic<AccumulatorSet, Distribution, Result>::type
    kolmogorov_smirnov_statistic(
        AccumulatorSet const& acc, 
        const Distribution& dist
    )/*<-*/
    {
            typedef Result result_;
            typedef std::size_t size_;

            typedef typename ecdf::result_of::ordered_sample<
                AccumulatorSet
            >::type result_ordered_sample_;

            typedef typename boost::remove_const< 
                typename boost::remove_reference<
                    result_ordered_sample_
                >::type
            >::type ordered_sample_;

            typedef typename boost::range_reference<
                ordered_sample_
            >::type pair_;

            typedef numeric::converter<size_, result_> converter_;
            result_ 
                result = converter_::convert( 0 ),
                a, x, y,
                n = converter_::convert( accumulators::count( acc ) );
            size_ i = 0;
            
            BOOST_FOREACH( pair_ p, ecdf::ordered_sample( acc ) )
            {
                i += p.second; 
                x = converter_::convert( i ) / n;
                typedef typename Distribution::value_type val_;
                y = numeric::converter<val_, result_>::convert( 
                    cdf( dist, p.first ) 
                );
                a = fabs( x - y );
                result = ( a > result ) ? a : result;
            }
            return result;
    }BOOST_ACCUMULATORS_ECDF_IGNORE(/*->*/;/*<-*/)/*->*/
    
}// extract 

    using extract::kolmogorov_smirnov_statistic;

}// ecdf
}// accumulators
//]
}// boost

#endif // BOOST_ACCUMULATORS_STATISTICS_KOLMOGOROV_SMIRNOV_STATISTIC_HPP_ER_2011
