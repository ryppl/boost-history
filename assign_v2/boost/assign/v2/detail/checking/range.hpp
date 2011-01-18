//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_CHECKING_RANGE_ER_2010_HPP
#define BOOST_ASSIGN_V2_CHECKING_RANGE_ER_2010_HPP
#include <boost/next_prior.hpp>
#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>
#include <boost/assign/v2/detail/traits/container/is_range.hpp>
#include <boost/assign/v2/detail/traits/container/is_sorted.hpp>
#include <boost/assign/v2/detail/checking/check.hpp>
#include <boost/assign/v2/detail/checking/iterator.hpp>

namespace boost{                                                               
namespace assign{ 
namespace v2{
namespace checking{ 
    
    template<typename R>
    void do_check(container_tag::range, R const& r)
    {
    	namespace ns = checking::iterator;     
        ns::do_check( boost::begin( r ) );
        BOOST_ASSIGN_V2_CHECK( 
            boost::next( boost::begin( r ), 8 ) == boost::end( r ) 
        );
    }                                                                              

    template<typename R>
    void do_check(container_tag::sorted, R const& r)
    {                                                                              
    	namespace ns = checking::iterator::sorted;     
        ns::do_check( boost::begin( r ) );
        BOOST_ASSIGN_V2_CHECK( 
            boost::next( boost::begin( r ), 8 ) == boost::end( r ) 
        );
    }                                                                              

}// checking
}// v2
}// assign
}// boost    
    
#endif
