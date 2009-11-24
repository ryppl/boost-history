
// Copyright Larry Evans 2009
//
// $Id$
// $Date: 2009/11/13 17:02:07 $
// $Revision: 1.3 $

#include <boost/mpl/fold_assoc_pack.hpp>
#include <boost/mpl/pair.hpp>
#include <boost/mpl/int.hpp>
#include <boost/type_traits/is_same.hpp>

#include <boost/mpl/aux_/test.hpp>

//assoc_left:

MPL_TEST_CASE()
{
    typedef int_<0> z;
    {
        typedef fold_assoc_pack<assoc_left,pair,z,int_<1>,int_<2> >::type result;
        typedef pair<pair<z,int_<1> >,int_<2> > answer;
        MPL_ASSERT(( is_same<answer,result> ));
    }
    {
        typedef fold_assoc_pack<assoc_left,pair,z>::type result;
        MPL_ASSERT(( is_same<z,result> ));
    }
}

//assoc_right:

MPL_TEST_CASE()
{
    typedef int_<3> z;
    {
        typedef fold_assoc_pack<assoc_right,pair,z,int_<1>,int_<2> >::type result;
        typedef pair<int_<1>,pair<int_<2>,z> > answer;
        MPL_ASSERT(( is_same<answer,result> ));
    }
    {
        typedef fold_assoc_pack<assoc_right,pair,z>::type result;
        MPL_ASSERT(( is_same<z,result> ));
    }
}
