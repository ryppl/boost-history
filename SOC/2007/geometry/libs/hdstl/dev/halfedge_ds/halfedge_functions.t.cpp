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
void set_next_helper_test(HalfedgeGen& H, HalfedgeDescriptor h, HalfedgeDescriptor g, ContainerS const&) {
    (void) H;
    h->m_next = g;
}

template <typename HalfedgeGen,typename HalfedgeDescriptor>
void set_next_helper_test(HalfedgeGen& H, HalfedgeDescriptor h, HalfedgeDescriptor g, vecS const&) {
    H.m_container[h].m_next = g;
}

template <typename HalfedgeGen, typename HalfedgeDescriptor, typename ContainerS>
void set_prev_helper_test(HalfedgeGen& H, HalfedgeDescriptor h, HalfedgeDescriptor g, ContainerS const&) {
    (void) H;
    h->m_prev = g;
}

template <typename HalfedgeGen,typename HalfedgeDescriptor>
void set_prev_helper_test(HalfedgeGen& H, HalfedgeDescriptor h, HalfedgeDescriptor g, vecS const&) {
    H.m_container[h].m_prev = g;
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
    set_next_helper_test(halfedgeGen, ha, hc, container_selector());
    set_next_helper_test(halfedgeGen, hc, he, container_selector());
    set_next_helper_test(halfedgeGen, hj, hd, container_selector());
    set_next_helper_test(halfedgeGen, hd, hg, container_selector());
    set_next_helper_test(halfedgeGen, hh, hb, container_selector());
    set_next_helper_test(halfedgeGen, hf, hi, container_selector());
    BOOST_CHECK(( next_in_facet(hc, halfedgeGen) == he));
    BOOST_CHECK(( next_at_source(hc, halfedgeGen) == hg));
    BOOST_CHECK(( next_at_target(hc, halfedgeGen) == hf));
    
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
    set_next_helper_test(halfedgeGen, hc, hg, container_selector());
    set_next_helper_test(halfedgeGen, hg, hb, container_selector());
    set_next_helper_test(halfedgeGen, hd, he, container_selector());
    set_next_helper_test(halfedgeGen, hb, hc, container_selector());
    set_next_helper_test(halfedgeGen, he, hi, container_selector());
    set_next_helper_test(halfedgeGen, hi, hd, container_selector());
    BOOST_CHECK(( next_in_facet(hc, halfedgeGen) == he));
    BOOST_CHECK(( next_at_source(hc, halfedgeGen) == hg));
    BOOST_CHECK(( next_at_target(hc, halfedgeGen) == hf));
    (void) ha;
    (void) hh;
    (void) hj;

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
    set_next_helper_test(halfedgeGen, ha, hd, container_selector());
    set_next_helper_test(halfedgeGen, hc, hf, container_selector());
    set_next_helper_test(halfedgeGen, hd, hh, container_selector());
    set_next_helper_test(halfedgeGen, hh, ha, container_selector());
    set_next_helper_test(halfedgeGen, hj, hc, container_selector());
    set_next_helper_test(halfedgeGen, hf, hj, container_selector());
    BOOST_CHECK(( next_in_facet(hc, halfedgeGen) == he));
    BOOST_CHECK(( next_at_source(hc, halfedgeGen) == hg));
    BOOST_CHECK(( next_at_target(hc, halfedgeGen) == hf));
    (void) hb;
    (void) hi;
    
    return true;
}

template <typename HalfedgeGen>
bool halfedge_prev_in_facet_test() {
    
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
    set_prev_helper_test(halfedgeGen, hc, ha, container_selector());
    set_prev_helper_test(halfedgeGen, he, hc, container_selector());
    set_prev_helper_test(halfedgeGen, hb, hh, container_selector());
    set_prev_helper_test(halfedgeGen, hi, hf, container_selector());
    set_prev_helper_test(halfedgeGen, hg, hd, container_selector());
    set_prev_helper_test(halfedgeGen, hd, hj, container_selector());
    BOOST_CHECK(( prev_in_facet(hc, halfedgeGen) == ha));
    BOOST_CHECK(( prev_at_source(hc, halfedgeGen) == hb));
    BOOST_CHECK(( prev_at_target(hc, halfedgeGen) == hj));
    
    return true;
}

