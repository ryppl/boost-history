//////////////////////////////////////////////////////////////////////////////
// range_ex::example::nested_chain.cpp                        				//
//                                                                          //
//  (C) Copyright 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#include <algorithm>
#include <iterator>
#include <vector>
#include <list>

#include <boost/typeof/typeof.hpp>
#include <boost/assign/std/vector.hpp>
#include <boost/assign/list_of.hpp>
#include <boost/range/algorithm/copy.hpp>
#include <boost/range/algorithm/equal.hpp>

#include <boost/fusion/include/make_vector.hpp>
#include <boost/fusion/include/at_c.hpp>
#include <boost/fusion/include/vector.hpp>
#include <boost/statistics/detail/range_ex/chain/nest.hpp>
#include <libs/statistics/detail/range_ex/example/nest_chain.h>

void example_nest_chain(std::ostream& os)
{

	namespace range = boost::statistics::detail::range;

	typedef int val_;
    typedef std::vector<val_> vec_;
    typedef std::list<val_> list_;

	using namespace boost::assign;

    typedef boost::fusion::vector<vec_,list_,vec_> seq_;

	seq_ seq;
    
    boost::fusion::at_c<0>(seq) += 0,1,2,3,4,5,6,7,8,9;
    boost::fusion::at_c<1>(seq) = list_of(10)(11)(12)(13)(14)(15)(16)(17)(18)(19);
    boost::fusion::at_c<2>(seq) += 20, 21, 22, 23, 24, 25, 26, 27, 28, 29;

	typedef range::result_of::nest_chain<seq_>::type chained_;

    chained_ chained = range::nest_chain( seq );
	vec_ vec;
    
    boost::copy(
    	boost::fusion::at_c<2>(seq), 
    	boost::copy(boost::fusion::at_c<1>(seq),
    		boost::copy(boost::fusion::at_c<0>(seq), std::back_inserter(vec))
        )
    );

	std::copy(
    	boost::begin(chained),
        boost::end(chained),
        std::ostream_iterator<val_>(std::cout," ")
    );

	BOOST_ASSERT(boost::equal(chained,vec));
    
}