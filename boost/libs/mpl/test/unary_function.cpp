//-----------------------------------------------------------------------------
// boost mpl/test/unary_function.cpp source file
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

#include "boost/mpl/unary_function.hpp"
#include "boost/mpl/assert_is_same.hpp"
#include "boost/static_assert.hpp"
#include "boost/config.hpp"

namespace mpl = boost::mpl;

struct type_function
{
    template<typename U> struct apply
    {
        typedef U type;
    };
};

struct value_function
{
    template<typename U> struct apply
    {
        BOOST_STATIC_CONSTANT(bool, value = true);
    };
};


int main()
{
    typedef mpl::unary_function<type_function,int>::type type;
    BOOST_MPL_ASSERT_IS_SAME(type, int);
    BOOST_STATIC_ASSERT((mpl::unary_function<value_function,int>::value));
    
    return 0;
}
