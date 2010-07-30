
#include <boost/mpl/repeat.hpp>

#include <boost/mpl/aux_/test.hpp>

MPL_TEST_CASE()
{
    typedef repeat<int,0>::type int0;
    MPL_ASSERT(( is_same< int0, package<> > ));
    typedef repeat<int,1>::type int1;
    MPL_ASSERT(( is_same< int1, package<int> > ));
    typedef repeat<int,2>::type int2;
    MPL_ASSERT(( is_same< int2, package<int,int> > ));
}
