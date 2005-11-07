//test child_i_depth_j
#include "boost/mpl/child_i_depth_j.hpp"

#include <boost/mpl/fold.hpp>
#include <boost/mpl/range_c.hpp>
#include <boost/mpl/empty_base.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/mpl/next_prior.hpp>
#include <boost/type_traits/is_same.hpp>

using namespace boost;

template<class T1, class T2>
struct inherit1_of2
  : public T1
{
    typedef inherit1_of2 type;
    T2 v2;
};

template<class T1, class T2, class T3>
struct inherit1_of3
  : public T1
{
    typedef inherit1_of3 type;
    T2 v2;
    T3 v3;
};

template<class T1, class T2, class T3>
struct inherit2_of3
  : public T2
{
    typedef inherit2_of3 type;
    T1 v1;
    T3 v3;
};

void test1_of2(void)
{
        typedef
      inherit1_of2<mpl::arg<1>,mpl::arg<2> >
    app2_type
    ;
        typedef
      mpl::int_<1>
    child_index_type
    ;
        typedef
      mpl::range_c<int,0,0>
    depth0_type
    ;
        typedef
      mpl::range_c<int,0,1>
    depth1_type
    ;
        typedef
      mpl::range_c<int,0,2>
    depth2_type
    ;
        typedef
      mpl::fold<depth0_type,mpl::empty_base,app2_type>::type
    app2_depth0_type
    ;
        typedef
      mpl::fold<depth1_type,mpl::empty_base,app2_type>::type
    app2_depth1_type
    ;
        typedef
      mpl::fold<depth2_type,mpl::empty_base,app2_type>::type
    app2_depth2_type
    ;
        typedef 
      mpl::child_i_depth_j
      < app2_depth2_type
      , child_index_type
      , mpl::int_<0>
      >::type 
    type_1_0
    ;
    BOOST_MPL_ASSERT((is_same<type_1_0,app2_depth2_type>));
        typedef 
      mpl::child_i_depth_j
      < app2_depth2_type
      , child_index_type
      , mpl::int_<1>
      >::type 
    type_1_1
    ;
    BOOST_MPL_ASSERT((is_same<type_1_1,app2_depth1_type>));
        typedef 
      mpl::child_i_depth_j
      < app2_depth2_type
      , child_index_type
      , mpl::int_<2>
      >::type 
    type_1_2
    ;
    BOOST_MPL_ASSERT((is_same<type_1_2,app2_depth0_type>));
}

void test1_of3(void)
{
        typedef
      inherit1_of3<mpl::arg<1>,mpl::arg<2>,mpl::int_<3> >
    app2_type
    ;
        typedef
      mpl::int_<1>
    child_index_type
    ;
        typedef
      mpl::range_c<int,0,0>
    depth0_type
    ;
        typedef
      mpl::range_c<int,0,1>
    depth1_type
    ;
        typedef
      mpl::range_c<int,0,2>
    depth2_type
    ;
        typedef
      mpl::fold<depth0_type,mpl::empty_base,app2_type>::type
    app2_depth0_type
    ;
        typedef
      mpl::fold<depth1_type,mpl::empty_base,app2_type>::type
    app2_depth1_type
    ;
        typedef
      mpl::fold<depth2_type,mpl::empty_base,app2_type>::type
    app2_depth2_type
    ;
        typedef 
      mpl::child_i_depth_j
      < app2_depth2_type
      , child_index_type
      , mpl::int_<0>
      >::type 
    type_1_0
    ;
    BOOST_MPL_ASSERT((is_same<type_1_0,app2_depth2_type>));
        typedef 
      mpl::child_i_depth_j
      < app2_depth2_type
      , child_index_type
      , mpl::int_<1>
      >::type 
    type_1_1
    ;
    BOOST_MPL_ASSERT((is_same<type_1_1,app2_depth1_type>));
        typedef 
      mpl::child_i_depth_j
      < app2_depth2_type
      , child_index_type
      , mpl::int_<2>
      >::type 
    type_1_2
    ;
    BOOST_MPL_ASSERT((is_same<type_1_2,app2_depth0_type>));
}

