
// Copyright Aleksey Gurtovoy 2002-2004
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Source$
// $Date$
// $Revision$

#include <boost/mpl/zip_view.hpp>
#include <boost/mpl/transform_view.hpp>
#include <boost/mpl/filter_view.hpp>
#include <boost/mpl/range_c.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/at.hpp>
#include <boost/mpl/equal.hpp>
#include <boost/mpl/equal_to.hpp>
#include <boost/mpl/math/is_even.hpp>
#include <boost/mpl/aux_/test.hpp>


template< typename BinaryOp >
struct apply_two_args_seq
{
    template< typename Args > struct apply
    {
        typedef typename apply2<
              BinaryOp
            , typename at_c<Args,0>::type
            , typename at_c<Args,1>::type
            >::type type;
    };
};

MPL_TEST_CASE()
{
    typedef transform_view<
          zip_view< vector< range_c<int,0,10>, range_c<int,10,20> > >
        , apply_two_args_seq< plus<> >
        > result;

    MPL_ASSERT(( equal< 
          result
        , filter_view< range_c<int,10,30>, is_even<_> >
        , equal_to<_,_>
        > ));
}
