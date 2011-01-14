//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_UTILITY_SUB_RANGE_ER_2010_HPP
#define BOOST_ASSIGN_V2_UTILITY_SUB_RANGE_ER_2010_HPP
//#include <boost/range/sub_range.hpp>
#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>
#include <boost/range/iterator.hpp>
#include <boost/range/iterator_range.hpp>

namespace boost{
namespace assign{
namespace v2{
namespace result_of{
	
    template<typename R>
    struct sub_range
    {

		// typedef boost::sub_range<R> type; // Compiler error    
        // Workaround:
        typedef typename ::boost::range_iterator<R>::type it_;
        typedef typename ::boost::iterator_range<it_>::type type;
    };

}// result_of

    // Use it, for example, to do comparisons:
    // v2::sub_range( ref::csv_array(-1, 0 ,1) ) < r;

    template<typename R>
    typename v2::result_of::sub_range<R>::type
    sub_range( R& r)
    {
    	typedef typename v2::result_of::sub_range<R>::type result_;
    	return result_( boost::begin( r ), boost::end( r ) ); 
    }

    template<typename R>
    typename v2::result_of::sub_range<R const>::type
    sub_range( R const& r)
    {
    	typedef typename v2::result_of::sub_range<R const>::type result_;
    	return result_( boost::begin( r ), boost::end( r ) ); 
    }

}// v2
}// assign
}// boost

#endif
