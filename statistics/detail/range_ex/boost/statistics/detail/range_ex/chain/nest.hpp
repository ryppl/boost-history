///////////////////////////////////////////////////////////////////////////////
// range_ex::chain::nest.hpp                              				 	 //
//                                                                           //
//  Copyright 2010 Erwann Rogard. Distributed under the Boost                //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
///////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_STATISTICS_DETAIL_RANGE_EX_CHAIN_NEST_HPP_ER_2010
#define BOOST_STATISTICS_DETAIL_RANGE_EX_CHAIN_NEST_HPP_ER_2010
#include <boost/mpl/equal_to.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/greater.hpp>

#include <boost/fusion/include/back.hpp>
#include <boost/fusion/include/pop_back.hpp>
#include <boost/fusion/include/size.hpp>

#include <boost/statistics/detail/range_ex/chain/result_of.hpp>
#include <boost/statistics/detail/range_ex/chain/detail/nest.hpp>

namespace boost{
namespace statistics{
namespace detail{
namespace range{

namespace result_of{

	// TODO :
    // case size(seq) = 1 ?
    // Why Most1!= Most

	template<typename Seq>
    struct nest_chain : range::impl::nest_chain<
    	typename boost::remove_const<
    		typename boost::remove_reference<
    			typename boost::fusion::result_of::pop_back<Seq>::type
            >::type
        >::type,
    	typename boost::remove_const<
    		typename boost::remove_reference<
        		typename boost::fusion::result_of::back<Seq>::type
            >::type
        >::type
    >{};


	// Alternatively?
    //	template<typename Seq>
	//	struct nest_chain{
    //    	typedef typename statistics::detail::mpl::most<Seq>::type most_;
    //     	typedef typename boost::mpl::back<Seq>::type back_;
    //     
    //       typedef typename boost::mpl::reverse_fold<
    //        	most_,
    //            back_,
    //        	statistics::detail::range::result_of
    //            	::chain<boost::mpl::_2,boost::mpl::_1>
    //        >::type
    //    >{};


}

	// Possible Usage:
    // nest_chain(fusion::make_vector(vec1,vec2,vec3,...)); 	
    template<typename Seq>
    typename range::result_of::nest_chain<Seq>::type
    nest_chain(const Seq& seq){
    	typedef typename boost::fusion::result_of::size<Seq>::type size_;
        typedef boost::mpl::int_<1> int1_;
    
    	BOOST_MPL_ASSERT(( boost::mpl::greater<size_,int1_>));
    
    	typedef typename 
        	boost::fusion::result_of::pop_back<Seq>::type cref_most_;
        typedef typename boost::remove_const<
        	typename boost::remove_reference<
            	cref_most_
            >::type
        >::type most_;
    	typedef typename boost::fusion::result_of::back<Seq>::type cref_back_;
        typedef typename boost::remove_const<
        	typename boost::remove_reference<
            	cref_back_
            >::type
        >::type back_;
    
    	typedef range::impl::nest_chain<most_,back_> meta_;
		return meta_::call(
        	boost::fusion::pop_back(seq),
            boost::fusion::back(seq)
        );    
    }

}// range
}// detail
}// statistics
}// boost

#endif
