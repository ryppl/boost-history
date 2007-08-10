//halfedge_ds.hpp   -*- C++ -*-
//
//@PURPOSE: Provide a concrete bu parameterizable implementation of HDS.
//
//@CLASSES:
//  'halfedge_ds_gen':  defines configuration options in a central place
//  'container_gen':  class for selecting the storage type of 'halfedge_ds'
//  'opposite_helper':  class for configuring opposite access methods
//  'halfedge_ds':  swiss-army knife of HDS implementations
//  
//@SEE_ALSO: {hds_concepts.hpp, halfedge_ds.hpp}
//
//@DESCRIPTION: This file provides a single class template, 'halfedge_ds', which
// accepts many template parameters that can be used to govern its
// implementation.  Most of these parameters can be chosen as selectors (see
// 'hds_selectors' component).
//
///Definition
//-----------
// - 'halfedge_ds_gen' this class defines the 'config' subtype and keeps
//   the configuration parameters of the 'HDS' in a central place. The 
//   configurations are obtained based on selectors 'HalfedgeS', 'VertexS', 
//   and 'FacetS'. 
// - 'opposite_helper' this class configures the opposite access method, based
//   on the storage type. Do not use and opposite method and manually calculates
//   the opposite if the storage is 'stl::vector' type, otherwise uses the 
//   defined 'opposite' member.
// - 'halfedge_ds' concrete implementation of the 'HDS'.  

#ifndef BOOST_HDSTL_HALFEDGE_DS_HPP
#define BOOST_HDSTL_HALFEDGE_DS_HPP 1

#include <boost/hdstl/halfedge_ds/meta_functions.hpp>
#include <boost/hdstl/halfedge_ds/halfedge_selectors.hpp>
#include <boost/hdstl/halfedge_ds/vertex_selectors.hpp>
#include <boost/hdstl/halfedge_ds/facet_selectors.hpp>
#include <boost/pending/ct_if.hpp>

namespace boost {
namespace hdstl {

typedef void* halfedge_ptr;

template <typename HalfedgeS, typename VertexS, typename FacetS>
struct halfedge_ds_gen {
    struct config {
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
};

template <typename HalfedgeS>
struct halfedge_ds_gen<HalfedgeS, noVertexS, noFacetS> {
    struct config {
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
};

template <typename HalfedgeS, typename FacetS>
struct halfedge_ds_gen<HalfedgeS, noVertexS, FacetS> {
    struct config {
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
};

template <typename HalfedgeS, typename VertexS>
struct halfedge_ds_gen<HalfedgeS, VertexS, noFacetS> {
    struct config {
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
};


//template <typename HalfedgeS = halfedgeS<vecS, forwardS<next_in_facet_tag> >,
//          typename VertexS = vertexS<vecS, vertexLinkS, sourceS>,
//          typename FacetS = facetS<vecS> >
//class halfedge_ds
//{
//    typedef void* halfedge_ptr; 
//
//    typedef typename  container_gen<HalfedgeS, halfedge_ptr
//                                                   >::is_random is_rand_access;
//
//    typedef typename boost::ct_if_t<is_rand_access,
//                          std::size_t, halfedge_ptr>::type halfedge_descriptor;
//
//    typedef typename  container_gen<HalfedgeS, halfedge_ptr>::type 
//                                                                  HalfedgeList;
//
//    typedef typename HalfedgeList::iterator halfedge_iterator;
//
//};

} // namespace hdstl
} // namespace boost

#endif
