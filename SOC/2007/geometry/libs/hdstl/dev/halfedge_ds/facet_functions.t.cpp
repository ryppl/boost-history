//facet_selectors.hpp   -*- C++ -*-
//
//@OVERVIEW:  The components under test are the stored_facet and facet_gen 
//classes.  We must make sure that all selectors are suitably defined and 
//that the selection is done properly.
//
//@TEST_PLAN: First create an instance of all the selectors (to make sure they
// exist, and verify that there are no more than tested).  Then instantiate the
// 'facet_gen' for all selector values, with base facet classes and without 
// base facet classes and verify that each combination compiles and works.

#include <boost/hdstl/halfedge_ds/facet_selectors.hpp>
#include <boost/hdstl/halfedge_ds/facet_functions.hpp>

#include <boost/test/minimal.hpp>

#include <set>
#include <string>
#include <vector>

using namespace boost::hdstl;
using namespace std;

// ===========================================================================
//                              SELECTION CLASSES
// ===========================================================================

template <typename FacetS>
bool selection_requirements(FacetS const&) {
    return false;
}

bool selection_requirements(noFacetS const&) {
    return true;
}

template <typename Selector>
bool selection_requirements(facetS<Selector, false> const&) {
    return true;
}

template <typename Selector>
bool selection_requirements(facetS<Selector, true> const&) {
    return true;
}

// ===========================================================================
//                              CLASS FACET_GEN
// ===========================================================================

template<typename FacetGen>
bool facet_gen_requirements_void_noFacetLink() {
    
    // Types must exist.
    typedef typename FacetGen::facet_selector   facet_selector;
    typedef typename FacetGen::facet_descriptor facet_descriptor;
    typedef typename FacetGen::facet_iterator   facet_iterator;
    typedef typename FacetGen::facet_type       facet_type;
    typedef typename FacetGen::container_type   container_type;

    facet_type fa;
    facet_type fb;
    facet_type fc;
    facet_type fd;
    facet_type array[] = { fa, fb, fc, fd };  
    
    // Construct a facet_gen object whose container contains array.
    // Verify that facets_begin(), facets_end(), and num_facets() work.
    // Access the facets and, if the has_facet_links is set, check that the
    // halfedge() works.
    container_type temp_con(array,array+4);
    FacetGen facetGen;
    facetGen.m_container = temp_con;
    BOOST_CHECK(( num_facets(facetGen) == 4 ));
    
    facet_descriptor fn = new_facet(facetGen);
    (void) fn;
    BOOST_CHECK(( num_facets(facetGen) == 5 ));
    return true;
}

template <typename FacetGen>
bool facet_gen_requirements_void() {
    
    // Types must exist.
    typedef typename FacetGen::facet_selector   facet_selector;
    typedef typename FacetGen::facet_descriptor facet_descriptor;
    typedef typename FacetGen::facet_iterator   facet_iterator;
    typedef typename FacetGen::facet_type       facet_type;
    typedef typename FacetGen::container_type   container_type;

    facet_type fa(1);
    facet_type fb(2);
    facet_type fc(3);
    facet_type fd(4);
    facet_type array[] = { fa, fb, fc, fd };  
    
    // Construct a facet_gen object whose container contains array.
    // Verify that facets_begin(), facets_end(), and num_facets() work.
    // Access the facets and, if the has_facet_links is set, check that the
    // halfedge() works.
    container_type temp_con(array,array+4);
    FacetGen facetGen;
    facetGen.m_container = temp_con;
    BOOST_CHECK(( num_facets(facetGen) == 4 ));
    BOOST_CHECK(( facets_begin(facetGen)->m_facetLink == 1 ));
    BOOST_CHECK(( (--facets_end(facetGen))->m_facetLink == 4 ));

    BOOST_CHECK(( halfedge(*facets_begin(facetGen), facetGen) == 1 ));
    BOOST_CHECK(( halfedge(*--facets_end(facetGen), facetGen) == 4 ));

    facet_descriptor fn = new_facet(facetGen);
    (void) fn;
    BOOST_CHECK(( num_facets(facetGen) == 5 ));
    

    return true;
}

