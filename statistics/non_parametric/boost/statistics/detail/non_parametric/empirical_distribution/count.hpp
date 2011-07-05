///////////////////////////////////////////////////////////////////////////////
// count.hpp                                                                 //
//                                                                           //
//  Copyright 2010 Erwann Rogard. Distributed under the Boost                //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
///////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_STATISTICS_DETAIL_NON_PARAMETRIC_EMPIRICAL_DISTRIBUTION_COUNT_HPP_ER_2010
#define BOOST_STATISTICS_DETAIL_NON_PARAMETRIC_EMPIRICAL_DISTRIBUTION_COUNT_HPP_ER_2010
#include <map>
#include <functional> // less

#include <boost/type_traits.hpp>

#include <boost/mpl/placeholders.hpp>
#include <boost/mpl/apply.hpp>
#include <boost/mpl/bool.hpp>

#include <boost/range.hpp>
#include <boost/numeric/conversion/converter.hpp>

#include <boost/accumulators/framework/extractor.hpp>
#include <boost/accumulators/framework/accumulator_base.hpp>
#include <boost/accumulators/framework/parameters/sample.hpp>
#include <boost/accumulators/framework/parameters/accumulator.hpp>
#include <boost/accumulators/framework/depends_on.hpp>
#include <boost/accumulators/statistics_fwd.hpp>

#include <boost/statistics/detail/non_parametric/empirical_distribution/ordered_sample.hpp>

namespace boost { 
namespace statistics{
namespace detail{
namespace empirical_distribution{
namespace impl{

    // T can be an integer or a float
    template<typename T>
	class count : public boost::accumulators::accumulator_base{
        typedef std::size_t size_;
        typedef boost::accumulators::dont_care dont_care_;

        public:

        typedef size_ result_type;
        typedef T sample_type;

        count(dont_care_){}

        void operator()(dont_care_){}
		
        template<typename Args>
        result_type result(const Args& args)const{
            namespace ns = empirical_distribution;
            typedef ns::tag::ordered_sample tag_;
            return boost::accumulators::extract_result<tag_>(
                args[ boost::accumulators::accumulator ]
            )[
                args[ boost::accumulators::sample ]
            ]; 
        }

    };
    
}// impl
namespace tag
{
    struct count: boost::accumulators::depends_on<
      	boost::statistics::detail
        	::empirical_distribution::tag::ordered_sample
    >
    {
        struct impl{
            template<typename T,typename W>
            struct apply{
                typedef detail::empirical_distribution::impl::count<T> type;
            };
        };
    };
}// tag
namespace result_of{

    template<typename AccSet,typename T>
    struct count{
    	typedef detail::empirical_distribution::tag::count tag_;
        typedef typename 
            boost::accumulators::detail::template 
            	extractor_result<AccSet,tag_>::type type; 
    };

}// result_of
namespace extract
{

    template<typename AccSet,typename T>
    typename detail::empirical_distribution
        ::result_of::template count<AccSet,T>::type
  	count(AccSet const& acc,const T& x)
    {
        namespace ed = detail::empirical_distribution;
    	typedef ed::tag::count tag_;
        return boost::accumulators::extract_result<tag_>(
            acc,
            (boost::accumulators::sample = x)
        );
  	}

}// extract
}// empirical_distribution
}// detail
}// statistics
}// boost

#endif
