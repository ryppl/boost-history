//Purpose:
//  Test boost::indexed_types::product
//
#include "boost/indexed_types/product.hpp"

enum index { a, b, c, last_ };

template<index> struct field_ndx;

template<> struct field_ndx<a> 
{
    static index const our_ndx=a;
};
template<> struct field_ndx<b> 
{
    static index const our_ndx=b;
};
template<> struct field_ndx<c> 
{
    static index const our_ndx=c;
};

template<unsigned i>
struct field_types
{
    typedef field_ndx<index(i)> type;
};

template<>
struct field_types<c>
//Purpose:
//  Illustrate duplication of types in 
//  SeqOfTypes(template arg to product)
//  is possible.
{
    typedef field_ndx<b> type;
};

//Acknowlegement:
//  The following upto the //==================== comment line
//  was from email with headers:
/*
From: 	Aleksey Gurtovoy <agurtovoy@meta-comm.com>
To: 	"'Larry Evans'" <cppljevans@cox-internet.com>
Subject: RE: typo mpl_paper.pdf  in 2.2.4 ?
Date: 	Fri, 3 Dec 2004 18:52:27 -0600 
 */
//

#include <boost/mpl/transform.hpp>
#include <boost/mpl/range_c.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/back_inserter.hpp>

template< typename Index > struct index_range;

template<
      template< unsigned > class FieldType
    >
struct make_field_type
{
    template< typename Index > struct apply
    {
        typedef typename FieldType<Index::value>::type type;
    };
};

template<> struct index_range<index>
    : boost::mpl::range_c<int,a,last_>
{
};

    typedef boost::mpl::transform<
          index_range<index>::type
        , make_field_type<field_types>
        , boost::mpl::back_inserter< boost::mpl::vector<> >
        >::type field_seq;

//=======================================        
#include <boost/test/unit_test.hpp>
#include <boost/type_traits/is_same.hpp>
using namespace boost;

namespace itps = indexed_types;


void field_seq_test()
{
    BOOST_CHECK((is_same<field_types<a>::type,mpl::at_c<field_seq,a>::type >::value));
    BOOST_CHECK((is_same<field_types<b>::type,mpl::at_c<field_seq,b>::type >::value));
    BOOST_CHECK((is_same<field_types<c>::type,mpl::at_c<field_seq,c>::type >::value));
    BOOST_CHECK((is_same<field_types<a>::type,field_ndx<a> >::value));
    BOOST_CHECK((is_same<field_types<b>::type,field_ndx<b> >::value));
    BOOST_CHECK((is_same<field_types<c>::type,field_ndx<b> >::value));
}

void factor_test()
{
    BOOST_CHECK((is_same<itps::factor<field_seq,a+1>::type,field_ndx<a> >::value));
    BOOST_CHECK((is_same<itps::factor<field_seq,b+1>::type,field_ndx<b> >::value));
    BOOST_CHECK((is_same<itps::factor<field_seq,c+1>::type,field_ndx<b> >::value));
}

void project_test()
{
    itps::product<field_seq> r;
    field_ndx<a>*pa = &r.project<a>();
    field_ndx<b>*pb = &r.project<b>();
    field_ndx<b>*pc = &r.project<c>();
    BOOST_CHECK((pa == &r.::itps::factor<field_seq,a+1>::my_field));
    BOOST_CHECK((pb == &r.::itps::factor<field_seq,b+1>::my_field));
    BOOST_CHECK((pc == &r.::itps::factor<field_seq,c+1>::my_field));
}

namespace test = unit_test_framework;

test::test_suite* init_unit_test_suite(int argc, char* argv[])
{
    test::test_suite* test = BOOST_TEST_SUITE("dependent function tests");
    test->add(BOOST_TEST_CASE(&field_seq_test));
    test->add(BOOST_TEST_CASE(&factor_test));
    test->add(BOOST_TEST_CASE(&project_test));
    return test;
}