template <typename FacetGen, typename Base>
bool facet_gen_requirements_noFacetLink() {
    
    typedef typename FacetGen::facet_selector   facet_selector;
    typedef typename FacetGen::facet_descriptor facet_descriptor;
    typedef typename FacetGen::facet_iterator   facet_iterator;
    typedef typename FacetGen::facet_type       facet_type;
    typedef typename FacetGen::container_type   container_type;

    facet_type fa(Base(1));
    facet_type fb(Base(2));
    facet_type fc(Base(3));
    facet_type fd(Base(4));
    facet_type array[] = { fa, fb, fc, fd };  (void) array;
    
    // Same checks as before:
    container_type temp_con(array,array+4);
    FacetGen facetGen;
    facetGen.m_container = temp_con;
    BOOST_CHECK(( num_facets(facetGen) == 4 ));

    // Plus: get the base back from the facets and making sure it matches.
    BOOST_CHECK(( facets_begin(facetGen)->base() == 1 ));
    BOOST_CHECK(( (--facets_end(facetGen))->base() == 4 ));

    facet_descriptor fn = new_facet(facetGen);
    (void) fn;
    BOOST_CHECK(( num_facets(facetGen) == 5 ));

    return true;
}

template <typename FacetGen, typename Base>
bool facet_gen_requirements() {
    
    typedef typename FacetGen::facet_selector   facet_selector;
    typedef typename FacetGen::facet_descriptor facet_descriptor;
    typedef typename FacetGen::facet_iterator   facet_iterator;
    typedef typename FacetGen::facet_type       facet_type;
    typedef typename FacetGen::container_type   container_type;

    facet_type fa(1, Base(1));
    facet_type fb(2, Base(2));
    facet_type fc(3, Base(3));
    facet_type fd(4, Base(4));
    facet_type array[] = { fa, fb, fc, fd };  (void) array;
    
    // Same checks as before:
    container_type temp_con(array,array+4);
    FacetGen facetGen;
    facetGen.m_container = temp_con;
    BOOST_CHECK(( num_facets(facetGen) == 4 ));

    BOOST_CHECK(( facets_begin(facetGen)->m_facetLink == 1 ));
    BOOST_CHECK(( (--facets_end(facetGen))->m_facetLink == 4 ));
    
    BOOST_CHECK(( halfedge(*facets_begin(facetGen), facetGen) == 1 ));
    BOOST_CHECK(( halfedge(*--facets_end(facetGen), facetGen) == 4 ));
    
    // Plus: get the base back from the facets and making sure it matches.
    BOOST_CHECK(( facets_begin(facetGen)->base() == 1 ));
    BOOST_CHECK(( (--facets_end(facetGen))->base() == 4 ));

    facet_descriptor fn = new_facet(facetGen);
    (void) fn;
    BOOST_CHECK(( num_facets(facetGen) == 5 ));
    return true;
}

// ===========================================================================
//                              USAGE EXAMPLE
// ===========================================================================

// The usage example is the component itself.

// ===========================================================================
//                              BOOST TEST APPARATUS
// ===========================================================================

int test_main(int, char **)
{
    BOOST_CHECK(( selection_requirements(noFacetS()) ));
    BOOST_CHECK(( selection_requirements(facetS<listS,true>()) ));
    BOOST_CHECK(( selection_requirements(facetS<listS,false>()) ));
    
    BOOST_CHECK(( facet_gen_requirements_void_noFacetLink<
                              facet_gen<facetS<listS,false>, int, void> >() ));
    BOOST_CHECK(( facet_gen_requirements_void<
                              facet_gen<facetS<listS,true>, int, void> >() ));

    BOOST_CHECK(( facet_gen_requirements_noFacetLink<
                          facet_gen<facetS<listS,false>, int, int>, int >() ));
    BOOST_CHECK(( facet_gen_requirements<
                            facet_gen<facetS<listS,true>, int, int>, int >() ));
   
    // BOOST_CHECK(( facet_gen_requirements<
    //           facet_gen<facetS<listS,false>, int, custom_facet_base> >() ));

    // BOOST_CHECK(( facet_gen_requirements<
    //           facet_gen<facetS<listS,true>, int, custom_facet_base> >() ));
    
    BOOST_CHECK(( selection_requirements(facetS<vecS,true>()) ));
    BOOST_CHECK(( selection_requirements(facetS<vecS,false>()) ));
    
    BOOST_CHECK(( facet_gen_requirements_void_noFacetLink<
                              facet_gen<facetS<vecS,false>, int, void> >() ));
    BOOST_CHECK(( facet_gen_requirements_void<
                              facet_gen<facetS<vecS,true>, int, void> >() ));

    BOOST_CHECK(( facet_gen_requirements_noFacetLink<
                          facet_gen<facetS<vecS,false>, int, int>, int >() ));
    BOOST_CHECK(( facet_gen_requirements<
                          facet_gen<facetS<vecS,true>, int, int>, int >() ));

    // BOOST_CHECK(( facet_gen_requirements<
    //            facet_gen<facetS<vecS,false>, int, custom_facet_base> >() ));
    // BOOST_CHECK(( facet_gen_requirements<
    //             facet_gen<facetS<vecS,true>, int, custom_facet_base> >() ));
    //BOOST_CHECK(( usageExample() ));
    
    return 0;
}

