
// Copyright Aleksey Gurtovoy 2000-2004
// Copyright David Abrahams 2003-2004
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Id$
// $Date: 2009/03/28 15:50:03 $
// $Revision: 1.2 $

#include <boost/mpl/remove.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/equal.hpp>

#include <boost/mpl/aux_/test.hpp>


MPL_TEST_CASE()
{
    typedef vector<int,float,char,float,float,double> types;
    typedef mpl::remove< types,float >::type result;
    typedef vector<int,char,double> answer;
    MPL_ASSERT(( equal< result,answer > ));
}
