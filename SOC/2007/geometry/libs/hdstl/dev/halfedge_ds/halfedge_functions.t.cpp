//halfedge_functions.t.cpp   -*- C++ -*-
//
//@OVERVIEW:  The component under test is a set of function templates.  We
// must make sure that all templates are suitably defined and that the
// template dispatch is done properly.
//
//@TEST_PLAN: Then instantiate the 'container_gen<HalfedgeS,ValueType>' for a
//given value type and all selectors, and verify that all the functions act
//correctly with it.  Finally, verify that the usage example compiles and
//executes without errors, when assert is replaced by BOOST_CHECK.

#include <boost/hdstl/halfedge_ds/halfedge_functions.hpp>

#include <boost/hdstl/halfedge_ds/halfedge_selectors.hpp>
#include <boost/hdstl/halfedge_ds/facet_selectors.hpp>
#include <boost/hdstl/halfedge_ds/vertex_selectors.hpp>
#include <boost/hdstl/halfedge_ds/meta_functions.hpp>
#include <boost/pending/ct_if.hpp>

#include <boost/test/minimal.hpp>

#include <set>
#include <string>
#include <vector>

using namespace boost::hdstl;
using namespace std;

// ===========================================================================
//                              CLASS HALFEDGE_GEN
// ===========================================================================

typedef void* halfedge_ptr;

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
    typedef typename boost::ct_if<halfedge_has_opposite_member,
                  halfedge_ptr, std::size_t>::type halfedge_descriptor;
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
    typedef typename boost::ct_if<halfedge_has_opposite_member,
                  halfedge_ptr, std::size_t>::type halfedge_descriptor;
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
    typedef typename boost::ct_if<halfedge_has_opposite_member,
                  halfedge_ptr, std::size_t>::type halfedge_descriptor;
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
    typedef typename boost::ct_if<halfedge_has_opposite_member,
                  halfedge_ptr, std::size_t>::type halfedge_descriptor;
};

template <typename HalfedgeType, typename HalfedgeDescriptor, typename ContainerS>
void set_opposite_helper(HalfedgeType& h, HalfedgeDescriptor g, ContainerS const&) {
    h.m_opposite = g;
}

template <typename HalfedgeType, typename HalfedgeDescriptor>
void set_opposite_helper(HalfedgeType&, HalfedgeDescriptor, vecS const&) {
    // nothing to do
}

