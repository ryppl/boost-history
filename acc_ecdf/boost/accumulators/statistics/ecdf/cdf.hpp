///////////////////////////////////////////////////////////////////////////////
//  acc_ecdf                                                                 //
//                                                                           //
//  Copyright (C) 2005 Eric Niebler                                          //
//  Copyright (C) 2011 Erwann Rogard                                         //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
///////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ACCUMULATORS_STATISTICS_ECDF_CDF_HPP_ER_2011
#define BOOST_ACCUMULATORS_STATISTICS_ECDF_CDF_HPP_ER_2011
#include <boost/accumulators/framework/accumulator_base.hpp>
#include <boost/accumulators/framework/extractor.hpp>
#include <boost/accumulators/framework/depends_on.hpp>
#include <boost/accumulators/framework/parameters/sample.hpp>
#include <boost/accumulators/framework/parameters/accumulator.hpp>
#include <boost/accumulators/statistics/count.hpp>
#include <boost/accumulators/statistics/ecdf/aux_/ignore.hpp>
#include <boost/accumulators/statistics/ecdf/cumulative_count.hpp>
#include <boost/numeric/conversion/converter.hpp>

namespace boost{ 
//[syntax_ecdf_cdf
namespace accumulators{
namespace ecdf{
//<-
namespace impl{

    // T can be an integer or a float
    template<typename T,typename T1,typename Comp = std::less<T> >
    class cdf
        : public accumulator_base
    {
        typedef Comp comp_;
        typedef dont_care dont_care_;

        public:

        typedef T1 result_type;
        typedef T sample_type;

        cdf(dont_care_){}

        void operator()(dont_care_){}
        
        template<typename Args>
        result_type result(const Args& args)const{
            namespace acc = boost::accumulators;
            typedef std::size_t size_;
            size_ i =  extract::cumulative_count( 
                args[ accumulator ], args[ sample ] 
            );
            size_ n = acc::extract::count( args[ accumulator ] );
            typedef numeric::converter<T1,size_> converter_;
            return converter_::convert( i )/converter_::convert( n );
        }

    };
    
}// impl
//->
namespace tag
{
    template<typename T1>
    struct cdf
        : depends_on<
            ecdf::tag::cumulative_count,
            accumulators::tag::count
        >
    {/*<-*/
        struct impl{
            template<typename T,typename W>
            struct apply{
                typedef ecdf::impl::cdf<T,T1> type;
            };
        };
    /*->*/};
    
}// tag
namespace result_of{

    template<typename T1,typename AccSet>
    struct cdf/*<-*/
    {
        typedef ecdf::tag::cdf<T1> tag_;
        typedef typename
            detail::template 
                extractor_result<AccSet,tag_>::type type; 
    }/*->*/;

}// result_of
namespace extract
{

    template<typename T1,typename AccSet,typename T>
    typename ecdf::result_of::template cdf<T1,AccSet>::type
    cdf(AccSet const& set,const T& x)/*<-*/
    { 
        namespace acc = boost::accumulators;
        typedef ecdf::tag::cdf<T1> tag_;
        return extract_result<tag_>( set, ( acc::sample = x ) );
    }BOOST_ACCUMULATORS_ECDF_IGNORE(/*->*/;/*<-*/)/*->*/
    
}// extract
}// ecdf
}// accumulators
//]
}// boost

#endif // BOOST_ACCUMULATORS_STATISTICS_ECDF_CDF_HPP_ER_2011
