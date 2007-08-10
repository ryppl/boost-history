//base_functions.t.cpp   -*- C++ -*-
//
//@OVERVIEW:  
//
//@TEST_PLAN:
//

#include <boost/hdstl/euler/base_functions.hpp>
#include <boost/hdstl/halfedge_ds/halfedge_functions.hpp>
#include <boost/hdstl/halfedge_ds/halfedge_selectors.hpp>
#include <boost/hdstl/halfedge_ds/facet_functions.hpp>
#include <boost/hdstl/halfedge_ds/vertex_functions.hpp>
#include <boost/hdstl/halfedge_ds/halfedge_ds.hpp>
#include <boost/test/minimal.hpp>

#include <set>
#include <string>
#include <vector>

using namespace boost::hdstl;
using namespace std;

// ===========================================================================
//                              CLASS HALFEDGE_GEN
// ===========================================================================

template <typename HalfedgeGen>
void create_hds(HalfedgeGen& halfedgeGen){
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
    halfedge_type array[] = { fa, fb};  
    
    // Construct a halfedge_gen object whose container contains array.  Verify
    // that halfedges_begin(), halfedges_end(), and num_halfedges() work.

    container_type halfedges(array, array+2);
    halfedgeGen.m_container = halfedges;
}

template <typename HalfedgeGen, typename TagS>
bool halfedge_set_next_test(TagS const&) {
    
    typedef typename HalfedgeGen::halfedge_descriptor    halfedge_descriptor;
    typedef typename HalfedgeGen::halfedge_iterator      halfedge_iterator;
    typedef typename HalfedgeGen::ContainerGen::selector container_selector;

    HalfedgeGen halfedgeGen;
    create_hds(halfedgeGen);

    halfedge_iterator begin = halfedges_begin(halfedgeGen);
    halfedge_descriptor ha = *begin;
    halfedge_descriptor hb = *(++begin);

    stitch_next_in_facet(ha, hb, halfedgeGen, next_in_facet_tag());
    stitch_next_at_source(ha, hb, halfedgeGen, next_in_facet_tag());
    stitch_next_at_target(ha, hb, halfedgeGen, next_in_facet_tag());
    
    BOOST_CHECK(( next_in_facet(ha, halfedgeGen) == hb )); 
    BOOST_CHECK(( get_next_in_facet(ha, halfedgeGen, next_in_facet_tag()) == hb )); 

    return true;
}

template <typename HalfedgeGen>
bool halfedge_set_next_test(next_at_source_tag const&) {
    
    typedef typename HalfedgeGen::halfedge_descriptor    halfedge_descriptor;
    typedef typename HalfedgeGen::halfedge_iterator      halfedge_iterator;
    typedef typename HalfedgeGen::ContainerGen::selector container_selector;

    HalfedgeGen halfedgeGen;
    create_hds(halfedgeGen);

    halfedge_iterator begin = halfedges_begin(halfedgeGen);
    halfedge_descriptor ha = *begin;
    halfedge_descriptor hb = *(++begin);

    stitch_next_in_facet(ha, hb, halfedgeGen, next_at_source_tag());
    stitch_next_at_source(ha, hb, halfedgeGen, next_at_source_tag());
    stitch_next_at_target(ha, hb, halfedgeGen, next_at_source_tag());
    
    BOOST_CHECK(( next_at_source(ha, halfedgeGen) == hb )); 

    return true;
}

template <typename HalfedgeGen>
bool halfedge_set_next_test(next_at_target_tag const&) {
    
    typedef typename HalfedgeGen::halfedge_descriptor    halfedge_descriptor;
    typedef typename HalfedgeGen::halfedge_iterator      halfedge_iterator;
    typedef typename HalfedgeGen::ContainerGen::selector container_selector;

    HalfedgeGen halfedgeGen;
    create_hds(halfedgeGen);

    halfedge_iterator begin = halfedges_begin(halfedgeGen);
    halfedge_descriptor ha = *begin;
    halfedge_descriptor hb = *(++begin);

    stitch_next_in_facet(ha, hb, halfedgeGen, next_at_target_tag());
    stitch_next_at_source(ha, hb, halfedgeGen, next_at_target_tag());
    stitch_next_at_target(ha, hb, halfedgeGen, next_at_target_tag());
    
    BOOST_CHECK(( next_at_target(ha, halfedgeGen) == hb )); 

    return true;
}

