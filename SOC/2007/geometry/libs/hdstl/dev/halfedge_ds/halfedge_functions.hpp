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
template <typename HalfedgeS, typename VertexDescriptor, typename FacetDescriptor, typename Config>
typename halfedge_gen<HalfedgeS, VertexDescriptor, FacetDescriptor, Config>::halfedge_iterator
halfedges_begin(halfedge_gen<HalfedgeS,  VertexDescriptor, FacetDescriptor, Config>& hds)
{
    typedef typename halfedge_gen<HalfedgeS, VertexDescriptor,FacetDescriptor, Config>
                ::ContainerGen ContainerGen;
    return ContainerGen::container_begin(hds.m_container);
}

template <typename HalfedgeS,  typename VertexDescriptor, typename FacetDescriptor, typename Config>
typename halfedge_gen<HalfedgeS, VertexDescriptor, FacetDescriptor, Config>::halfedge_iterator
halfedges_end(halfedge_gen<HalfedgeS, VertexDescriptor, FacetDescriptor, Config>& hds)
{
    typedef typename halfedge_gen<HalfedgeS, VertexDescriptor,FacetDescriptor, Config>
                ::ContainerGen ContainerGen;
    return ContainerGen::container_end(hds.m_container);
}

template <typename HalfedgeS, typename VertexDescriptor, typename FacetDescriptor, typename Config>
typename halfedge_gen<HalfedgeS,  VertexDescriptor, FacetDescriptor, Config>::size_type
num_halfedges(halfedge_gen<HalfedgeS,  VertexDescriptor, FacetDescriptor, Config>& hds)
{
    return hds.m_container.size();
}

// \cond
template<typename HalfedgeS, typename HalfedgeDescriptor>
struct opposite_function_helper {
    static
    HalfedgeDescriptor
    opposite(HalfedgeDescriptor h)
    {
        return static_cast<HalfedgeDescriptor>(h->m_opposite);
    }
};

template<typename HalfedgeDescriptor, typename TraversalS>
struct opposite_function_helper<halfedgeS<vecS,TraversalS>, HalfedgeDescriptor> {
    static
    HalfedgeDescriptor
    opposite(HalfedgeDescriptor h)
    {
        return h^1;
    }
};
// \endcond

template <typename HalfedgeS,  typename VertexDescriptor, typename FacetDescriptor,
          typename Config>
typename Config::halfedge_descriptor
opposite(typename halfedge_gen<HalfedgeS, VertexDescriptor,FacetDescriptor, Config>::halfedge_descriptor h,
         const halfedge_gen<HalfedgeS, VertexDescriptor, FacetDescriptor, Config>&)
{
    typedef typename halfedge_gen<HalfedgeS, 
                                  VertexDescriptor,FacetDescriptor, Config
                                 >::halfedge_selector halfedge_selector;
    typedef typename halfedge_gen<HalfedgeS, 
                                  VertexDescriptor,FacetDescriptor, Config
                                 >::halfedge_descriptor halfedge_descriptor;
    return opposite_function_helper<halfedge_selector,
                                    halfedge_descriptor
                                   >::opposite(h);
}

} // namespace hdstl
} // namespace boost

#endif
