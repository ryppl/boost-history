//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_CONVERT_CHECK_ER_2010_HPP
#define BOOST_ASSIGN_V2_CONVERT_CHECK_ER_2010_HPP
#include <deque>
#include <list>
#include <map>
#include <queue>
#include <set>
#include <stack>
#include <vector>
#include <boost/type.hpp>
#include <boost/array.hpp>
#include <boost/assign/v2/misc/convert/converter.hpp>
#include <boost/assign/v2/detail/checking/fwd.hpp>

// Don't include this file in v2/misc/convert.hpp
// To use it, precede by
// #include <boost/assign/v2/detail/checking/container.hpp>

namespace boost{
namespace assign{
namespace v2{
namespace checking{
namespace convert{

	template<typename To, typename From>
    void do_check(From const& from)
    {
        namespace ns = checking::container;
        To to = converter( from );
        ns::do_check( to );
        // ns::do_check( To( converter( from ) ) ); 
    }

	template<typename From>
    void do_check(From const& from)
    {
    	namespace ns = checking::convert;
        // From is specified in case it is either of those in the lhs below.
        ns::do_check<boost::array<int, 8>, From >( from );
        ns::do_check<std::deque<int>, From >( from );
        ns::do_check<std::list<int>, From >( from );
        ns::do_check<std::queue<int>, From >( from );
        ns::do_check<std::set<int>, From >( from );
        ns::do_check<std::stack<int>, From >( from );
        ns::do_check<std::vector<int>, From >( from );
    }

}// convert
}// checking
}// v2
}// assign
}// boost

#endif
