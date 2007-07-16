//facet_selectors.hpp   -*- C++ -*-
//
//@OVERVIEW:  The component under test is a selector class.  We
// must make sure that all selectors are suitably defined and that the
// selection is done properly.
//
//@TEST_PLAN: First create an instance of all the selectors (to make sure they
// exist, and verify that there are no more than tested).  Then instantiate the
// 'container_gen<FacetS,ValueType>' for a given value type and all
// selectors, and verify that its members have the expected types and
// signatures.  Finally, verify that the usage example compiles and executes
// without errors, when assert is replaced by BOOST_CHECK.

#include <boost/hdstl/halfedge_ds/facet_selectors.hpp>

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

template <typename Base>
bool selection_requirements(facetS<Base, false> const&) {
    return true;
}

template <typename Base>
bool selection_requirements(facetS<Base, true> const&) {
    return true;
}

// ===========================================================================
//                              CLASS FACET_GEN
// ===========================================================================

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
    facet_type array[] = { fa, fb, fc, fd };  (void) array;
    
    // Contruct a facet_gen object whose container contains array.
    // Verify that halfedges_begin(), halfedges_end(), and num_facets() work.
    // Access the facets and, if the has_facet_links is set, check that the
    // halfedge() works.
    // ... TODO

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
    BOOST_CHECK(( facet_gen_requirements_void<FacetGen>() ));

    // Plus: get the base back from the facets and making sure it matches.
    // ... TODO

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
    BOOST_CHECK(( facet_gen_requirements_void<facet_gen<facetS<listS,false>, int, void> >() ));
    BOOST_CHECK(( facet_gen_requirements<facet_gen<facetS<listS,false>, int, int>, int >() ));
    // BOOST_CHECK(( facet_gen_requirements<facet_gen<facetS<listS,false>, int, custom_facet_base> >() ));
    BOOST_CHECK(( facet_gen_requirements_void<facet_gen<facetS<listS,true>, int, void> >() ));
    BOOST_CHECK(( facet_gen_requirements<facet_gen<facetS<listS,true>, int, int>, int >() ));
    // BOOST_CHECK(( facet_gen_requirements<facet_gen<facetS<listS,true>, int, custom_facet_base> >() ));
    
    BOOST_CHECK(( selection_requirements(facetS<vecS,true>()) ));
    BOOST_CHECK(( selection_requirements(facetS<vecS,false>()) ));
    BOOST_CHECK(( facet_gen_requirements_void<facet_gen<facetS<vecS,false>, int, void> >() ));
    BOOST_CHECK(( facet_gen_requirements<facet_gen<facetS<vecS,false>, int, int>, int >() ));
    // BOOST_CHECK(( facet_gen_requirements<facet_gen<facetS<vecS,false>, int, custom_facet_base> >() ));
    BOOST_CHECK(( facet_gen_requirements_void<facet_gen<facetS<vecS,true>, int, void> >() ));
    BOOST_CHECK(( facet_gen_requirements<facet_gen<facetS<vecS,true>, int, int>, int >() ));
    // BOOST_CHECK(( facet_gen_requirements<facet_gen<facetS<vecS,true>, int, custom_facet_base> >() ));
   
    //BOOST_CHECK(( usageExample() ));
    
    return 0;
}