template <typename HalfedgeGen>
bool halfedge_functions_requirements() {
    
    // Types must exist.
    typedef typename HalfedgeGen::halfedge_selector      halfedge_selector;
    typedef typename HalfedgeGen::halfedge_descriptor    halfedge_descriptor;
    typedef typename HalfedgeGen::halfedge_iterator      halfedge_iterator;
    typedef typename HalfedgeGen::halfedge_type          halfedge_type;
    typedef typename HalfedgeGen::ContainerGen           container_gen;
    typedef typename HalfedgeGen::ContainerGen::selector container_selector;
    typedef typename HalfedgeGen::container_type         container_type;

    halfedge_type fa;
    halfedge_type fb;
    halfedge_type fc;
    halfedge_type fd;
    halfedge_type array[] = { fa, fb, fc, fd };  
    
    // Construct a halfedge_gen object whose container contains array.  Verify
    // that halfedges_begin(), halfedges_end(), and num_halfedges() work.

    container_type halfedges(array, array+4);
    HalfedgeGen halfedgeGen;
    halfedgeGen.m_container = halfedges;

    BOOST_CHECK(( num_halfedges(halfedgeGen) == 4 ));

    BOOST_CHECK(( halfedges_begin(halfedgeGen) 
                    == container_gen::container_begin(halfedgeGen.m_container)
               ));
    BOOST_CHECK(( halfedges_end(halfedgeGen)
                    == container_gen::container_end(halfedgeGen.m_container)
               ));
    
    // Set up opposites correctly (nothing to do if 'vecS', let the helper
    // decide.  Then check that opposite() returns the correct value.
    { 
        typename container_type::iterator begin = halfedgeGen.m_container.begin();
        halfedge_type& ha = *begin;
        halfedge_type& hb = *(++begin);
        halfedge_type& hc = *(++begin);
        halfedge_type& hd = *(++begin);
        set_opposite_helper(ha, &hb, container_selector());
        set_opposite_helper(hb, &ha, container_selector());
        set_opposite_helper(hc, &hd, container_selector());
        set_opposite_helper(hd, &hc, container_selector());
    }

    halfedge_iterator begin = halfedges_begin(halfedgeGen);
    halfedge_descriptor ha = *begin;
    halfedge_descriptor hb = *(++begin);
    halfedge_descriptor hc = *(++begin);
    halfedge_descriptor hd = *(++begin);
    BOOST_CHECK(( opposite(ha, halfedgeGen) == hb ));
    BOOST_CHECK(( opposite(hb, halfedgeGen) == ha ));
    BOOST_CHECK(( opposite(hc, halfedgeGen) == hd ));
    BOOST_CHECK(( opposite(hd, halfedgeGen) == hc ));
    
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
    // Check 'forwardS'.
    BOOST_CHECK(( halfedge_functions_requirements<
                    halfedge_gen<
                      halfedgeS<ContainerS, forwardS<next_in_facet_tag> >, 
                      int, int, 
                      halfedge_config<
                        halfedgeS<ContainerS, forwardS<next_in_facet_tag> >, 
                        noVertexS, noFacetS> > 
                  >() ));
    BOOST_CHECK(( halfedge_functions_requirements<
                    halfedge_gen<
                      halfedgeS<ContainerS, forwardS<next_at_source_tag> >, 
                      int, int, 
                      halfedge_config<
                        halfedgeS<ContainerS, forwardS<next_at_source_tag> >, 
                        noVertexS, noFacetS> > 
                >() ));
    BOOST_CHECK(( halfedge_functions_requirements<
                    halfedge_gen<
                      halfedgeS<ContainerS, forwardS<next_at_target_tag> >, 
                      int, int, 
                      halfedge_config<
                        halfedgeS<ContainerS, forwardS<next_at_target_tag> >, 
                        noVertexS, noFacetS> > 
                  >() ));
                    
    // Check 'backwardS'.
    BOOST_CHECK(( halfedge_functions_requirements<
                    halfedge_gen<
                      halfedgeS<ContainerS, backwardS<prev_in_facet_tag> >, 
                      int, int, 
                      halfedge_config<
                        halfedgeS<ContainerS, backwardS<prev_in_facet_tag> >, 
                        noVertexS, noFacetS> > 
                  >() ));
    BOOST_CHECK(( halfedge_functions_requirements<
                    halfedge_gen<
                      halfedgeS<ContainerS, backwardS<prev_at_source_tag> >, 
                      int, int, 
                      halfedge_config<
                        halfedgeS<ContainerS, backwardS<prev_at_source_tag> >, 
                        noVertexS, noFacetS> > 
                  >() ));
    BOOST_CHECK(( halfedge_functions_requirements<
                    halfedge_gen<
                      halfedgeS<ContainerS, backwardS<prev_at_target_tag> >, 
                      int, int, 
                      halfedge_config<
                        halfedgeS<ContainerS, backwardS<prev_at_target_tag> >, 
                        noVertexS, noFacetS> > 
                  >() ));
                    
    // Check 'bidirS'.
    BOOST_CHECK(( halfedge_functions_requirements<
                    halfedge_gen<
                      halfedgeS<ContainerS, bidirS<next_in_facet_tag,
                                                   prev_in_facet_tag> >, 
                      int, int, 
                      halfedge_config<
                        halfedgeS<ContainerS, bidirS<next_in_facet_tag,
                                                     prev_in_facet_tag> >, 
                        noVertexS, noFacetS> > 
                  >() ));
    BOOST_CHECK(( halfedge_functions_requirements<
                    halfedge_gen<
                      halfedgeS<ContainerS, bidirS<next_at_source_tag,
                                                   prev_at_source_tag> >, 
                      int, int, 
                      halfedge_config<
                        halfedgeS<ContainerS, bidirS<next_at_source_tag,
                                                       prev_at_source_tag> >, 
                        noVertexS, noFacetS> > 
                  >() ));
    BOOST_CHECK(( halfedge_functions_requirements<
                    halfedge_gen<
                      halfedgeS<ContainerS, bidirS<next_at_target_tag,
                                                   prev_at_target_tag> >, 
                      int, int, 
                      halfedge_config<
                        halfedgeS<ContainerS, bidirS<next_at_target_tag,
                                                     prev_at_target_tag> >, 
                        noVertexS, noFacetS> > 
                  >() ));
                    
    // Check 'vertexS'.
    BOOST_CHECK(( halfedge_functions_requirements<
                    halfedge_gen<
                      halfedgeS<ContainerS, forwardS<next_in_facet_tag> >, 
                      int, int, 
                      halfedge_config<
                        halfedgeS<ContainerS, forwardS<next_in_facet_tag> >, 
                        vertexS<ContainerS,false,sourceS>, noFacetS> > 
                  >() ));
    BOOST_CHECK(( halfedge_functions_requirements<
                    halfedge_gen<
                      halfedgeS<ContainerS, forwardS<next_in_facet_tag> >, 
                      int, int, 
                      halfedge_config<
                        halfedgeS<ContainerS, forwardS<next_in_facet_tag> >, 
                        vertexS<ContainerS,false,targetS>, noFacetS> > 
                  >() ));
    BOOST_CHECK(( halfedge_functions_requirements<
                    halfedge_gen<
                      halfedgeS<ContainerS, forwardS<next_in_facet_tag> >, 
                      int, int, 
                      halfedge_config<
                        halfedgeS<ContainerS, forwardS<next_in_facet_tag> >, 
                        vertexS<ContainerS,true,sourceS>, noFacetS> > 
                  >() ));
    BOOST_CHECK(( halfedge_functions_requirements<
                    halfedge_gen<
                      halfedgeS<ContainerS, forwardS<next_in_facet_tag> >, 
                      int, int, 
                      halfedge_config<
                        halfedgeS<ContainerS, forwardS<next_in_facet_tag> >, 
                        vertexS<ContainerS,true,targetS>, noFacetS> > 
                  >() ));
    
    // Check 'facetS'.
    BOOST_CHECK(( halfedge_functions_requirements<
                    halfedge_gen<
                      halfedgeS<ContainerS, forwardS<next_in_facet_tag> >, 
                      int, int, 
                      halfedge_config<
                        halfedgeS<ContainerS, forwardS<next_in_facet_tag> >, 
                        noVertexS, facetS<ContainerS,true> > > 
                  >() ));
    BOOST_CHECK(( halfedge_functions_requirements<
                    halfedge_gen<
                      halfedgeS<ContainerS, forwardS<next_in_facet_tag> >, 
                      int, int, 
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
