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

template <typename HalfedgeGen, typename HalfedgeDescriptor, typename ContainerS>
void set_next_helper(HalfedgeGen& H, HalfedgeDescriptor h, HalfedgeDescriptor g, ContainerS const&) {
    (void) H;
    h->m_next = g;
}

template <typename HalfedgeGen,typename HalfedgeDescriptor>
void set_next_helper(HalfedgeGen& H, HalfedgeDescriptor h, HalfedgeDescriptor g, vecS const&) {
    H.m_container[h].m_next = g;
}

template <typename HalfedgeGen>
bool halfedge_next_in_facet_test() {
    
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
    halfedge_type fe;
    halfedge_type ff;
    halfedge_type fg;
    halfedge_type fh;
    halfedge_type fi;
    halfedge_type fj;
    halfedge_type array[] = { fa, fb, fc, fd, fe, ff, fg, fh, fi, fj};  
    
    // Construct a halfedge_gen object whose container contains array.  Verify
    // that halfedges_begin(), halfedges_end(), and num_halfedges() work.

    container_type halfedges(array, array+10);
    HalfedgeGen halfedgeGen;
    halfedgeGen.m_container = halfedges;

    { 
        typename container_type::iterator begin = halfedgeGen.m_container.begin();
        halfedge_type& ha = *begin;
        halfedge_type& hb = *(++begin);
        halfedge_type& hc = *(++begin);
        halfedge_type& hd = *(++begin);
        halfedge_type& he = *(++begin);
        halfedge_type& hf = *(++begin);
        halfedge_type& hg = *(++begin);
        halfedge_type& hh = *(++begin);
        halfedge_type& hi = *(++begin);
        halfedge_type& hj = *(++begin);
        set_opposite_helper(ha, &hb, container_selector());
        set_opposite_helper(hb, &ha, container_selector());
        set_opposite_helper(hc, &hd, container_selector());
        set_opposite_helper(hd, &hc, container_selector());
        set_opposite_helper(he, &hf, container_selector());
        set_opposite_helper(hf, &he, container_selector());
        set_opposite_helper(hg, &hh, container_selector());
        set_opposite_helper(hh, &hg, container_selector());
        set_opposite_helper(hi, &hj, container_selector());
        set_opposite_helper(hj, &hi, container_selector());
    }

    halfedge_iterator begin = halfedges_begin(halfedgeGen);
    halfedge_descriptor ha = *begin;
    halfedge_descriptor hb = *(++begin);
    halfedge_descriptor hc = *(++begin);
    halfedge_descriptor hd = *(++begin);
    halfedge_descriptor he = *(++begin);
    halfedge_descriptor hf = *(++begin);
    halfedge_descriptor hg = *(++begin);
    halfedge_descriptor hh = *(++begin);
    halfedge_descriptor hi = *(++begin);
    halfedge_descriptor hj = *(++begin);
    set_next_helper(halfedgeGen, ha, hc, container_selector());
    set_next_helper(halfedgeGen, hc, he, container_selector());
    set_next_helper(halfedgeGen, hj, hd, container_selector());
    set_next_helper(halfedgeGen, hd, hg, container_selector());
    set_next_helper(halfedgeGen, hh, hb, container_selector());
    set_next_helper(halfedgeGen, hf, hi, container_selector());
    BOOST_CHECK(( next_in_facet(hc, halfedgeGen) == he));
    BOOST_CHECK(( next_at_source(hc, halfedgeGen) == hg));
    BOOST_CHECK(( next_at_target(hc, halfedgeGen) == hf));
    //BOOST_CHECK(( prev_in_facet(hc, halfedgeGen) == ha));
    //BOOST_CHECK(( prev_at_source<next_in_facet_tag>(hc, halfedgeGen) == hg));
    //BOOST_CHECK(( prev_at_target<next_in_facet_tag>(hc, halfedgeGen) == hf));
    
    return true;
}

