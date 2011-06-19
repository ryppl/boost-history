///////////////////////////////////////////////////////////////////////////////
//  acc_ecdf                                                                 //
//                                                                           //
//  Copyright (C) 2005 Eric Niebler                                          //
//  Copyright (C) 2011 Erwann Rogard                                         //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
///////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ACCUMULATORS_STATISTICS_KOLMOGOROV_SMIRNOV_STATISTIC_HPP_ER_2011
#define BOOST_ACCUMULATORS_STATISTICS_KOLMOGOROV_SMIRNOV_STATISTIC_HPP_ER_2011
#include <boost/accumulators/framework/extractor.hpp>
#include <boost/accumulators/framework/accumulator_base.hpp>
#include <boost/accumulators/framework/parameters/sample.hpp>
#include <boost/accumulators/framework/parameters/accumulator.hpp>
#include <boost/accumulators/framework/depends_on.hpp>
#include <boost/accumulators/statistics/count.hpp>
#include <boost/accumulators/statistics/ecdf/aux_/ignore.hpp>
#include <boost/accumulators/statistics/ecdf/count.hpp>
#include <boost/accumulators/statistics/stats.hpp>
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

    template<typename T>
    class kolmogorov_smirnov 
        : public accumulator_base
    {
        
        typedef std::size_t size_;
        typedef dont_care dont_care_;

        public:

        kolmogorov_smirnov(dont_care_){};

        typedef size_ size_type;
        typedef T sample_type;
        typedef void result_type;

        void operator()(dont_care_)const{}

        template<typename Args>
        result_type result(dont_care_) const
        {
        }
    };
    
}// impl
//->
namespace tag
{
    struct kolmogorov_smirnov
      : depends_on<
          ecdf::tag::ordered_sample,
          accumulators::tag::count
    >
    {/*<-*/
        struct impl{
            template<typename T, typename W>
            struct apply{
                typedef ecdf::impl::kolmogorov_smirnov<T> type;        
            };
        };
    /*->*/};
    
}// tag
namespace result_of{

    template<typename T1,typename AccSet,typename D>
    struct kolmogorov_smirnov_statistic/*<-*/
    {
        typedef T1 type;
    }/*->*/;

}// result_of

    // Usage : statistic<T1>(acc,dist)
    template<typename T1,typename AccSet,typename D>
    typename result_of::template 
    	kolmogorov_smirnov_statistic<T1, AccSet, D>::type
    kolmogorov_smirnov_statistic(AccSet const& set,const D& dist)/*<-*/
    {
            namespace acc = boost::accumulators;
    
            typedef T1 val_;
            typedef std::size_t size_;
            typedef tag::count tag_n_;
            typedef ecdf::tag::ordered_sample tag_os_;

            typedef typename ecdf::result_of::ordered_sample<
                AccSet>::type ref_os_; 
            typedef typename boost::remove_const< //in case ref changed to cref
                typename boost::remove_reference<
                    ref_os_
                >::type
            >::type os_;
            typedef typename boost::range_reference<os_>::type ref_elem_;

            ref_os_ ref_os = extract_result<tag_os_>( set );

            val_ m1 = static_cast<val_>(0);
            size_ i = 0;
            size_ n = acc::extract::count( set );
            
            BOOST_FOREACH(ref_elem_ e,ref_os){
                i += e.second; 
                val_ ecdf_val = static_cast<val_>(i) / static_cast<val_>(n);
                val_ true_cdf = cdf( dist, e.first );
                val_ m2 = (true_cdf > ecdf_val )
                    ?(true_cdf - ecdf_val) : (ecdf_val - true_cdf);
                if( m2 > m1 ){ m1 = m2; } 
            }
            
            return m1;
    }BOOST_ACCUMULATORS_ECDF_IGNORE(/*->*/;/*<-*/)/*->*/

}// ecdf
}// accumulators
//]
}// boost

#endif // BOOST_ACCUMULATORS_STATISTICS_KOLMOGOROV_SMIRNOV_STATISTIC_HPP_ER_2011
