//-----------------------------------------------------------------------------
// boost mpl/test/switch_on.cpp source file
// See http://www.boost.org for updates, documentation, and revision history.
//-----------------------------------------------------------------------------
//
// Copyright (c) 2000-01
// Aleksey Gurtovoy
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is hereby granted without fee, 
// provided that the above copyright notice appears in all copies and 
// that both the copyright notice and this permission notice appear in 
// supporting documentation. No representations are made about the 
// suitability of this software for any purpose. It is provided "as is" 
// without express or implied warranty.

#include "boost/mpl/switch_on.hpp"
#include "boost/mpl/int_t.hpp"
#include "boost/mpl/assert_is_same.hpp"
#include "boost/static_assert.hpp"
#include "boost/config.hpp"

namespace mpl = boost::mpl;
    
template<typename T>
struct type_to_value
{
    BOOST_STATIC_CONSTANT(long, value = 
        (mpl::switch_on< T
            , mpl::type_case<char>          ::value<1>
            , mpl::type_case<signed char>   ::value<2>
            , mpl::type_case<unsigned char> ::value<3>
            , mpl::type_case<signed short>  ::value<10>
            , mpl::type_case<unsigned short>::value<11>
            , mpl::type_case<signed int>    ::value<20>
            , mpl::type_case<unsigned int>  ::value<21>
            , mpl::type_case<signed long>   ::value<30>
            , mpl::type_case<unsigned long> ::value<31>
            , mpl::default_case             ::value<-1>
            >::value)
        );
};

template<typename T>
struct value_to_type
{
    typedef typename mpl::switch_on< T
            , mpl::value_case<1>    ::type<char>
            , mpl::value_case<2>    ::type<signed char>
            , mpl::value_case<3>    ::type<unsigned char>
            , mpl::value_case<10>   ::type<signed short>  
            , mpl::value_case<11>   ::type<unsigned short>
            , mpl::value_case<20>   ::type<signed int>  
            , mpl::value_case<21>   ::type<unsigned int>  
            , mpl::value_case<30>   ::type<signed long> 
            , mpl::value_case<31>   ::type<unsigned long> 
            , mpl::default_case     ::type<void>
            >::type type;
};


int main()
{
    BOOST_MPL_ASSERT_IS_SAME(value_to_type< type_to_value<char> >::type, char);
    BOOST_MPL_ASSERT_IS_SAME(value_to_type< type_to_value<int> >::type, int);
    BOOST_MPL_ASSERT_IS_SAME(value_to_type< type_to_value<long> >::type, long);

    return 0;
}
