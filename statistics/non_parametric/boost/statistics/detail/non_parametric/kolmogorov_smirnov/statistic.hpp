///////////////////////////////////////////////////////////////////////////////
// kolmogorov_smirnov::statistic.hpp     	 	 							 //
//                                                                           //
//  Copyright 2010 Erwann Rogard. Distributed under the Boost                //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
///////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_STATISTICS_DETAIL_KOLMOGOROV_SMIRNOV_STATISTIC_HPP_ER_2010
#define BOOST_STATISTICS_DETAIL_KOLMOGOROV_SMIRNOV_STATISTIC_HPP_ER_2010
#include <iostream> // tmp

#include <boost/type_traits.hpp>
#include <boost/range.hpp>

#include <boost/mpl/placeholders.hpp>
#include <boost/mpl/apply.hpp>
#include <boost/mpl/if.hpp>

#include <boost/foreach.hpp>
#include <boost/parameter/binding.hpp>

#include <boost/accumulators/framework/extractor.hpp>
#include <boost/accumulators/framework/accumulator_base.hpp>
#include <boost/accumulators/framework/parameters/sample.hpp>
#include <boost/accumulators/framework/parameters/accumulator.hpp>
#include <boost/accumulators/framework/depends_on.hpp>
#include <boost/accumulators/statistics_fwd.hpp>
#include <boost/accumulators/statistics/count.hpp>

#include <boost/statistics/detail/non_parametric/empirical_distribution/count.hpp>
#include <boost/statistics/detail/non_parametric/kolmogorov_smirnov/keyword/reference_distribution.hpp> 

namespace boost { 
namespace statistics{
namespace detail{
namespace kolmogorov_smirnov{

namespace impl{

	// Usage :
    // Let acc denote an accumulator with feature kolmogorov::tag::statistic,
    // T1 result = extract::statistic(acc,dist); 
    //
    // Warning : See empirical_distribution::impl::count
	template<typename T,typename T1>
	class statistic : public boost::accumulators::accumulator_base{
    	
		typedef std::size_t size_;
        typedef boost::accumulators::dont_care dont_care_;

        public:

		statistic(dont_care_){};

		typedef size_ size_type;
		typedef T sample_type;
		typedef T1 result_type;

        void operator()(dont_care_)const{}

        template<typename Args>
        result_type result(const Args& args) const
        {
        	typedef T1 val_;
			typedef boost::accumulators::tag::accumulator tag_acc_;        
        	typedef boost::accumulators::tag::count tag_n_;
			typedef boost::statistics::detail::empirical_distribution
            	::tag::ordered_sample tag_os_;
			typedef boost::statistics::detail::kolmogorov_smirnov
            	::tag::reference_distribution tag_dist_;

            typedef boost::parameter::binding<Args,tag_acc_> bind1_;
            typedef typename bind1_::type cref_acc_;
            typedef boost::parameter::binding<Args,tag_dist_> bind2_;
            typedef typename bind2_::type cref_dist_;
			typedef typename boost::accumulators::detail
            	::extractor_result<Args,tag_os_>::type ref_os_;
            typedef typename boost::remove_const< //in case ref changed to cref
            	typename boost::remove_reference<
                	ref_os_
                >::type
            >::type os_;
            typedef typename boost::range_reference<os_>::type ref_elem_;

            cref_acc_ 	acc = args[boost::accumulators::accumulator];
            cref_dist_ 	dist = args[
            	boost::statistics::detail
                	::kolmogorov_smirnov::keyword::reference_distribution
            ];
            ref_os_ ref_os = boost::accumulators::extract_result<tag_os_>(acc);

            val_ m1 = static_cast<val_>(0);
            size_type i = 0;
            size_type n = boost::accumulators::extract_result<tag_n_>(acc);
            
            BOOST_FOREACH(ref_elem_ e,ref_os){
                i += e.second;
                val_ ecdf = static_cast<val_>(i) / static_cast<val_>(n);
                val_ true_cdf = cdf(dist,e.first);
                val_ m2 
                	= (true_cdf > ecdf)?(true_cdf - ecdf) : (ecdf - true_cdf);
                if(m2 > m1){ m1 = m2; } 
            }
            
            return m1;
        }
	};
    
}

namespace tag
{
	template<typename T1 = double>
    struct statistic
      : boost::accumulators::depends_on<
      	statistics::detail::empirical_distribution::tag::ordered_sample,
        boost::accumulators::tag::count
    >
    {
        struct impl{
        	template<typename T,typename W>
            struct apply{
        		typedef boost::statistics::detail::kolmogorov_smirnov
                	::impl::statistic<T,T1> type;    	
            };
        };
    };
}

namespace result_of{
	
    template<typename T1,typename AccSet>
    struct statistic{
    	typedef boost::statistics::detail
        	::kolmogorov_smirnov::tag::statistic<T1> tag_;
        typedef typename boost::accumulators::detail::extractor_result<
        	AccSet,
            tag_
        >::type type;
    };

}

namespace extract
{

  	template<typename T1,typename AccSet,typename D>
    typename boost::statistics::detail::kolmogorov_smirnov
    	::result_of::template statistic<T1,AccSet>::type
  	statistic(AccSet const& acc,const D& dist)
    {
    	typedef boost::statistics::detail
        	::kolmogorov_smirnov::tag::statistic<T1> the_tag;
        return boost::accumulators::extract_result<the_tag>(
            acc,
            (
            	boost::statistics::detail::kolmogorov_smirnov
                	::keyword::reference_distribution = dist
            )
        );
  	}

}

using extract::statistic;

}// kolmogorov_smirnov
}// detail
}// statistics
}// boost

#endif