template <typename HalfedgeGen, typename TagS>
bool halfedge_set_prev_test(TagS const&) {
    
    typedef typename HalfedgeGen::halfedge_descriptor    halfedge_descriptor;
    typedef typename HalfedgeGen::halfedge_iterator      halfedge_iterator;
    typedef typename HalfedgeGen::ContainerGen::selector container_selector;

    HalfedgeGen halfedgeGen;
    create_hds(halfedgeGen);

    halfedge_iterator begin = halfedges_begin(halfedgeGen);
    halfedge_descriptor ha = *begin;
    halfedge_descriptor hb = *(++begin);

    stitch_prev_in_facet(ha, hb, halfedgeGen, prev_in_facet_tag());
    stitch_prev_at_source(ha, hb, halfedgeGen, prev_in_facet_tag());
    stitch_prev_at_target(ha, hb, halfedgeGen, prev_in_facet_tag());
    
    BOOST_CHECK(( prev_in_facet(ha, halfedgeGen) == hb )); 
    BOOST_CHECK(( get_prev_in_facet(ha, halfedgeGen, prev_in_facet_tag()) == hb )); 

    return true;
}

template <typename HalfedgeGen>
bool halfedge_set_prev_test(prev_at_source_tag const&) {
    
    typedef typename HalfedgeGen::halfedge_descriptor    halfedge_descriptor;
    typedef typename HalfedgeGen::halfedge_iterator      halfedge_iterator;
    typedef typename HalfedgeGen::ContainerGen::selector container_selector;

    HalfedgeGen halfedgeGen;
    create_hds(halfedgeGen);
    
    halfedge_iterator begin = halfedges_begin(halfedgeGen);
    halfedge_descriptor ha = *begin;
    halfedge_descriptor hb = *(++begin);

    stitch_prev_in_facet(ha, hb, halfedgeGen, prev_at_source_tag());
    stitch_prev_at_source(ha, hb, halfedgeGen, prev_at_source_tag());
    stitch_prev_at_target(ha, hb, halfedgeGen, prev_at_source_tag());
    
    BOOST_CHECK(( prev_at_source(ha, halfedgeGen) == hb )); 

    return true;
}

template <typename HalfedgeGen>
bool halfedge_set_prev_test(prev_at_target_tag const&) {
    
    typedef typename HalfedgeGen::halfedge_descriptor    halfedge_descriptor;
    typedef typename HalfedgeGen::halfedge_iterator      halfedge_iterator;
    typedef typename HalfedgeGen::ContainerGen::selector container_selector;

    HalfedgeGen halfedgeGen;
    create_hds(halfedgeGen);

    halfedge_iterator begin = halfedges_begin(halfedgeGen);
    halfedge_descriptor ha = *begin;
    halfedge_descriptor hb = *(++begin);

    stitch_prev_in_facet(ha, hb, halfedgeGen, prev_at_target_tag());
    stitch_prev_at_source(ha, hb, halfedgeGen, prev_at_target_tag());
    stitch_prev_at_target(ha, hb, halfedgeGen, prev_at_target_tag());
    
    BOOST_CHECK(( prev_at_target(ha, halfedgeGen) == hb )); 

    return true;
}

// ===========================================================================
//                              BOOST TEST APPARATUS
// ===========================================================================

template <typename ContainerS, typename TraversalS, typename VertexS, typename FacetS>
bool test_set_next_functions()
{

    typedef typename TraversalS::next_tag next_tag;
    //check set_next_... functions
    BOOST_CHECK(( halfedge_set_next_test<
                    halfedge_gen<
                      halfedgeS<ContainerS, TraversalS>, 
                      int, int, 
                      typename halfedge_ds_gen<
                        halfedgeS<ContainerS, TraversalS>, 
                        VertexS, FacetS>::config> 
                  >(next_tag()) ));
   return true;
}

template <typename ContainerS, typename TraversalS, typename VertexS, typename FacetS>
bool test_set_prev_functions()
{

    typedef typename TraversalS::prev_tag prev_tag;
    //check set_prev_... functions
    BOOST_CHECK(( halfedge_set_prev_test<
                    halfedge_gen<
                      halfedgeS<ContainerS, TraversalS>, 
                      int, int, 
                      typename halfedge_ds_gen<
                        halfedgeS<ContainerS, TraversalS>, 
                        VertexS, FacetS>::config> 
                  >(prev_tag()) ));
    return true;
}

