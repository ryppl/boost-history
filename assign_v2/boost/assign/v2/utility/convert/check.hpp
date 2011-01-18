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
#include <boost/mpl/assert.hpp>
#include <boost/type.hpp>
#include <boost/array.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/assign/v2/utility/convert/converter.hpp>
#include <boost/assign/v2/utility/convert/deduce.hpp>
#include <boost/assign/v2/utility/convert/predicate.hpp>
#include <boost/assign/v2/detail/checking/fwd.hpp>

// Don't include this file in v2/utility/convert.hpp
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
    	namespace as2 = boost::assign::v2;
    	namespace ns = checking::convert;
        
        typedef boost::array<int, 8> ar_;
        typedef std::deque<int> deque_;
        typedef std::list<int> list_;
        typedef std::queue<int> queue_;
        typedef std::set<int> set_;
        typedef std::stack<int> stack_;
        typedef std::vector<int> vec_;
        
        BOOST_MPL_ASSERT(( as2::convert_aux::use_put<ar_, From> ));
        BOOST_MPL_ASSERT(( as2::convert_aux::use_put<stack_, From> ));
        BOOST_MPL_ASSERT(( as2::convert_aux::use_put<queue_, From> ));
        BOOST_MPL_ASSERT_NOT(( as2::convert_aux::use_put<deque_, From> ));
        BOOST_MPL_ASSERT_NOT(( as2::convert_aux::use_put<list_, From> ));
        BOOST_MPL_ASSERT_NOT(( as2::convert_aux::use_put<set_, From> ));
        BOOST_MPL_ASSERT_NOT(( as2::convert_aux::use_put<vec_, From> ));        

        BOOST_MPL_ASSERT(( boost::is_same< 
        		typename as2::convert_aux::deduce_tag<ar_, From>::type,
                convert_tag::put
            >
        ));
        BOOST_MPL_ASSERT(( boost::is_same< 
        		typename as2::convert_aux::deduce_tag<stack_, From>::type,
                convert_tag::put
            >
        ));
        BOOST_MPL_ASSERT(( boost::is_same< 
        		typename as2::convert_aux::deduce_tag<queue_, From>::type,
                convert_tag::put
            >
        ));
        BOOST_MPL_ASSERT(( boost::is_same< 
        		typename as2::convert_aux::deduce_tag<deque_, From>::type,
                convert_tag::copy
            >
        ));
        BOOST_MPL_ASSERT(( boost::is_same< 
        		typename as2::convert_aux::deduce_tag<list_, From>::type,
                convert_tag::copy
            >
        ));
        BOOST_MPL_ASSERT(( boost::is_same< 
        		typename as2::convert_aux::deduce_tag<set_, From>::type,
                convert_tag::copy
            >
        ));
        BOOST_MPL_ASSERT(( boost::is_same< 
        		typename as2::convert_aux::deduce_tag<vec_, From>::type,
                convert_tag::copy
            >
        ));

        
        // From is specified in case it is either of those in the lhs below.
        ns::do_check<queue_, From >( from );
        ns::do_check<stack_, From >( from );
        ns::do_check<ar_, From >( from );

        ns::do_check<deque_, From >( from );
        ns::do_check<list_, From >( from );
        ns::do_check<set_, From >( from );
        ns::do_check<vec_, From >( from );
    }

}// convert
}// checking
}// v2
}// assign
}// boost

#endif
