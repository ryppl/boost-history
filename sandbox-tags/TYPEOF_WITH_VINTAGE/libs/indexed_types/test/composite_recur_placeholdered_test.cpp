//Purpose:
//  Same as recur_test.cpp but with type_operators and  composite_type
//  and placeholders.
#include "boost/indexed_types/composite_product.hpp"
#include "boost/indexed_types/composite_sum.hpp"
#include "boost/indexed_types/typemap_placeholdered_enumerated.hpp"
#include <boost/type_traits/is_same.hpp>
#include <boost/test/unit_test.hpp>
//{test decls

  enum
emptiness
{ yes_empty
, not_empty
};
  enum
list_accessors
{ list_head
, list_tail
};

namespace boost{namespace indexed_types{
template<>
struct enum_size<emptiness>
{
    static unsigned const value=not_empty+1;
};
template<>
struct enum_size<list_accessors>
{
    static unsigned const value=list_tail+1;
};
}}

unsigned const emptiness_size = boost::indexed_types::enum_size<emptiness>::value;
unsigned const list_accessors_size = boost::indexed_types::enum_size<list_accessors>::value;

using namespace boost;
using namespace indexed_types;

  struct
empty_list
{
};
//}test decls

//{tests:

namespace sum_emptiness
{
template
  < typename
  >
  struct
type_map
;
template
  <
  >
  struct
type_map
  < mpl::integral_c<emptiness,yes_empty>
  >
{
    typedef empty_list type;
};

template
  <
  >
  struct
type_map
  < mpl::integral_c<emptiness,not_empty>
  >
{
    typedef int type;
};

void test(void)
{
      composite_type
      < operator_sum
      , typemap_placeholdered_enumerated<emptiness,type_map<mpl::_1> > 
      > 
    l_test;
    BOOST_CHECK(l_test.index() == emptiness_size);
    l_test.inject<yes_empty>(empty_list());
    BOOST_CHECK(l_test.index() == yes_empty);
}

}//exit sum_emptiness namespace

namespace mpl_list
{

template
  < typename T
  >
  struct
recur_list
;
template
  < typename T
  , typename ListAccessor
  >
  struct
product_map
;
template
  < typename T
  >
  struct
product_map
  < T
  , mpl::integral_c<list_accessors,list_head>
  >
{
        typedef
      T
    type
    ;
};
template
  < typename T
  >
  struct
product_map
  < T
  , mpl::integral_c<list_accessors,list_tail>
  >
{
        typedef
      recur_list<T>*
    type
    ;
};
template
  < typename T
  , typename Emptiness
  >
  struct
sum_map
;
template
  < typename T
  >
  struct
sum_map
  < T
  , mpl::integral_c<emptiness,yes_empty>
  >
{
        typedef
      empty_list
    type
    ;
};
template
  < typename T
  >
  struct
sum_map
  < T
  , mpl::integral_c<emptiness,not_empty>
  >
{
        typedef
      composite_type
      < operator_product
      , typemap_placeholdered_enumerated<list_accessors,product_map<T,mpl::_1> > 
      >
    type
    ;
};
template
  < typename T
  >
  struct
recur_list
  : public composite_type
    < operator_sum
    , typemap_placeholdered_enumerated<emptiness,sum_map<T,mpl::_1> > 
    >
{
        typedef 
      composite_type
      < operator_sum
      , typemap_placeholdered_enumerated<emptiness,sum_map<T,mpl::_1> > 
      >
    super_type
    ;
    recur_list(void)
    {
        super_type& me=*this;
        empty_list e_list;
        me.inject<yes_empty>(e_list);
    }
};

template
  < typename T
  >
  recur_list<T>*
push_head(T a_head, recur_list<T>* a_tail)
{
    recur_list<T>*l_list=new recur_list<T>;
    l_list->inject<not_empty>(sum_map<T,mpl::integral_c<emptiness,not_empty> >::type());
    l_list->project<not_empty>().project<list_head>()= a_head;
    l_list->project<not_empty>().project<list_tail>()= a_tail;
    return l_list;
}

void test(void)
{
    BOOST_CHECK((is_same<product_map<int,mpl::integral_c<list_accessors,list_head> >::type,int>::value));
    recur_list<int>* mpl_ilist= new recur_list<int>;
    BOOST_CHECK(mpl_ilist->index() == yes_empty);
    int hd= 999;
    mpl_ilist= push_head(hd, mpl_ilist);
    BOOST_CHECK(mpl_ilist->index() == not_empty);
    BOOST_CHECK(mpl_ilist->project<not_empty>().project<list_head>() == hd);
    BOOST_CHECK(mpl_ilist->project<not_empty>().project<list_tail>()->index() == yes_empty);
}

}//exit mpl_list namespace

//}tests
namespace test = boost::unit_test_framework;
test::test_suite* init_unit_test_suite(int argc, char* argv[])
{
    test::test_suite* test = BOOST_TEST_SUITE("indexed_types recur tests");
    test->add(BOOST_TEST_CASE(&sum_emptiness::test));
    test->add(BOOST_TEST_CASE(&mpl_list::test));
    return test;
}
