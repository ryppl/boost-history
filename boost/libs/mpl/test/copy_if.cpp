//-----------------------------------------------------------------------------
// boost mpl/test/copy_if.cpp source file
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

#include "boost/mpl/copy_if.hpp"
#include "boost/mpl/type_list.hpp"
#include "boost/mpl/make_fun.hpp"
#include "boost/mpl/size.hpp"
#include "boost/mpl/at.hpp"
#include "boost/mpl/assert_is_same.hpp"
#include "boost/mpl/mpl_config.hpp"
#include "boost/type_traits/same_traits.hpp"
#include "boost/type_traits/arithmetic_traits.hpp"
#include "boost/static_assert.hpp"

namespace mpl = boost::mpl;

#if !defined(BOOST_NO_TEMPLATE_TEMPLATE_PARAMETERS)
    typedef mpl::make_f_x<boost::is_float> is_float_type;
#else
    BOOST_MPL_MAKE_F_X(is_float_type, boost::is_float);
#endif

int main()
{
    typedef mpl::type_list<int,float,long,double,char,long,double,float> types;
    typedef mpl::copy_if<
          types
        , mpl::type_list<>
        , is_float_type
        >::sequence float_types;

    BOOST_STATIC_ASSERT(mpl::size<float_types>::value == 4);
    BOOST_STATIC_ASSERT((boost::is_same< mpl::at<0,float_types>::type, float>::value));
    BOOST_STATIC_ASSERT((boost::is_same< mpl::at<1,float_types>::type, double >::value));
    BOOST_STATIC_ASSERT((boost::is_same< mpl::at<2,float_types>::type, double >::value));
    BOOST_STATIC_ASSERT((boost::is_same< mpl::at<3,float_types>::type, float >::value));

    return 0;
}
