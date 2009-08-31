
// Copyright Aleksey Gurtovoy 2002-2004
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Id$
// $Date: 2009/08/27 23:59:24 $
// $Revision: 1.8 $

#include <boost/mpl/unpack_args.hpp>

#include <boost/mpl/vector.hpp>
#include <boost/mpl/aux_/test.hpp>
#include <boost/type_traits/is_same.hpp>

MPL_TEST_CASE()
{
    MPL_ASSERT(( apply< 
          unpack_args< is_same<_1,_2> >
        , vector<int,int>
        > ));
}