template<typename ContainerS, typename VertexS, typename FacetS>
bool test_set_functions_all() 
{
    
    BOOST_CHECK(( test_set_next_functions<ContainerS, forwardS<next_in_facet_tag>, VertexS, FacetS>() ));
    BOOST_CHECK(( test_set_next_functions<ContainerS, forwardS<next_at_source_tag>, VertexS, FacetS>() ));
    BOOST_CHECK(( test_set_next_functions<ContainerS, forwardS<next_at_target_tag>, VertexS, FacetS>() ));
    
    BOOST_CHECK(( test_set_prev_functions<ContainerS, backwardS<prev_in_facet_tag>, VertexS, FacetS>() ));
    BOOST_CHECK(( test_set_prev_functions<ContainerS, backwardS<prev_at_source_tag>, VertexS, FacetS>() ));
    BOOST_CHECK(( test_set_prev_functions<ContainerS, backwardS<prev_at_target_tag>, VertexS, FacetS>() ));
    
    BOOST_CHECK(( test_set_next_functions<ContainerS, bidirS<next_in_facet_tag, prev_in_facet_tag>, 
                                                                                     VertexS, FacetS>() ));
    BOOST_CHECK(( test_set_next_functions<ContainerS, bidirS<next_in_facet_tag, prev_at_source_tag>, 
                                                                                     VertexS, FacetS>() ));
    BOOST_CHECK(( test_set_next_functions<ContainerS, bidirS<next_in_facet_tag, prev_at_target_tag>, 
                                                                                     VertexS, FacetS>() ));

    BOOST_CHECK(( test_set_next_functions<ContainerS, bidirS<next_at_source_tag, prev_in_facet_tag>, 
                                                                                     VertexS, FacetS>() ));
    BOOST_CHECK(( test_set_next_functions<ContainerS, bidirS<next_at_source_tag, prev_at_source_tag>, 
                                                                                     VertexS, FacetS>() ));
    BOOST_CHECK(( test_set_next_functions<ContainerS, bidirS<next_at_source_tag, prev_at_target_tag>, 
                                                                                     VertexS, FacetS>() ));

    BOOST_CHECK(( test_set_next_functions<ContainerS, bidirS<next_at_target_tag, prev_in_facet_tag>, 
                                                                                     VertexS, FacetS>() ));
    BOOST_CHECK(( test_set_next_functions<ContainerS, bidirS<next_at_target_tag, prev_at_source_tag>, 
                                                                                     VertexS, FacetS>() ));
    BOOST_CHECK(( test_set_next_functions<ContainerS, bidirS<next_at_target_tag, prev_at_target_tag>, 
                                                                                     VertexS, FacetS>() ));
    
    BOOST_CHECK(( test_set_prev_functions<ContainerS, bidirS<next_in_facet_tag, prev_in_facet_tag>, 
                                                                                     VertexS, FacetS>() ));
    BOOST_CHECK(( test_set_prev_functions<ContainerS, bidirS<next_in_facet_tag, prev_at_source_tag>, 
                                                                                     VertexS, FacetS>() ));
    BOOST_CHECK(( test_set_prev_functions<ContainerS, bidirS<next_in_facet_tag, prev_at_target_tag>, 
                                                                                     VertexS, FacetS>() ));

    BOOST_CHECK(( test_set_prev_functions<ContainerS, bidirS<next_at_source_tag, prev_in_facet_tag>, 
                                                                                     VertexS, FacetS>() ));
    BOOST_CHECK(( test_set_prev_functions<ContainerS, bidirS<next_at_source_tag, prev_at_source_tag>, 
                                                                                     VertexS, FacetS>() ));
    BOOST_CHECK(( test_set_prev_functions<ContainerS, bidirS<next_at_source_tag, prev_at_target_tag>, 
                                                                                     VertexS, FacetS>() ));

    BOOST_CHECK(( test_set_prev_functions<ContainerS, bidirS<next_at_target_tag, prev_in_facet_tag>, 
                                                                                     VertexS, FacetS>() ));
    BOOST_CHECK(( test_set_prev_functions<ContainerS, bidirS<next_at_target_tag, prev_at_source_tag>, 
                                                                                     VertexS, FacetS>() ));
    BOOST_CHECK(( test_set_prev_functions<ContainerS, bidirS<next_at_target_tag, prev_at_target_tag>, 
                                                                                     VertexS, FacetS>() ));
    return true;
}

template<typename ContainerS>
bool test_all()
{
    BOOST_CHECK(( test_set_functions_all<ContainerS, noVertexS, noFacetS>() ));
    return true;
}

int test_main(int, char**)
{
    BOOST_CHECK(( test_all<listS>() ));
    BOOST_CHECK(( test_all<vecS>() ));

    return 0;
}
