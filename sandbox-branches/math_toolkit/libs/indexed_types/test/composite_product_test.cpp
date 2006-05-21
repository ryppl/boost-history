//Purpose:
//  Test boost::indexed_types::composite_product.hpp
//
#include "boost/indexed_types/composite_product.hpp"
#include "boost/indexed_types/typemap_enumerated.hpp"

enum indices { a, b, c };

namespace boost{namespace indexed_types{
template<>
struct enum_size<indices>{static unsigned const value=c+1;};
}}//exit boost::indexed_types namespace

template<indices> struct field_ndx;

template<> struct field_ndx<a> 
{
    static indices const our_ndx=a;
};
template<> struct field_ndx<b> 
{
    static indices const our_ndx=b;
};
template<> struct field_ndx<c> 
{
    static indices const our_ndx=c;
};

template<indices i>
struct field_types
{
    typedef field_ndx<i> type;
};

template<>
struct field_types<c>
//Purpose:
//  Illustrate repetition of types in 
//  TypeMap (template arg to product)
//  is possible.
{
    typedef field_ndx<b> type;
};

#include <boost/test/unit_test.hpp>
#include <boost/type_traits/is_same.hpp>
using namespace boost;

namespace itps = indexed_types;

typedef itps::typemap_enumerated<indices,field_types> named_types;

void field_types_test()
{
    BOOST_CHECK((is_same<field_types<a>::type,field_ndx<a> >::value));
    BOOST_CHECK((is_same<field_types<b>::type,field_ndx<b> >::value));
    BOOST_CHECK((is_same<field_types<c>::type,field_ndx<b> >::value));
}

void factor_test()
{
    BOOST_CHECK((is_same<itps::factor<named_types,a+1>::type,field_ndx<a> >::value));
    BOOST_CHECK((is_same<itps::factor<named_types,b+1>::type,field_ndx<b> >::value));
    BOOST_CHECK((is_same<itps::factor<named_types,c+1>::type,field_ndx<b> >::value));
}

void project_test()
{
    itps::composite_type<itps::operator_product,named_types> r;
    field_ndx<a>*pa = &r.project<a>();
    field_ndx<b>*pb = &r.project<b>();
    field_ndx<b>*pc = &r.project<c>();
  #if 0  
    BOOST_CHECK((pa == &r.::itps::factor<named_types,a+1>::my_field));
    BOOST_CHECK((pb == &r.::itps::factor<named_types,b+1>::my_field));
    BOOST_CHECK((pc == &r.::itps::factor<named_types,c+1>::my_field));
  #endif
}

namespace test = unit_test_framework;

test::test_suite* init_unit_test_suite(int argc, char* argv[])
{
    test::test_suite* test = BOOST_TEST_SUITE("cartesian product tests");
    test->add(BOOST_TEST_CASE(&field_types_test));
    test->add(BOOST_TEST_CASE(&factor_test));
    test->add(BOOST_TEST_CASE(&project_test));
    return test;
}