template <typename HalfedgeGen>
bool halfedge_next_at_source_test() {
    
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
    halfedge_type fe;
    halfedge_type ff;
    halfedge_type fg;
    halfedge_type fh;
    halfedge_type fi;
    halfedge_type fj;
    halfedge_type array[] = { fa, fb, fc, fd, fe, ff, fg, fh, fi, fj};  
    
    // Construct a halfedge_gen object whose container contains array.  Verify
    // that halfedges_begin(), halfedges_end(), and num_halfedges() work.

    container_type halfedges(array, array+10);
    HalfedgeGen halfedgeGen;
    halfedgeGen.m_container = halfedges;

    { 
        typename container_type::iterator begin = halfedgeGen.m_container.begin();
        halfedge_type& ha = *begin;
        halfedge_type& hb = *(++begin);
        halfedge_type& hc = *(++begin);
        halfedge_type& hd = *(++begin);
        halfedge_type& he = *(++begin);
        halfedge_type& hf = *(++begin);
        halfedge_type& hg = *(++begin);
        halfedge_type& hh = *(++begin);
        halfedge_type& hi = *(++begin);
        halfedge_type& hj = *(++begin);
        set_opposite_helper(ha, &hb, container_selector());
        set_opposite_helper(hb, &ha, container_selector());
        set_opposite_helper(hc, &hd, container_selector());
        set_opposite_helper(hd, &hc, container_selector());
        set_opposite_helper(he, &hf, container_selector());
        set_opposite_helper(hf, &he, container_selector());
        set_opposite_helper(hg, &hh, container_selector());
        set_opposite_helper(hh, &hg, container_selector());
        set_opposite_helper(hi, &hj, container_selector());
        set_opposite_helper(hj, &hi, container_selector());
    }

    halfedge_iterator begin = halfedges_begin(halfedgeGen);
    //halfedge_descriptor ha = *begin;
    halfedge_descriptor hb = *(++begin);
    halfedge_descriptor hc = *(++begin);
    halfedge_descriptor hd = *(++begin);
    halfedge_descriptor he = *(++begin);
    halfedge_descriptor hf = *(++begin);
    halfedge_descriptor hg = *(++begin);
    //halfedge_descriptor hh = *(++begin);
    halfedge_descriptor hi = *(++begin);
    //halfedge_descriptor hj = *(++begin);
    set_next_helper(halfedgeGen, hc, hg, container_selector());
    set_next_helper(halfedgeGen, hg, hb, container_selector());
    set_next_helper(halfedgeGen, hd, he, container_selector());
    set_next_helper(halfedgeGen, hb, hc, container_selector());
    set_next_helper(halfedgeGen, he, hi, container_selector());
    set_next_helper(halfedgeGen, hi, hd, container_selector());
    BOOST_CHECK(( next_in_facet(hc, halfedgeGen) == he));
    BOOST_CHECK(( next_at_source(hc, halfedgeGen) == hg));
    BOOST_CHECK(( next_at_target(hc, halfedgeGen) == hf));
    //BOOST_CHECK(( prev_in_facet(hc, halfedgeGen) == ha));
    //BOOST_CHECK(( prev_at_source<next_in_facet_tag>(hc, halfedgeGen) == hg));
    //BOOST_CHECK(( prev_at_target<next_in_facet_tag>(hc, halfedgeGen) == hf));
    
    return true;
}

