///////////////////////////////////////////////////////////////////////////////
//  accumulator_ecdf                                                         //
//                                                                           //
//  Copyright (C) 2005 Eric Niebler                                          //
//  Copyright (C) 2011 Erwann Rogard                                         //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
///////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ACCUMULATORS_STATISTICS_ECDF_COUNT_HPP_ER_2011
#define BOOST_ACCUMULATORS_STATISTICS_ECDF_COUNT_HPP_ER_2011
#include <map>
#include <functional> // less
#include <boost/accumulators/framework/accumulator_base.hpp>
#include <boost/accumulators/framework/extractor.hpp>
#include <boost/accumulators/framework/depends_on.hpp>
#include <boost/accumulators/framework/parameters/sample.hpp>
#include <boost/accumulators/framework/parameters/accumulator.hpp>
#include <boost/accumulators/statistics_fwd.hpp>
#include <boost/accumulators/statistics/ecdf/aux_/ignore.hpp>
#include <boost/accumulators/statistics/ecdf/ordered_sample.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/mpl/apply.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/type_traits.hpp>
#include <boost/numeric/conversion/converter.hpp>


namespace boost{    
//[syntax_ecdf_count
namespace accumulators{
namespace ecdf{
//<-
namespace impl{

    // Sample can be an integer or a float
    template<typename Sample>
    class count
        : public accumulator_base
    {
        typedef std::size_t size_;
        typedef dont_care dont_care_;

        public:

        typedef size_ result_type;
        typedef Sample sample_type;

        count(dont_care_){}

        void operator()(dont_care_){}
        
        template<typename Args>
        result_type result(const Args& args)const
        {
            typedef ecdf::tag::ordered_sample tag_;
            return extract_result<tag_>(
                args[ accumulator ]
            )[
                args[ sample ]
            ]; 
        }

    };
    
}// impl
//->
namespace tag
{

    struct count
        : depends_on<
            ecdf::tag::ordered_sample
        >
    {/*<-*/
        struct impl{
            template<typename Sample, typename Weight>
            struct apply{
                typedef ecdf::impl::count<Sample> type;
            };
        };
    /*->*/};
    
}// tag
namespace result_of{

    template<typename AccumulatorSet, typename Sample>
    struct count/*<-*/
    {
        typedef ecdf::tag::count tag_;
        typedef typename detail::template 
            extractor_result<AccumulatorSet,tag_>::type type; 
    }/*->*/;

}// result_of
namespace extract
{

    template<typename AccumulatorSet, typename Sample>
    typename ecdf::result_of::template count<AccumulatorSet,Sample>::type
    count(AccumulatorSet const& acc, const Sample& x)/*<-*/
    {
        typedef ecdf::tag::count tag_;
        return extract_result<tag_>( acc, (sample = x) );
    }BOOST_ACCUMULATORS_ECDF_IGNORE(/*->*/;/*<-*/)/*->*/

}// extract

    using extract::count;

}// ecdf
}// accumulators
//]
}// boost

#endif // BOOST_ACCUMULATORS_STATISTICS_ECDF_COUNT_HPP_ER_2011
