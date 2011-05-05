//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2011 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_CONVERSION_CHECK_ER_2011_HPP
#define BOOST_ASSIGN_V2_CONVERSION_CHECK_ER_2011_HPP
#include <deque>
#include <list>
#include <map>
#include <queue>
#include <set>
#include <stack>
#include <vector>
#include <boost/array.hpp>
#include <boost/assign/v2/conversion/converter.hpp>
#include <boost/assign/v2/conversion/deduce_tag.hpp>
#include <boost/assign/v2/support/check/fwd_equal_container.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/type.hpp>
#include <boost/type_traits/is_same.hpp>

// Don't include this file in v2/utility/conversion.hpp

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
        namespace as2 = assign::v2;
        namespace ns = check_aux;

        typedef boost::array<int, 8> ar_;
        typedef std::deque<int> deque_;
        typedef std::list<int> list_;
        typedef std::queue<int> queue_;
        typedef std::set<int> set_;
        typedef std::stack<int> stack_;
        typedef std::vector<int> vec_;

        BOOST_MPL_ASSERT(( boost::is_same<
                typename as2::conversion_aux::deduce_tag<ar_, R>::type,
                convert_tag::put
            >
        ));
        BOOST_MPL_ASSERT(( boost::is_same<
                typename as2::conversion_aux::deduce_tag<stack_, R>::type,
                convert_tag::put
            >
        ));
        BOOST_MPL_ASSERT(( boost::is_same<
                typename as2::conversion_aux::deduce_tag<queue_, R>::type,
                convert_tag::put
            >
        ));
        BOOST_MPL_ASSERT(( boost::is_same<
                typename as2::conversion_aux::deduce_tag<deque_, R>::type,
                convert_tag::copy
            >
        ));
        BOOST_MPL_ASSERT(( boost::is_same<
                typename as2::conversion_aux::deduce_tag<list_, R>::type,
                convert_tag::copy
            >
        ));
        BOOST_MPL_ASSERT(( boost::is_same<
                typename as2::conversion_aux::deduce_tag<set_, R>::type,
                convert_tag::copy
            >
        ));
        BOOST_MPL_ASSERT(( boost::is_same<
                typename as2::conversion_aux::deduce_tag<vec_, R>::type,
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

#endif // BOOST_ASSIGN_V2_CONVERSION_CHECK_ER_2011_HPP
