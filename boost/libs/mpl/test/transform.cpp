//-----------------------------------------------------------------------------
// boost mpl/test/transform.cpp source file
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

#include "boost/mpl/transform.hpp"
#include "boost/mpl/type_list.hpp"
#include "boost/mpl/make_fun.hpp"
#include "boost/mpl/equal.hpp"
#include "boost/type_traits/transform_traits.hpp"
#include "boost/static_assert.hpp"

namespace mpl = boost::mpl;

#if !defined(BOOST_NO_TEMPLATE_TEMPLATE_PARAMETERS)
    typedef mpl::make_f_x<boost::add_pointer> add_pointer;
#else
    BOOST_MPL_MAKE_F_X(add_pointer, boost::add_pointer);
#endif

int main()
{    
    typedef mpl::type_list<char,short,int,long,float,double> types;
    typedef mpl::type_list<char*,short*,int*,long*,float*,double*> pointers;

    typedef mpl::transform<types, mpl::type_list<>, add_pointer>::sequence result;
    BOOST_STATIC_ASSERT((mpl::equal<result, pointers>::value == true));
    
    return 0;
}
