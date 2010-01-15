///////////////////////////////////////////////////////////////////////////////
// accumulator::statistics::count_int.hpp     						     	 //
//                                                                           //
//  Copyright 2010 Erwann Rogard. Distributed under the Boost                //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
///////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_STATISTICS_DETAIL_ACCUMULATOR_STATISTICS_COUNT_INT_HPP_ER_2010
#define BOOST_STATISTICS_DETAIL_ACCUMULATOR_STATISTICS_COUNT_INT_HPP_ER_2010
#include <map>
#include <functional>

#include <boost/mpl/placeholders.hpp>
#include <boost/mpl/apply.hpp>
#include <boost/mpl/bool.hpp>

#include <boost/range.hpp>

#include <boost/accumulators/framework/extractor.hpp>
#include <boost/accumulators/framework/accumulator_base.hpp>
#include <boost/accumulators/framework/parameters/sample.hpp>
#include <boost/accumulators/framework/parameters/accumulator.hpp>
#include <boost/accumulators/framework/depends_on.hpp>
#include <boost/accumulators/statistics_fwd.hpp>

#include <boost/statistics/detail/accumulator/statistics/keyword/key.hpp>
#include <boost/statistics/detail/accumulator/statistics/empirical_distribution_int.hpp>

namespace boost { 
namespace statistics{
namespace detail{
namespace accumulator{

namespace impl{

	template<typename Int,bool Cum>
	class count_int : public boost::accumulators::accumulator_base{
		typedef std::less<Int> comp_;
		typedef std::size_t size_;
        typedef boost::accumulators::dont_care dont_care_;

        public:

		typedef size_ result_type;

		count_int(dont_care_){}

		void operator()(dont_care_){}
		
        template<typename Args>
        result_type result(const Args& args)const{
        	Int key = args[statistics::detail::accumulator::keyword::key];
        	typedef typename boost::mpl::bool_<Cum> is_cum_;
            typedef statistics::detail::accumulator
            	::tag::empirical_distribution_int tag_;
        	return this->result_impl(
            	boost::accumulators::extract_result<tag_>(
                	args[boost::accumulators::accumulator]
                ),
                key,
                is_cum_()
            ); 
        }

		private:
		
        template<typename Map,typename N>
		result_type result_impl(
        	Map& map,
            const N& key,
            boost::mpl::bool_<false> cum
        )const{
        	return (map[key]); 
        }

        template<typename Map,typename N>
		result_type result_impl(
        	Map& map, 
            const N& key,
            boost::mpl::bool_<true> cum
        )const{
        	return std::for_each(
            	boost::begin(map),
                map.upper_bound(key),
            	accumulator()
            ).value; 
        }

		struct accumulator{
        	mutable size_ value;
        	
            accumulator():value(0){}
			accumulator(const accumulator& that)
            	:value(that.value){}
            
            template<typename Data>
            void operator()(const Data& data)const{
            	value += data.second;
            }
        
        };
	};
    
}

namespace tag
{
	template<bool Cum>
    struct count_int
      : boost::accumulators::depends_on<
      	statistics::detail::accumulator::tag::empirical_distribution_int
    >
    {

// TODO compile error
//      typedef statistics::detail::accumulator::
//      	impl::count_int<boost::mpl::_1,Cum> impl;
// must explicitly have:        
        
        struct impl{
        	template<typename Int,typename W>
            struct apply{
            	typedef statistics::detail::accumulator::
                	impl::count_int<Int,Cum> type;
            };
        };
    };
}

namespace extract
{

  	template<bool Cum,typename AccumulatorSet,typename Int>
	typename boost::mpl::apply<
		AccumulatorSet,
        boost::statistics::detail::accumulator::tag::count_int<Cum>
    >::type::result_type
  	count_int(AccumulatorSet const& acc,const Int& i)
    {
    	typedef boost::statistics::detail
        	::accumulator::tag::count_int<Cum> the_tag;
        return boost::accumulators::extract_result<the_tag>(
            acc,
            (boost::statistics::detail::accumulator::keyword::key = i)
        );
  	}

}

using extract::count_int;

}// accumulator
}// detail
}// statistics
}// boost

#endif
