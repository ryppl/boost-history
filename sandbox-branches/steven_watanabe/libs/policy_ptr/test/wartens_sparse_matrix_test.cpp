#include "boost/policy_ptr/detail/collector_sp_counted_accepting.hpp"
#include "boost/policy_ptr/std_ptr.hpp"
#include "boost/fields_visitor/container_extern/compare_double.hpp"
#include <boost/mpl/vector.hpp>
#include <map>
#include <iostream>
using namespace std;

struct Element;

    typedef
  boost::policy_ptr::detail::collector
  < boost::policy_ptr::detail::sp_counted_accepting
  >
collector_type
;
    typedef 
  boost::std_ptr<Element>::shared_accepting 
element_sptr
;
    typedef
  boost::mpl::vector<boost::policy_ptr::detail::shared_count_visitor_abs>
visitor_seq
;
//row of sparse matrix, pointers to elements keyed by unsigned
    typedef
  boost::fields_visitor::container_extern::compare_double
  < std::map
  , unsigned
  , element_sptr
  , visitor_seq
  >
Row_T
;
//sparse matrix, pointers to rows keyed by unsigned
    typedef 
  boost::fields_visitor::container_extern::compare_double
  < std::map
  , unsigned
  , Row_T
  , visitor_seq
  >
Matrix_T
;
//each element holds a sparse matrix of elements,
//this is a tree-like data structure 
struct Element {
        static unsigned created;
        static unsigned destroyed;
        Matrix_T mat;
        Element(){
                ++created;
        }
        Element(const Element& n){
                ++created;
        }
        ~Element()
        {
                cout<<"delete"<<endl;
                ++destroyed;
        }
};
unsigned Element::created(0);
unsigned Element::destroyed(0);

SELECTED_FIELDS_DESCRIPTION_OF_RECORD(Element)

#include <boost/test/unit_test.hpp>

void testgc(){
        element_sptr someElement(new Element());
        for (unsigned i=0; i<1; i++)
        {
                element_sptr n1(new Element());
                element_sptr n2(new Element());

                n2->mat[1][1] = n1;
                n2->mat[2][2] = n1;
                n2->mat[3][3] = someElement;
                n2->mat[4][4] = n2;//create cycle
        }
}
void test_main()
{
        testgc();
        collector_type::collect();
        cout << "Elements created: " << Element::created << endl;
        cout << "Elements destroyed: " << Element::destroyed << endl;
        BOOST_CHECK_EQUAL(Element::created, Element::destroyed);
}

void always_fail(void)
{
    BOOST_CHECK(false);
}

namespace butf = boost::unit_test_framework;
butf::test_suite* init_unit_test_suite(int argc, char* argv[])
{
    butf::test_suite* tests = BOOST_TEST_SUITE("wartens_sparse_matrix tests");
    tests->add(BOOST_TEST_CASE(&test_main));
//    tests->add(BOOST_TEST_CASE(always_fail));

    return tests;
}

//=====================================================================
//ChangeLog:
//  2005-08-27: Larry Evans
//    WHAT:
//      1) created cycle.
//      2) Used SELECTED_FIELDS_DESCRIPTION_OF_RECORD on Element.
//    WHY:
//      1) see if collector<sp_counted_accepting> could collect it.
//      2) To create field (i.e. smart_ptr) descriptor for Element.
//    RESULT:
//      Test passed.
//  2005-08-15: Larry Evans
//    WHAT:
//      1) replaced gc_*.hpp includes with boost/policy_ptr includes.
//      2) replaced all raw pointers with policy_ptr's able to
//         collect cycles.
//      3) added explicit call to collector.
//      4) replaced main() with boost's init_unit_test_suite.
//  2005-08-13: Larry Evans
//    The following code was started by pasting the code contained 
//    in newsgoup post with header:
/*
From: Martin Wartens <martin.wartens-B4qg7kJ0jjftRgLqZ5aouw@public.gmane.org>
Newsgroups: gmane.comp.programming.garbage-collection.boehmgc
Subject: Re: GC needs to be invoked manually
Date: Thu, 11 Aug 2005 20:48:46 +0000 (UTC)
Lines: 59
Approved: news@gmane.org
Message-ID: <loom.20050811T223825-747@post.gmane.org>
References: <380-2200581822464346@mymail.ch>
 */
