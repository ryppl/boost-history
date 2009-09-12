
// Copyright Larry Evans 2009
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Id$
// $Date: 2009/08/29 21:19:01 $
// $Revision: 1.1 $

#include <boost/mpl/foldr_pack.hpp>
#include <boost/mpl/pair.hpp>
#include <boost/mpl/arg.hpp>
#include <boost/type_traits/is_same.hpp>

#include <boost/mpl/aux_/test.hpp>

MPL_TEST_CASE()
{
    typedef foldr_pack<int,pair,float,char>::type result;
    typedef pair<float,pair<char,int> > answer;
    MPL_ASSERT(( is_same<answer,result> ));
}
