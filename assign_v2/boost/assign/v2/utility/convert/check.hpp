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
#include <boost/assign/v2/detail/check/fwd_equal_container.hpp>

// Don't include this file in v2/utility/convert.hpp
// To use it, precede by
// #include <boost/assign/v2/detail/checking/container.hpp>

namespace boost{
namespace assign{
namespace v2{
namespace check_aux{

    template<typename C, typename R>
    void equal_convert(R const& r)
    {
        C cont = converter( r );
        namespace ns = v2::check_aux;
        ns::equal_container( cont, r );
    }

    template<typename R>
    void equal_convert(R const& r)
    {
        namespace as2 = boost::assign::v2;
        namespace ns = check_aux;
        
        typedef boost::array<int, 8> ar_;
        typedef std::deque<int> deque_;
        typedef std::list<int> list_;
        typedef std::queue<int> queue_;
        typedef std::set<int> set_;
        typedef std::stack<int> stack_;
        typedef std::vector<int> vec_;
        
        BOOST_MPL_ASSERT(( as2::convert_aux::use_put<ar_, R> ));
        BOOST_MPL_ASSERT(( as2::convert_aux::use_put<stack_, R> ));
        BOOST_MPL_ASSERT(( as2::convert_aux::use_put<queue_, R> ));
        BOOST_MPL_ASSERT_NOT(( as2::convert_aux::use_put<deque_, R> ));
        BOOST_MPL_ASSERT_NOT(( as2::convert_aux::use_put<list_, R> ));
        BOOST_MPL_ASSERT_NOT(( as2::convert_aux::use_put<set_, R> ));
        BOOST_MPL_ASSERT_NOT(( as2::convert_aux::use_put<vec_, R> ));        

        BOOST_MPL_ASSERT(( boost::is_same< 
                typename as2::convert_aux::deduce_tag<ar_, R>::type,
                convert_tag::put
            >
        ));
        BOOST_MPL_ASSERT(( boost::is_same< 
                typename as2::convert_aux::deduce_tag<stack_, R>::type,
                convert_tag::put
            >
        ));
        BOOST_MPL_ASSERT(( boost::is_same< 
                typename as2::convert_aux::deduce_tag<queue_, R>::type,
                convert_tag::put
            >
        ));
        BOOST_MPL_ASSERT(( boost::is_same< 
                typename as2::convert_aux::deduce_tag<deque_, R>::type,
                convert_tag::copy
            >
        ));
        BOOST_MPL_ASSERT(( boost::is_same< 
                typename as2::convert_aux::deduce_tag<list_, R>::type,
                convert_tag::copy
            >
        ));
        BOOST_MPL_ASSERT(( boost::is_same< 
                typename as2::convert_aux::deduce_tag<set_, R>::type,
                convert_tag::copy
            >
        ));
        BOOST_MPL_ASSERT(( boost::is_same< 
                typename as2::convert_aux::deduce_tag<vec_, R>::type,
                convert_tag::copy
            >
        ));

        
        // R as 2nd arg is explicit in case it is the same as the 1st arg
        ns::equal_convert<queue_, R >( r );
        ns::equal_convert<stack_, R >( r );
        ns::equal_convert<ar_, R >( r );

        ns::equal_convert<deque_, R >( r );
        ns::equal_convert<list_, R >( r );
        ns::equal_convert<set_, R >( r );
        ns::equal_convert<vec_, R >( r );
    }

}// check_aux
}// v2
}// assign
}// boost

#endif
