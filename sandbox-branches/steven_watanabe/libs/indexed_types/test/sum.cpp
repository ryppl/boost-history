//Purpose:
//  Test boost::indexed_types::sum
//
#include "boost/indexed_types/sum.hpp"

enum indices { a, b, c };

namespace boost{namespace indexed_types{
template<>
struct enum_size<indices>{static unsigned const value=c+1;};
}}//exit boost::indexed_types namespace

template<indices> struct field_ndx;

template<> struct field_ndx<a> 
{
    static indices const our_ndx=a;
    char memory[(our_ndx+1)*10];
};
template<> struct field_ndx<b> 
{
    static indices const our_ndx=b;
    char memory[(our_ndx+1)*10];
};
template<> struct field_ndx<c> 
{
    static indices const our_ndx=c;
    char memory[(our_ndx+1)*10];
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
//  TypeMap (template arg to sum)
//  is possible.
{
    typedef field_ndx<b> type;
};

#include <boost/test/unit_test.hpp>
#include <boost/type_traits/is_same.hpp>
using namespace boost;

namespace itps = indexed_types;



void project_test()
{
    typedef itps::sum<indices,field_types> sum_type;
    unsigned const map_size=itps::enum_size<indices>::value;
    BOOST_CHECK(sum_type::map_size == map_size);
    BOOST_CHECK(sum_type::max_size == sizeof(field_ndx<b>));
    sum_type r;
    BOOST_CHECK(r.index() == map_size);
    BOOST_CHECK(0 == r.project<a>());
    BOOST_CHECK(0 == r.project<b>());
    BOOST_CHECK(0 == r.project<c>());
    field_ndx<a>*pa = r.inject<a>(field_ndx<a>());
    BOOST_CHECK(r.index() == a);
    BOOST_CHECK(pa == r.project<a>());
    BOOST_CHECK(0 == r.project<b>());
    BOOST_CHECK(0 == r.project<c>());
    field_ndx<b>*pb = r.inject<b>(field_ndx<b>());
    BOOST_CHECK(r.index() == b);
    BOOST_CHECK(0 == r.project<a>());
    BOOST_CHECK(pb == r.project<b>());
    BOOST_CHECK(0 == r.project<c>());
}

namespace test = unit_test_framework;

test::test_suite* init_unit_test_suite(int argc, char* argv[])
{
    test::test_suite* test = BOOST_TEST_SUITE("dependent function tests");
    test->add(BOOST_TEST_CASE(&project_test));
    return test;
}