void test2_of3(void)
{
        typedef
      inherit2_of3<mpl::int_<3>,mpl::arg<1>,mpl::arg<2> >
    app2_type
    ;
        typedef
      mpl::int_<2>
    child_index_type
    ;
        typedef
      mpl::range_c<int,0,0>
    depth0_type
    ;
        typedef
      mpl::range_c<int,0,1>
    depth1_type
    ;
        typedef
      mpl::range_c<int,0,2>
    depth2_type
    ;
        typedef
      mpl::fold<depth0_type,mpl::empty_base,app2_type>::type
    app2_depth0_type
    ;
        typedef
      mpl::fold<depth1_type,mpl::empty_base,app2_type>::type
    app2_depth1_type
    ;
        typedef
      mpl::fold<depth2_type,mpl::empty_base,app2_type>::type
    app2_depth2_type
    ;
        typedef 
      mpl::child_i_depth_j
      < app2_depth2_type
      , child_index_type
      , mpl::int_<0>
      >::type 
    type_1_0
    ;
    BOOST_MPL_ASSERT((is_same<type_1_0,app2_depth2_type>));
        typedef 
      mpl::child_i_depth_j
      < app2_depth2_type
      , child_index_type
      , mpl::int_<1>
      >::type 
    type_1_1
    ;
    BOOST_MPL_ASSERT((is_same<type_1_1,app2_depth1_type>));
        typedef 
      mpl::child_i_depth_j
      < app2_depth2_type
      , child_index_type
      , mpl::int_<2>
      >::type 
    type_1_2
    ;
    BOOST_MPL_ASSERT((is_same<type_1_2,app2_depth0_type>));
}

  template
  < class Application
  >
struct application_parts_inherit2_of3
;
  template
  < class T0
  , class T1
  , class T2
  >
struct application_parts_inherit2_of3
  < inherit2_of3<T0,T1,T2>
  >
  : public mpl::application_parts< inherit2_of3<T0,T1,T2> >
{
};

void test_app_parts2_of3(void)
{
        typedef
      inherit2_of3<mpl::int_<3>,mpl::arg<1>,mpl::arg<2> >
    app2_type
    ;
        typedef
      mpl::int_<2>
    child_index_type
    ;
        typedef
      mpl::range_c<int,0,0>
    depth0_type
    ;
        typedef
      mpl::range_c<int,0,1>
    depth1_type
    ;
        typedef
      mpl::range_c<int,0,2>
    depth2_type
    ;
        typedef
      mpl::fold<depth0_type,mpl::empty_base,app2_type>::type
    app2_depth0_type
    ;
        typedef
      mpl::fold<depth1_type,mpl::empty_base,app2_type>::type
    app2_depth1_type
    ;
        typedef
      mpl::fold<depth2_type,mpl::empty_base,app2_type>::type
    app2_depth2_type
    ;
        typedef 
      mpl::child_i_depth_j
      < app2_depth2_type
      , child_index_type
      , mpl::int_<0>
      , application_parts_inherit2_of3<mpl::arg<1> >
      >::type 
    type_1_0
    ;
    BOOST_MPL_ASSERT((is_same<type_1_0,app2_depth2_type>));
        typedef 
      mpl::child_i_depth_j
      < app2_depth2_type
      , child_index_type
      , mpl::int_<1>
      , application_parts_inherit2_of3<mpl::arg<1> >
      >::type 
    type_1_1
    ;
    BOOST_MPL_ASSERT((is_same<type_1_1,app2_depth1_type>));
        typedef 
      mpl::child_i_depth_j
      < app2_depth2_type
      , child_index_type
      , mpl::int_<2>
      , application_parts_inherit2_of3<mpl::arg<1> >
      >::type 
    type_1_2
    ;
    BOOST_MPL_ASSERT((is_same<type_1_2,app2_depth0_type>));
}

void test_doc(void)
//Test what's in documentation
{
    typedef mpl::int_<99>     app_3;
    typedef mpl::prior<app_3> app_2;
    typedef mpl::prior<app_2> app_1;
    typedef mpl::prior<app_1> app_0;
        typedef
      mpl::int_<1>
    child_index_type
    ;
    BOOST_MPL_ASSERT(( is_same< 
          mpl::child_i_depth_j<app_0,child_index_type,mpl::int_<0> >::type
        , app_0
        > ));
    BOOST_MPL_ASSERT(( is_same< 
          mpl::child_i_depth_j<app_0,child_index_type,mpl::int_<3> >::type
        , app_3
        > ));
}