template <typename HalfedgeGen>
bool halfedge_prev_at_source_test() {
    
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
    set_prev_helper_test(halfedgeGen, hc, hb, container_selector());
    set_prev_helper_test(halfedgeGen, hg, hc, container_selector());
    set_prev_helper_test(halfedgeGen, hb, hg, container_selector());
    set_prev_helper_test(halfedgeGen, he, hd, container_selector());
    set_prev_helper_test(halfedgeGen, hi, he, container_selector());
    set_prev_helper_test(halfedgeGen, hd, hi, container_selector());
    BOOST_CHECK(( prev_in_facet(hc, halfedgeGen) == ha));
    BOOST_CHECK(( prev_at_source(hc, halfedgeGen) == hb));
    BOOST_CHECK(( prev_at_target(hc, halfedgeGen) == hj));
    (void) hf;
    (void) hh;
    return true;
}

template <typename HalfedgeGen>
bool halfedge_prev_at_target_test() {
    
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
    set_prev_helper_test(halfedgeGen, hc, hj, container_selector());
    set_prev_helper_test(halfedgeGen, hd, ha, container_selector());
    set_prev_helper_test(halfedgeGen, ha, hh, container_selector());
    set_prev_helper_test(halfedgeGen, hh, hd, container_selector());
    set_prev_helper_test(halfedgeGen, hf, hc, container_selector());
    set_prev_helper_test(halfedgeGen, hj, hf, container_selector());
    BOOST_CHECK(( prev_in_facet(hc, halfedgeGen) == ha));
    BOOST_CHECK(( prev_at_source(hc, halfedgeGen) == hb));
    BOOST_CHECK(( prev_at_target(hc, halfedgeGen) == hj));
    (void) he;
    (void) hg;
    (void) hi;
    
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

template <typename HalfedgeGen>
bool halfedge_set_next_in_facet_test() {
    
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
    set_next_in_facet(ha, hc, halfedgeGen);
    set_next_in_facet(hc, he, halfedgeGen);
    set_next_in_facet(hj, hd, halfedgeGen);
    set_next_in_facet(hd, hg, halfedgeGen);
    set_next_in_facet(hh, hb, halfedgeGen);
    set_next_in_facet(hf, hi, halfedgeGen);
    BOOST_CHECK(( next_in_facet(hc, halfedgeGen) == he));
    BOOST_CHECK(( next_at_source(hc, halfedgeGen) == hg));
    BOOST_CHECK(( next_at_target(hc, halfedgeGen) == hf));
    BOOST_CHECK(( next_in_facet(ha, halfedgeGen) == hc));
    BOOST_CHECK(( next_in_facet(hj, halfedgeGen) == hd));
    BOOST_CHECK(( next_in_facet(hd, halfedgeGen) == hg));
    BOOST_CHECK(( next_in_facet(hh, halfedgeGen) == hb));
    BOOST_CHECK(( next_in_facet(hf, halfedgeGen) == hi));
    
    return true;
}

template <typename HalfedgeGen>
bool halfedge_set_next_at_source_test() {
    
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
    set_next_at_source(hc, hg, halfedgeGen);
    set_next_at_source(hg, hb, halfedgeGen);
    set_next_at_source(hd, he, halfedgeGen);
    set_next_at_source(hb, hc, halfedgeGen);
    set_next_at_source(he, hi, halfedgeGen);
    set_next_at_source(hi, hd, halfedgeGen);
    BOOST_CHECK(( next_in_facet(hc, halfedgeGen) == he));
    BOOST_CHECK(( next_at_source(hc, halfedgeGen) == hg));
    BOOST_CHECK(( next_at_target(hc, halfedgeGen) == hf));
    BOOST_CHECK(( next_at_source(hc, halfedgeGen) == hg));
    BOOST_CHECK(( next_at_source(hg, halfedgeGen) == hb));
    BOOST_CHECK(( next_at_source(hd, halfedgeGen) == he));
    BOOST_CHECK(( next_at_source(hb, halfedgeGen) == hc));
    BOOST_CHECK(( next_at_source(he, halfedgeGen) == hi));
    BOOST_CHECK(( next_at_source(hi, halfedgeGen) == hd));
    (void) ha;
    (void) hh;
    (void) hj;

    return true;
}

template <typename HalfedgeGen>
bool halfedge_set_next_at_target_test() {
    
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
    set_next_at_target(ha, hd, halfedgeGen);
    set_next_at_target(hc, hf, halfedgeGen);
    set_next_at_target(hd, hh, halfedgeGen);
    set_next_at_target(hh, ha, halfedgeGen);
    set_next_at_target(hj, hc, halfedgeGen);
    set_next_at_target(hf, hj, halfedgeGen);
    BOOST_CHECK(( next_in_facet(hc, halfedgeGen) == he));
    BOOST_CHECK(( next_at_source(hc, halfedgeGen) == hg));
    BOOST_CHECK(( next_at_target(hc, halfedgeGen) == hf));
    BOOST_CHECK(( next_at_target(ha, halfedgeGen) == hd));
    BOOST_CHECK(( next_at_target(hd, halfedgeGen) == hh));
    BOOST_CHECK(( next_at_target(hh, halfedgeGen) == ha));
    BOOST_CHECK(( next_at_target(hj, halfedgeGen) == hc));
    BOOST_CHECK(( next_at_target(hf, halfedgeGen) == hj));
    (void) hb;
    (void) hi;
    
    return true;
}

template <typename HalfedgeGen>
bool halfedge_set_prev_in_facet_test() {
    
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
    set_prev_in_facet(hc, ha, halfedgeGen);
    set_prev_in_facet(he, hc, halfedgeGen);
    set_prev_in_facet(hb, hh, halfedgeGen);
    set_prev_in_facet(hi, hf, halfedgeGen);
    set_prev_in_facet(hg, hd, halfedgeGen);
    set_prev_in_facet(hd, hj, halfedgeGen);
    BOOST_CHECK(( prev_in_facet(hc, halfedgeGen) == ha));
    BOOST_CHECK(( prev_at_source(hc, halfedgeGen) == hb));
    BOOST_CHECK(( prev_at_target(hc, halfedgeGen) == hj));
    BOOST_CHECK(( prev_in_facet(he, halfedgeGen) == hc));
    BOOST_CHECK(( prev_in_facet(hb, halfedgeGen) == hh));
    BOOST_CHECK(( prev_in_facet(hi, halfedgeGen) == hf));
    BOOST_CHECK(( prev_in_facet(hg, halfedgeGen) == hd));
    BOOST_CHECK(( prev_in_facet(hd, halfedgeGen) == hj));
    
    return true;
}

template <typename HalfedgeGen>
bool halfedge_set_prev_at_source_test() {
    
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
    set_prev_at_source(hc, hb, halfedgeGen);
    set_prev_at_source(hg, hc, halfedgeGen);
    set_prev_at_source(hb, hg, halfedgeGen);
    set_prev_at_source(he, hd, halfedgeGen);
    set_prev_at_source(hi, he, halfedgeGen);
    set_prev_at_source(hd, hi, halfedgeGen);
    BOOST_CHECK(( prev_in_facet(hc, halfedgeGen) == ha));
    BOOST_CHECK(( prev_at_source(hc, halfedgeGen) == hb));
    BOOST_CHECK(( prev_at_target(hc, halfedgeGen) == hj));
    BOOST_CHECK(( prev_at_source(hg, halfedgeGen) == hc));
    BOOST_CHECK(( prev_at_source(hb, halfedgeGen) == hg));
    BOOST_CHECK(( prev_at_source(he, halfedgeGen) == hd));
    BOOST_CHECK(( prev_at_source(hi, halfedgeGen) == he));
    BOOST_CHECK(( prev_at_source(hd, halfedgeGen) == hi));
    (void) hf;
    (void) hh;
    return true;
}

template <typename HalfedgeGen>
bool halfedge_set_prev_at_target_test() {
    
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
    set_prev_at_target(hc, hj, halfedgeGen);
    set_prev_at_target(hd, ha, halfedgeGen);
    set_prev_at_target(ha, hh, halfedgeGen);
    set_prev_at_target(hh, hd, halfedgeGen);
    set_prev_at_target(hf, hc, halfedgeGen);
    set_prev_at_target(hj, hf, halfedgeGen);
    BOOST_CHECK(( prev_in_facet(hc, halfedgeGen) == ha));
    BOOST_CHECK(( prev_at_source(hc, halfedgeGen) == hb));
    BOOST_CHECK(( prev_at_target(hc, halfedgeGen) == hj));
    BOOST_CHECK(( prev_at_target(hd, halfedgeGen) == ha));
    BOOST_CHECK(( prev_at_target(ha, halfedgeGen) == hh));
    BOOST_CHECK(( prev_at_target(hh, halfedgeGen) == hd));
    BOOST_CHECK(( prev_at_target(hf, halfedgeGen) == hc));
    BOOST_CHECK(( prev_at_target(hj, halfedgeGen) == hf));
    (void) he;
    (void) hg;
    (void) hi;
    
    return true;
}

template <typename HalfedgeGen>
bool halfedge_add_edge_test() {
    
    // Types must exist.
    typedef typename HalfedgeGen::halfedge_selector      halfedge_selector;
    typedef typename HalfedgeGen::halfedge_descriptor    halfedge_descriptor;
    typedef typename HalfedgeGen::halfedge_iterator      halfedge_iterator;
    typedef typename HalfedgeGen::halfedge_type          halfedge_type;
    typedef typename HalfedgeGen::ContainerGen           container_gen;
    typedef typename HalfedgeGen::ContainerGen::selector container_selector;
    typedef typename HalfedgeGen::container_type         container_type;

     
    // Construct a halfedge_gen object, add hh and hg halfedge pair and
    // test that they are added properly.

    HalfedgeGen halfedgeGen;
    halfedge_descriptor hg = static_cast<halfedge_descriptor>(new_edge(halfedgeGen));

    halfedge_iterator begin = halfedges_begin(halfedgeGen);
    halfedge_descriptor hh = *begin;
    BOOST_CHECK(( opposite(hh, halfedgeGen) == hg));
    BOOST_CHECK(( opposite(hg, halfedgeGen) == hh));
    
    halfedge_descriptor hi = static_cast<halfedge_descriptor>(new_edge(halfedgeGen));
    halfedge_iterator end = --halfedges_end(halfedgeGen);
    halfedge_descriptor hj = *--end;
    BOOST_CHECK(( opposite(hh, halfedgeGen) == hg));
    BOOST_CHECK(( opposite(hg, halfedgeGen) == hh));
    BOOST_CHECK(( opposite(hi, halfedgeGen) == hj));
    BOOST_CHECK(( opposite(hj, halfedgeGen) == hi));
    
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

    // Check prev_... functions 
    BOOST_CHECK(( halfedge_prev_in_facet_test<
                    halfedge_gen<
                      halfedgeS<ContainerS, backwardS<prev_in_facet_tag> >, 
                      int, int, 
                      halfedge_config<
                        halfedgeS<ContainerS, backwardS<prev_in_facet_tag> >, 
                        noVertexS, noFacetS> > 
                  >() ));
    BOOST_CHECK(( halfedge_prev_at_source_test<
                    halfedge_gen<
                      halfedgeS<ContainerS, backwardS<prev_at_source_tag> >, 
                      int, int, 
                      halfedge_config<
                        halfedgeS<ContainerS, backwardS<prev_at_source_tag> >, 
                        noVertexS, noFacetS> > 
                >() ));
    BOOST_CHECK(( halfedge_prev_at_target_test<
                    halfedge_gen<
                      halfedgeS<ContainerS, backwardS<prev_at_target_tag> >, 
                      int, int, 
                      halfedge_config<
                        halfedgeS<ContainerS, backwardS<prev_at_target_tag> >, 
                        noVertexS, noFacetS> > 
                  >() ));

    // Check next_ and prev_ functions for bidirS 
    BOOST_CHECK(( halfedge_next_in_facet_test<
                    halfedge_gen<
                      halfedgeS<ContainerS, bidirS<next_in_facet_tag,
                                                   prev_in_facet_tag> >, 
                      int, int, 
                      halfedge_config<
                        halfedgeS<ContainerS, bidirS<next_in_facet_tag,
                                                     prev_in_facet_tag> >, 
                        noVertexS, noFacetS> > 
                  >() ));
    BOOST_CHECK(( halfedge_prev_in_facet_test<
                    halfedge_gen<
                      halfedgeS<ContainerS, bidirS<next_in_facet_tag,
                                                   prev_in_facet_tag> >, 
                      int, int, 
                      halfedge_config<
                        halfedgeS<ContainerS, bidirS<next_in_facet_tag,
                                                     prev_in_facet_tag> >, 
                        noVertexS, noFacetS> > 
                  >() ));
    BOOST_CHECK(( halfedge_next_at_source_test<
                    halfedge_gen<
                      halfedgeS<ContainerS, bidirS<next_at_source_tag,
                                                   prev_at_source_tag> >, 
                      int, int, 
                      halfedge_config<
                        halfedgeS<ContainerS, bidirS<next_at_source_tag,
                                                       prev_at_source_tag> >, 
                        noVertexS, noFacetS> > 
                  >() ));
    BOOST_CHECK(( halfedge_prev_at_source_test<
                    halfedge_gen<
                      halfedgeS<ContainerS, bidirS<next_at_source_tag,
                                                   prev_at_source_tag> >, 
                      int, int, 
                      halfedge_config<
                        halfedgeS<ContainerS, bidirS<next_at_source_tag,
                                                       prev_at_source_tag> >, 
                        noVertexS, noFacetS> > 
                  >() ));
    BOOST_CHECK(( halfedge_next_at_target_test<
                    halfedge_gen<
                      halfedgeS<ContainerS, bidirS<next_at_target_tag,
                                                   prev_at_target_tag> >, 
                      int, int, 
                      halfedge_config<
                        halfedgeS<ContainerS, bidirS<next_at_target_tag,
                                                     prev_at_target_tag> >, 
                        noVertexS, noFacetS> > 
                  >() ));
    BOOST_CHECK(( halfedge_prev_at_target_test<
                    halfedge_gen<
                      halfedgeS<ContainerS, bidirS<next_at_target_tag,
                                                   prev_at_target_tag> >, 
                      int, int, 
                      halfedge_config<
                        halfedgeS<ContainerS, bidirS<next_at_target_tag,
                                                     prev_at_target_tag> >, 
                        noVertexS, noFacetS> > 
                  >() ));

    //check set_next_... functions
    BOOST_CHECK(( halfedge_set_next_in_facet_test<
                    halfedge_gen<
                      halfedgeS<ContainerS, forwardS<next_in_facet_tag> >, 
                      int, int, 
                      halfedge_config<
                        halfedgeS<ContainerS, forwardS<next_in_facet_tag> >, 
                        noVertexS, noFacetS> > 
                  >() ));
    BOOST_CHECK(( halfedge_set_next_at_source_test<
                    halfedge_gen<
                      halfedgeS<ContainerS, forwardS<next_at_source_tag> >, 
                      int, int, 
                      halfedge_config<
                        halfedgeS<ContainerS, forwardS<next_at_source_tag> >, 
                        noVertexS, noFacetS> > 
                  >() ));
    BOOST_CHECK(( halfedge_set_next_at_target_test<
                    halfedge_gen<
                      halfedgeS<ContainerS, forwardS<next_at_target_tag> >, 
                      int, int, 
                      halfedge_config<
                        halfedgeS<ContainerS, forwardS<next_at_target_tag> >, 
                        noVertexS, noFacetS> > 
                  >() ));
    
    //check set_prev_... functions
    BOOST_CHECK(( halfedge_set_prev_in_facet_test<
                    halfedge_gen<
                      halfedgeS<ContainerS, backwardS<prev_in_facet_tag> >, 
                      int, int, 
                      halfedge_config<
                        halfedgeS<ContainerS, backwardS<prev_in_facet_tag> >, 
                        noVertexS, noFacetS> > 
                  >() ));
    BOOST_CHECK(( halfedge_set_prev_at_source_test<
                    halfedge_gen<
                      halfedgeS<ContainerS, backwardS<prev_at_source_tag> >, 
                      int, int, 
                      halfedge_config<
                        halfedgeS<ContainerS, backwardS<prev_at_source_tag> >, 
                        noVertexS, noFacetS> > 
                  >() ));
    BOOST_CHECK(( halfedge_set_prev_at_target_test<
                    halfedge_gen<
                      halfedgeS<ContainerS, backwardS<prev_at_target_tag> >, 
                      int, int, 
                      halfedge_config<
                        halfedgeS<ContainerS, backwardS<prev_at_target_tag> >, 
                        noVertexS, noFacetS> > 
                  >() ));
    
    // Check set_next_ and set_prev_ functions for bidirS 
    BOOST_CHECK(( halfedge_set_next_in_facet_test<
                    halfedge_gen<
                      halfedgeS<ContainerS, bidirS<next_in_facet_tag,
                                                   prev_in_facet_tag> >, 
                      int, int, 
                      halfedge_config<
                        halfedgeS<ContainerS, bidirS<next_in_facet_tag,
                                                     prev_in_facet_tag> >, 
                        noVertexS, noFacetS> > 
                  >() ));
    BOOST_CHECK(( halfedge_set_prev_in_facet_test<
                    halfedge_gen<
                      halfedgeS<ContainerS, bidirS<next_in_facet_tag,
                                                   prev_in_facet_tag> >, 
                      int, int, 
                      halfedge_config<
                        halfedgeS<ContainerS, bidirS<next_in_facet_tag,
                                                     prev_in_facet_tag> >, 
                        noVertexS, noFacetS> > 
                  >() ));
    BOOST_CHECK(( halfedge_set_next_at_source_test<
                    halfedge_gen<
                      halfedgeS<ContainerS, bidirS<next_at_source_tag,
                                                   prev_at_source_tag> >, 
                      int, int, 
                      halfedge_config<
                        halfedgeS<ContainerS, bidirS<next_at_source_tag,
                                                       prev_at_source_tag> >, 
                        noVertexS, noFacetS> > 
                  >() ));
    BOOST_CHECK(( halfedge_set_prev_at_source_test<
                    halfedge_gen<
                      halfedgeS<ContainerS, bidirS<next_at_source_tag,
                                                   prev_at_source_tag> >, 
                      int, int, 
                      halfedge_config<
                        halfedgeS<ContainerS, bidirS<next_at_source_tag,
                                                       prev_at_source_tag> >, 
                        noVertexS, noFacetS> > 
                  >() ));
    BOOST_CHECK(( halfedge_set_next_at_target_test<
                    halfedge_gen<
                      halfedgeS<ContainerS, bidirS<next_at_target_tag,
                                                   prev_at_target_tag> >, 
                      int, int, 
                      halfedge_config<
                        halfedgeS<ContainerS, bidirS<next_at_target_tag,
                                                     prev_at_target_tag> >, 
                        noVertexS, noFacetS> > 
                  >() ));
    BOOST_CHECK(( halfedge_set_prev_at_target_test<
                    halfedge_gen<
                      halfedgeS<ContainerS, bidirS<next_at_target_tag,
                                                   prev_at_target_tag> >, 
                      int, int, 
                      halfedge_config<
                        halfedgeS<ContainerS, bidirS<next_at_target_tag,
                                                     prev_at_target_tag> >, 
                        noVertexS, noFacetS> > 
                  >() ));
    
    // add_edge test
    BOOST_CHECK(( halfedge_add_edge_test<
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
