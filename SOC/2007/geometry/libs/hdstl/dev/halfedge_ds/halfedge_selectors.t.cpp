//halfedge_selectors.t.cpp   -*- C++ -*-
//
//@OVERVIEW:  The component under test is a selector class.  We
// must make sure that all selectors are suitably defined and that the
// selection is done properly.
//
//@TEST_PLAN: First create an instance of all the selectors (to make sure they
// exist, and verify that there are no more than tested).  Then instantiate the
// 'container_gen<HalfedgeS,ValueType>' for a given value type and all
// selectors, and verify that its members have the expected types and
// signatures.  Finally, verify that the usage example compiles and executes
// without errors, when assert is replaced by BOOST_CHECK.

#include <boost/hdstl/halfedge_ds/halfedge_selectors.hpp>

#include <boost/hdstl/halfedge_ds/facet_selectors.hpp>
#include <boost/hdstl/halfedge_ds/halfedge_functions.hpp>
#include <boost/hdstl/halfedge_ds/vertex_selectors.hpp>
#include <boost/hdstl/halfedge_ds/meta_functions.hpp>

#include <boost/test/minimal.hpp>

#include <set>
#include <string>
#include <vector>

using namespace boost::hdstl;
using namespace std;

// ===========================================================================
//                              SELECTION CLASSES
// ===========================================================================

template <typename HalfedgeS>
bool selection_requirements(HalfedgeS const&) {
    return false;
}

template <typename Selector, typename ForwardCategory>
bool selection_requirements(halfedgeS<Selector, 
                            forwardS<ForwardCategory> > const&) {
    return true;
}

template <typename Selector, typename BackwardCategory>
bool selection_requirements(halfedgeS<Selector, 
                            backwardS<BackwardCategory> > const&) {
    return true;
}

template <typename Selector, typename ForwardCategory,
                             typename BackwardCategory>
bool selection_requirements(halfedgeS<Selector, 
                            bidirS<ForwardCategory, 
                            BackwardCategory> > const&) {
    return true;
}

// ===========================================================================
//                              CLASS HALFEDGE_GEN
// ===========================================================================

template <typename HalfedgeS, typename VertexS, typename FacetS>
struct halfedge_config {
    // This halfedge_config to identically replace the halfedge_ds_gen::config
    // and reproduced here for test purposes only. Note that this tests are 
    // not for the config class, but the halfedge_selectors, so config will
    // be tested in its own package.

    enum {
        halfedge_has_opposite_member = !meta_is_same<
                        typename HalfedgeS::container_selector, vecS>::value,
        is_forward = HalfedgeS::is_forward,
        is_backward = HalfedgeS::is_backward,
        halfedge_supports_vertices = !meta_is_same<VertexS,noVertexS>::value,
        is_source = VertexS::is_source,
        halfedge_supports_facets = !meta_is_same<FacetS,noFacetS>::value
    };
};

template <typename HalfedgeS>
struct halfedge_config<HalfedgeS, noVertexS, noFacetS> {
    enum {
        halfedge_has_opposite_member = !meta_is_same<
                        typename HalfedgeS::container_selector, vecS>::value,
        is_forward = HalfedgeS::is_forward,
        is_backward = HalfedgeS::is_backward,
        halfedge_supports_vertices = false,
        is_source = false,
        halfedge_supports_facets = false
    };
};

template <typename HalfedgeS, typename FacetS>
struct halfedge_config<HalfedgeS, noVertexS, FacetS> {
    enum {
        halfedge_has_opposite_member = !meta_is_same<
                        typename HalfedgeS::container_selector, vecS>::value,
        is_forward = HalfedgeS::is_forward,
        is_backward = HalfedgeS::is_backward,
        halfedge_supports_vertices = false,
        is_source = false,
        halfedge_supports_facets = !meta_is_same<FacetS,noFacetS>::value
    };
};

template <typename HalfedgeS, typename VertexS>
struct halfedge_config<HalfedgeS, VertexS, noFacetS> {
    enum {
        halfedge_has_opposite_member = !meta_is_same<
                        typename HalfedgeS::container_selector, vecS>::value,
        is_forward = HalfedgeS::is_forward,
        is_backward = HalfedgeS::is_backward,
        halfedge_supports_vertices = !meta_is_same<VertexS,noVertexS>::value,
        is_source = VertexS::is_source,
        halfedge_supports_facets = false
    };
};


template <typename HalfedgeGen>
bool halfedge_gen_requirements_void() {
    
    // Types must exist.
    typedef typename HalfedgeGen::halfedge_selector   halfedge_selector;
    typedef typename HalfedgeGen::halfedge_descriptor halfedge_descriptor;
    typedef typename HalfedgeGen::halfedge_iterator   halfedge_iterator;
    typedef typename HalfedgeGen::halfedge_type       halfedge_type;
    typedef typename HalfedgeGen::container_type      container_type;

    halfedge_type fa;
    halfedge_type fb;
    halfedge_type fc;
    halfedge_type fd;
//  fa.m_opposite = 1;
//  fb.m_opposite = 2;
//  fc.m_opposite = 3;
//  fd.m_opposite = 4;
    halfedge_type array[] = { fa, fb, fc, fd };  
    
    // Construct a halfedge_gen object whose container contains array.  Verify
    // that halfedges_begin(), halfedges_end(), and num_halfedges() work.

    container_type halfedges(array, array+4);
    HalfedgeGen halfedgeGen;
    halfedgeGen.m_container = halfedges;

    return true;
}