template <typename HalfedgeGen>
bool halfedge_next_at_target_test() {
    
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
    halfedge_type fe;
    halfedge_type ff;
    halfedge_type fg;
    halfedge_type fh;
    halfedge_type fi;
    halfedge_type fj;
    halfedge_type array[] = { fa, fb, fc, fd, fe, ff, fg, fh, fi, fj};  
    
    // Construct a halfedge_gen object whose container contains array.  Verify
    // that halfedges_begin(), halfedges_end(), and num_halfedges() work.

    container_type halfedges(array, array+10);
    HalfedgeGen halfedgeGen;
    halfedgeGen.m_container = halfedges;

    { 
        typename container_type::iterator begin = halfedgeGen.m_container.begin();
        halfedge_type& ha = *begin;
        halfedge_type& hb = *(++begin);
        halfedge_type& hc = *(++begin);
        halfedge_type& hd = *(++begin);
        halfedge_type& he = *(++begin);
        halfedge_type& hf = *(++begin);
        halfedge_type& hg = *(++begin);
        halfedge_type& hh = *(++begin);
        halfedge_type& hi = *(++begin);
        halfedge_type& hj = *(++begin);
        set_opposite_helper(ha, &hb, container_selector());
        set_opposite_helper(hb, &ha, container_selector());
        set_opposite_helper(hc, &hd, container_selector());
        set_opposite_helper(hd, &hc, container_selector());
        set_opposite_helper(he, &hf, container_selector());
        set_opposite_helper(hf, &he, container_selector());
        set_opposite_helper(hg, &hh, container_selector());
        set_opposite_helper(hh, &hg, container_selector());
        set_opposite_helper(hi, &hj, container_selector());
        set_opposite_helper(hj, &hi, container_selector());
    }

    halfedge_iterator begin = halfedges_begin(halfedgeGen);
    halfedge_descriptor ha = *begin;
    halfedge_descriptor hb = *(++begin);
    halfedge_descriptor hc = *(++begin);
    halfedge_descriptor hd = *(++begin);
    halfedge_descriptor he = *(++begin);
    halfedge_descriptor hf = *(++begin);
    halfedge_descriptor hg = *(++begin);
    halfedge_descriptor hh = *(++begin);
    halfedge_descriptor hi = *(++begin);
    halfedge_descriptor hj = *(++begin);
    set_next_helper(halfedgeGen, ha, hd, container_selector());
    set_next_helper(halfedgeGen, hc, hf, container_selector());
    set_next_helper(halfedgeGen, hd, hh, container_selector());
    set_next_helper(halfedgeGen, hh, ha, container_selector());
    set_next_helper(halfedgeGen, hj, hc, container_selector());
    set_next_helper(halfedgeGen, hf, hj, container_selector());
    BOOST_CHECK(( next_in_facet(hc, halfedgeGen) == he));
    BOOST_CHECK(( next_at_source(hc, halfedgeGen) == hg));
    BOOST_CHECK(( next_at_target(hc, halfedgeGen) == hf));
    (void) hb;
    (void) hi;
    //BOOST_CHECK(( prev_in_facet(hc, halfedgeGen) == ha));
    //BOOST_CHECK(( prev_at_source<next_in_facet_tag>(hc, halfedgeGen) == hg));
    //BOOST_CHECK(( prev_at_target<next_in_facet_tag>(hc, halfedgeGen) == hf));
    
    return true;
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
    halfedge_type fe;
    halfedge_type ff;
    halfedge_type fg;
    halfedge_type fh;
    halfedge_type fi;
    halfedge_type fj;
    halfedge_type array[] = { fa, fb, fc, fd, fe, ff, fg, fh, fi, fj};  
    
    // Construct a halfedge_gen object whose container contains array.  Verify
    // that halfedges_begin(), halfedges_end(), and num_halfedges() work.

    container_type halfedges(array, array+10);
    HalfedgeGen halfedgeGen;
    halfedgeGen.m_container = halfedges;

    BOOST_CHECK(( num_halfedges(halfedgeGen) == 10 ));

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
        halfedge_type& he = *(++begin);
        halfedge_type& hf = *(++begin);
        halfedge_type& hg = *(++begin);
        halfedge_type& hh = *(++begin);
        halfedge_type& hi = *(++begin);
        halfedge_type& hj = *(++begin);
        set_opposite_helper(ha, &hb, container_selector());
        set_opposite_helper(hb, &ha, container_selector());
        set_opposite_helper(hc, &hd, container_selector());
        set_opposite_helper(hd, &hc, container_selector());
        set_opposite_helper(he, &hf, container_selector());
        set_opposite_helper(hf, &he, container_selector());
        set_opposite_helper(hg, &hh, container_selector());
        set_opposite_helper(hh, &hg, container_selector());
        set_opposite_helper(hi, &hj, container_selector());
        set_opposite_helper(hj, &hi, container_selector());
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

    // Check next_... functions 
    BOOST_CHECK(( halfedge_next_in_facet_test<
                    halfedge_gen<
                      halfedgeS<ContainerS, forwardS<next_in_facet_tag> >, 
                      int, int, 
                      halfedge_config<
                        halfedgeS<ContainerS, forwardS<next_in_facet_tag> >, 
                        noVertexS, noFacetS> > 
                  >() ));
    BOOST_CHECK(( halfedge_next_at_source_test<
                    halfedge_gen<
                      halfedgeS<ContainerS, forwardS<next_at_source_tag> >, 
                      int, int, 
                      halfedge_config<
                        halfedgeS<ContainerS, forwardS<next_at_source_tag> >, 
                        noVertexS, noFacetS> > 
                >() ));
    BOOST_CHECK(( halfedge_next_at_target_test<
                    halfedge_gen<
                      halfedgeS<ContainerS, forwardS<next_at_target_tag> >, 
                      int, int, 
                      halfedge_config<
                        halfedgeS<ContainerS, forwardS<next_at_target_tag> >, 
                        noVertexS, noFacetS> > 
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
