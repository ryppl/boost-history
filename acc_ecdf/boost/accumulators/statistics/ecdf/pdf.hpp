///////////////////////////////////////////////////////////////////////////////
//  accumulator_ecdf                                                         //
//                                                                           //
//  Copyright (C) 2005 Eric Niebler                                          //
//  Copyright (C) 2011 Erwann Rogard                                         //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
///////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ACCUMULATORS_STATISTICS_ECDF_PDF_HPP_ER_2011
#define BOOST_ACCUMULATORS_STATISTICS_ECDF_PDF_HPP_ER_2011
#include <boost/range.hpp>
#include <boost/numeric/conversion/converter.hpp>
#include <boost/accumulators/framework/extractor.hpp>
#include <boost/accumulators/framework/accumulator_base.hpp>
#include <boost/accumulators/framework/parameters/sample.hpp>
#include <boost/accumulators/framework/parameters/accumulator.hpp>
#include <boost/accumulators/framework/depends_on.hpp>
#include <boost/accumulators/statistics/count.hpp>
#include <boost/accumulators/statistics/ecdf/aux_/ignore.hpp>
#include <boost/accumulators/statistics/ecdf/count.hpp>

namespace boost{ 
//[syntax_ecdf_pdf
namespace accumulators{
namespace ecdf{
//<-
namespace impl{

    // T can be an integer or a float
    template<typename Sample, typename Result>
    class pdf 
        : public accumulator_base
    {
        typedef dont_care dont_care_;

        public:

        typedef Result result_type;
        typedef Sample sample_type;

        pdf(dont_care_){}

        void operator()(dont_care_){}
        
        template<typename Args>
        result_type result(const Args& args)const{
            typedef std::size_t size_;
            size_ i =  ecdf::extract::count( 
                args[ accumulator ], 
                args[ sample ] 
            );
            size_ n = accumulators::extract::count( args[ accumulator ] );
            typedef numeric::converter<Result,size_> converter_;
            return converter_::convert( i ) / converter_::convert( n );
        }

    };
    
}// impl
//->
namespace tag
{

    template<typename Result>
    struct pdf: depends_on<
        ecdf::tag::count,
        accumulators::tag::count
    >
    {/*<-*/
        struct impl{
            template<typename Sample, typename Weight>
            struct apply{
                typedef ecdf::impl::pdf<Sample, Result> type;
            };
        };
    /*->*/};

}// tag
namespace result_of{

    template<typename Result, typename AccumulatorSet>
    struct pdf/*<-*/
    {
        typedef ecdf::tag::pdf<Result> tag_;
        typedef typename
            detail::template 
                extractor_result<AccumulatorSet,tag_>::type type; 
    }/*->*/;

}// result_of
namespace extract
{

    template<typename Result, typename AccumulatorSet, typename T>
    typename ecdf::result_of::template pdf<Result, AccumulatorSet>::type
    pdf(AccumulatorSet const& set, const T& x)/*<-*/
    { 
        typedef ecdf::tag::pdf<Result> tag_;
        return extract_result<tag_>( set, ( sample = x ) );
    }BOOST_ACCUMULATORS_ECDF_IGNORE(/*->*/;/*<-*/)/*->*/

}// extract

    using extract::pdf;

}// ecdf
}// accumulators
//]
}// boost

#endif // BOOST_ACCUMULATORS_STATISTICS_ECDF_PDF_HPP_ER_2011