template <typename HalfedgeGen, typename Base>
bool halfedge_gen_requirements() {
    
    typedef typename HalfedgeGen::halfedge_selector   halfedge_selector;
    typedef typename HalfedgeGen::halfedge_descriptor halfedge_descriptor;
    typedef typename HalfedgeGen::halfedge_iterator   halfedge_iterator;
    typedef typename HalfedgeGen::halfedge_type       halfedge_type;
    typedef typename HalfedgeGen::container_type   container_type;

    halfedge_type fa(1, Base(1));
    halfedge_type fb(2, Base(2));
    halfedge_type fc(3, Base(3));
    halfedge_type fd(4, Base(4));
    halfedge_type array[] = { fa, fb, fc, fd };  (void) array;
    
    // Same checks as before:
    // BOOST_CHECK(( halfedge_gen_requirements_void<HalfedgeGen>() ));
    container_type temp_con(array,array+4);
    HalfedgeGen halfedgeGen;
    halfedgeGen.m_container = temp_con;
    BOOST_CHECK(( halfedgeGen.m_container.size() == 4 ));

    BOOST_CHECK(( HalfedgeGen::value(*halfedgeGen.m_container.begin(), 
                                     halfedgeGen)->m_halfedgeLink == 1 ));
    BOOST_CHECK(( HalfedgeGen::value(*(--(halfedgeGen.m_container.begin())),
                                     halfedgeGen)->m_halfedgeLink == 4 ));


    return true;
}

// ===========================================================================
//                              USAGE EXAMPLE
// ===========================================================================

// The usage example is the component itself.

// ===========================================================================
//                              BOOST TEST APPARATUS
// ===========================================================================

