///////////////////////////////////////////////////////////////////////////////
// accumulator::statistics::empirical_distribution_int.hpp     				 //
//                                                                           //
//  Copyright 2010 Erwann Rogard. Distributed under the Boost                //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
///////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_STATISTICS_DETAIL_ACCUMULATOR_STATISTICS_EMPIRICAL_DISTRIBUTION_INT_HPP_ER_2010
#define BOOST_STATISTICS_DETAIL_ACCUMULATOR_STATISTICS_EMPIRICAL_DISTRIBUTION_INT_HPP_ER_2010
#include <map>
#include <functional>

#include <boost/mpl/placeholders.hpp>
#include <boost/mpl/apply.hpp>

#include <boost/accumulators/framework/extractor.hpp>
#include <boost/accumulators/framework/accumulator_base.hpp>
#include <boost/accumulators/framework/parameters/sample.hpp>
#include <boost/accumulators/framework/parameters/accumulator.hpp>
#include <boost/accumulators/framework/depends_on.hpp>
#include <boost/accumulators/statistics_fwd.hpp>

#include <boost/statistics/detail/accumulator/statistics/keyword/key.hpp>

namespace boost { 
namespace statistics{
namespace detail{
namespace accumulator{

namespace impl{

	template<typename Int>
	class empirical_distribution_int 
    		: public boost::accumulators::accumulator_base{
		typedef std::less<Int> comp_;
		typedef std::size_t size_;
        typedef boost::accumulators::dont_care dont_care_;
        typedef std::map<Int,size_,comp_> map_;

        public:

		typedef size_ size_type;

		// non-const because map::operator[](key) returns a non-const
		typedef map_& result_type;

		empirical_distribution_int(dont_care_){}

		template<typename Args>
		void operator()(const Args& args){
        	++(this->freq[args[boost::accumulators::sample]]);
        }
		
		// Returns the entire distribution, represented by a map
        result_type result(dont_care_)const{
        	return (this->freq); 
        }

		private:
        mutable map_ freq;
	};
    
}

namespace tag
{
    struct empirical_distribution_int
      : boost::accumulators::depends_on<>
    {
      typedef statistics::detail::accumulator::
      	impl::empirical_distribution_int<boost::mpl::_1> impl;
    };
}

namespace extract
{

  	template<typename AccumulatorSet>
	typename boost::mpl::apply<
		AccumulatorSet,
        boost::statistics::detail::accumulator::tag::empirical_distribution_int
    >::type::result_type
  	empirical_distribution_int(AccumulatorSet const& acc)
    {
    	typedef boost::statistics::detail::accumulator::
    		tag::empirical_distribution_int the_tag;
        return boost::accumulators::extract_result<the_tag>(acc);
  	}

}

using extract::empirical_distribution_int;

}// accumulator
}// detail
}// statistics
}// boost

#endif