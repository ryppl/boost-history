//halfedge_functions.hpp   -*- C++ -*-
//
//@PURPOSE: Provide free functions for accessing and manipulating halfedges.
//
//@DESCRIPTION: This file provides free functions for accessing and
// manipulating halfedges. The list of these functions are as follows:
//
//  - halfedges_begin: returns an iterator to the first halfedge in the 'hds' 
//  - halfedges_end: returns an iterator after the last halfedge in the 'hds'
//  - num_halfedges: returns the number of halfedges in the 'hds'
//

#ifndef BOOST_HDSTL_HALFEDGE_FUNCTIONS_HPP
#define BOOST_HDSTL_HALFEDGE_FUNCTIONS_HPP 1

#include <boost/hdstl/halfedge_ds/halfedge_selectors.hpp>

namespace boost {
namespace hdstl {

// FREE FUNCTIONS
template <typename HalfedgeS, typename HalfedgeDescriptor, 
          typename VertexDescriptor, typename FacetDescriptor, typename Config>
typename halfedge_gen<HalfedgeS, HalfedgeDescriptor, VertexDescriptor, 
                                   FacetDescriptor, Config>::halfedge_iterator
halfedges_begin(halfedge_gen<HalfedgeS, HalfedgeDescriptor, VertexDescriptor, 
                                         FacetDescriptor, Config>& hds) {
    typedef typename halfedge_gen<HalfedgeS, HalfedgeDescriptor,
          VertexDescriptor,FacetDescriptor, Config>::ContainerGen ContainerGen;
    return ContainerGen::container_begin(hds.m_container);
}

template <typename HalfedgeS, typename HalfedgeDescriptor, 
          typename VertexDescriptor, typename FacetDescriptor, typename Config>
typename halfedge_gen<HalfedgeS, HalfedgeDescriptor, VertexDescriptor, 
                                    FacetDescriptor, Config>::halfedge_iterator
halfedges_end(halfedge_gen<HalfedgeS, HalfedgeDescriptor, VertexDescriptor, 
                                         FacetDescriptor, Config>& hds) {
    typedef typename halfedge_gen<HalfedgeS, HalfedgeDescriptor,
          VertexDescriptor,FacetDescriptor, Config>::ContainerGen ContainerGen;
    return ContainerGen::container_end(hds.m_container);
}

template <typename HalfedgeS, typename HalfedgeDescriptor, 
          typename VertexDescriptor, typename FacetDescriptor, typename Config>
typename halfedge_gen<HalfedgeS, HalfedgeDescriptor, VertexDescriptor, 
                                    FacetDescriptor, Config>::size_type
num_halfedges(halfedge_gen<HalfedgeS, HalfedgeDescriptor, VertexDescriptor, 
                                         FacetDescriptor, Config>& hds) {
    return hds.m_container.size();
}

template<typename HalfedgeS, typename HalfedgeDescriptor>
struct opposite_function_helper {
    static
    HalfedgeDescriptor&
    opposite(HalfedgeDescriptor& h)
    {
        // halfedges come in pairs, point to each other as
        // opposite. If halfedges stored in a list, use the
        // opposite pointer.
        return h->m_opposite;
    }
};

template<typename HalfedgeDescriptor, typename TraversalS>
struct opposite_function_helper<halfedgeS<vecS,TraversalS>, HalfedgeDescriptor> {
    static
    HalfedgeDescriptor&
    opposite(HalfedgeDescriptor& h)
    {
        // halfedges come in pairs, point to each other as
        // opposite. If halfedges stored in a vector:
        return h^1;
    }
};

template <typename HalfedgeS, typename HalfedgeDescriptor, 
          typename VertexDescriptor, typename FacetDescriptor, typename Config>
HalfedgeDescriptor
opposite(HalfedgeDescriptor& h, halfedge_gen<HalfedgeS, HalfedgeDescriptor, 
                                VertexDescriptor, FacetDescriptor, Config>& hds)
{
    
//  typedef typename halfedge_gen<HalfedgeS, HalfedgeDescriptor,
//                                VertexDescriptor,FacetDescriptor, Config>
//                                ::halfedge_selector halfedge_selector;
//  return opposite_function_helper<halfedge_selector,HalfedgeDescriptor>
//                                 ::opposite(h);
}


} // namespace hdstl
} // namespace boost

#endif