template <typename ContainerS>
bool test_container_selector()
{
    BOOST_CHECK(( selection_requirements(halfedgeS<ContainerS,
                                            forwardS<next_in_facet_tag> >()) ));
    BOOST_CHECK(( selection_requirements(halfedgeS<ContainerS,
                                           forwardS<next_at_source_tag> >()) ));
    BOOST_CHECK(( selection_requirements(halfedgeS<ContainerS,
                                           forwardS<next_at_target_tag> >()) ));
    
    BOOST_CHECK(( selection_requirements(halfedgeS<ContainerS,
                                           backwardS<prev_in_facet_tag> >()) ));
    BOOST_CHECK(( selection_requirements(halfedgeS<ContainerS,
                                          backwardS<prev_at_source_tag> >()) ));
    BOOST_CHECK(( selection_requirements(halfedgeS<ContainerS,
                                          backwardS<prev_at_target_tag> >()) ));
    
    BOOST_CHECK(( selection_requirements(halfedgeS<ContainerS,
                           bidirS<next_in_facet_tag, prev_in_facet_tag> >()) ));
    BOOST_CHECK(( selection_requirements(halfedgeS<ContainerS,
                         bidirS<next_at_source_tag, prev_at_source_tag> >()) ));
    BOOST_CHECK(( selection_requirements(halfedgeS<ContainerS,
                         bidirS<next_at_target_tag, prev_at_target_tag> >()) ));

    // Check 'forwardS'.
    BOOST_CHECK(( halfedge_gen_requirements_void<
                    halfedge_gen<
                      halfedgeS<ContainerS, forwardS<next_in_facet_tag> >, 
                        int, int, int, 
                        halfedge_config<
                           halfedgeS<ContainerS, forwardS<next_in_facet_tag> >, 
                        noVertexS, noFacetS> > 
                  >() ));
    BOOST_CHECK(( halfedge_gen_requirements_void<
                  halfedge_gen<
                  halfedgeS<ContainerS, forwardS<next_at_source_tag> >, 
                  int, int, int, 
                  halfedge_config<
                  halfedgeS<ContainerS, forwardS<next_at_source_tag> >, 
                  noVertexS, noFacetS> > 
                  >() ));
    BOOST_CHECK(( halfedge_gen_requirements_void<
                  halfedge_gen<
                  halfedgeS<ContainerS, forwardS<next_at_target_tag> >, 
                  int, int, int, 
                  halfedge_config<
                  halfedgeS<ContainerS, forwardS<next_at_target_tag> >, 
                  noVertexS, noFacetS> > 
                  >() ));
                    
    // Check 'backwardS'.
    BOOST_CHECK(( halfedge_gen_requirements_void<
                  halfedge_gen<
                  halfedgeS<ContainerS, backwardS<prev_in_facet_tag> >, 
                  int, int, int, 
                  halfedge_config<
                  halfedgeS<ContainerS, backwardS<prev_in_facet_tag> >, 
                  noVertexS, noFacetS> > 
                  >() ));
    BOOST_CHECK(( halfedge_gen_requirements_void<
                  halfedge_gen<
                  halfedgeS<ContainerS, backwardS<prev_at_source_tag> >, 
                  int, int, int, 
                  halfedge_config<
                  halfedgeS<ContainerS, backwardS<prev_at_source_tag> >, 
                  noVertexS, noFacetS> > 
                  >() ));
    BOOST_CHECK(( halfedge_gen_requirements_void<
                  halfedge_gen<
                  halfedgeS<ContainerS, backwardS<prev_at_target_tag> >, 
                  int, int, int, 
                  halfedge_config<
                  halfedgeS<ContainerS, backwardS<prev_at_target_tag> >, 
                  noVertexS, noFacetS> > 
                  >() ));
                    
    // Check 'bidirS'.
    BOOST_CHECK(( halfedge_gen_requirements_void<
                    halfedge_gen<
                      halfedgeS<ContainerS, bidirS<next_in_facet_tag,
                                              prev_in_facet_tag> >, 
                      int, int, int, 
                      halfedge_config<
                        halfedgeS<ContainerS, bidirS<next_in_facet_tag,
                                                prev_in_facet_tag> >, 
                      noVertexS, noFacetS> > 
                  >() ));
    BOOST_CHECK(( halfedge_gen_requirements_void<
                  halfedge_gen<
                    halfedgeS<ContainerS, bidirS<next_at_source_tag,
                                            prev_at_source_tag> >, 
                    int, int, int, 
                    halfedge_config<
                    halfedgeS<ContainerS, bidirS<next_at_source_tag,
                                            prev_at_source_tag> >, 
                    noVertexS, noFacetS> > 
                  >() ));
    BOOST_CHECK(( halfedge_gen_requirements_void<
                  halfedge_gen<
                  halfedgeS<ContainerS, bidirS<next_at_target_tag,
                                          prev_at_target_tag> >, 
                  int, int, int, 
                  halfedge_config<
                  halfedgeS<ContainerS, bidirS<next_at_target_tag,
                                          prev_at_target_tag> >, 
                  noVertexS, noFacetS> > 
                  >() ));
                    
    // Check 'vertexS'.
    BOOST_CHECK(( halfedge_gen_requirements_void<
                  halfedge_gen<
                  halfedgeS<ContainerS, forwardS<next_in_facet_tag> >, 
                  int, int, int, 
                  halfedge_config<
                  halfedgeS<ContainerS, forwardS<next_in_facet_tag> >, 
                  vertexS<ContainerS,false,sourceS>, noFacetS> > 
                  >() ));
    BOOST_CHECK(( halfedge_gen_requirements_void<
                  halfedge_gen<
                  halfedgeS<ContainerS, forwardS<next_in_facet_tag> >, 
                  int, int, int, 
                  halfedge_config<
                  halfedgeS<ContainerS, forwardS<next_in_facet_tag> >, 
                  vertexS<ContainerS,false,targetS>, noFacetS> > 
                  >() ));
    BOOST_CHECK(( halfedge_gen_requirements_void<
                  halfedge_gen<
                  halfedgeS<ContainerS, forwardS<next_in_facet_tag> >, 
                  int, int, int, 
                  halfedge_config<
                  halfedgeS<ContainerS, forwardS<next_in_facet_tag> >, 
                  vertexS<ContainerS,true,sourceS>, noFacetS> > 
                  >() ));
    BOOST_CHECK(( halfedge_gen_requirements_void<
                  halfedge_gen<
                  halfedgeS<ContainerS, forwardS<next_in_facet_tag> >, 
                  int, int, int, 
                  halfedge_config<
                  halfedgeS<ContainerS, forwardS<next_in_facet_tag> >, 
                  vertexS<ContainerS,true,targetS>, noFacetS> > 
                  >() ));
    
    // Check 'facetS'.
    BOOST_CHECK(( halfedge_gen_requirements_void<
                  halfedge_gen<
                  halfedgeS<ContainerS, forwardS<next_in_facet_tag> >, 
                  int, int, int, 
                  halfedge_config<
                  halfedgeS<ContainerS, forwardS<next_in_facet_tag> >, 
                  noVertexS, facetS<ContainerS,true> > > 
                  >() ));
    BOOST_CHECK(( halfedge_gen_requirements_void<
                  halfedge_gen<
                  halfedgeS<ContainerS, forwardS<next_in_facet_tag> >, 
                  int, int, int, 
                  halfedge_config<
                  halfedgeS<ContainerS, forwardS<next_in_facet_tag> >, 
                  noVertexS, facetS<ContainerS,false> > > 
                  >() ));

    return true;
}

int test_main(int, char**)
{
    BOOST_CHECK(( test_container_selector<listS>() ));
    BOOST_CHECK(( test_container_selector<vecS>() ));
    // BOOST_CHECK(( test_container_selector() ));
    return 0;
}
