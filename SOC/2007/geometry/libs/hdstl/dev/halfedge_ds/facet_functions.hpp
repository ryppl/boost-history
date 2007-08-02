//facet_functions.hpp   -*- C++ -*-
//
//@PURPOSE: Provide free functions for accessing and manipulating facets.
//
//@DESCRIPTION: This file provides free functions for accessing and
// manipulating facets. The list of these functions are as follows:
//
//  - facets_begin: returns an iterator to the first facet in the 'hds' as part of the 'FacetListHDS' concept. 
//  -   facets_end: returns an iterator after the last facet in the 'hds' as part of the 'FacetListHDS' concept.
//  -   num_facets: returns the number of facets in the 'hds' as part of the 'FacetListHDS' concept.
//  -     halfedge: returns a descriptor to one of the halfedges around this facets cycle as part of the 'FacetHDS'
//                  concept.
//  -        facet: returns the facet to the left of the given halfedge 'h' in the 'hds' as part of the 'FacetHDS'
//                  concept.
//  -    set_facet: sets the facet 'f' as the facet on the left for halfedge 'h' in the 'hds' as part of the 
//                  'MutableFacetHDS' concept.
//  -    new_facet: creates a facet and adds it into the 'hds' as part of the 'MutableFacetHDS' concept.
//  - delete_facet: removes the given facet 'f' from the 'hds' as part of the 'MutableFacetHDS' concept.
//
//@SEE_ALSO: {hds_concepts.hpp, container_selectors.hpp, facet_selectors.hpp}


#ifndef BOOST_HDSTL_FACET_FUNCTIONS_HPP
#define BOOST_HDSTL_FACET_FUNCTIONS_HPP 1

#include <boost/hdstl/halfedge_ds/facet_selectors.hpp>

namespace boost {
namespace hdstl {
                   // ====================
                   // FacetListHDS Concept
                   // ====================

// FREE FUNCTIONS
template <typename FacetS, typename HalfedgeDescriptor, typename FacetBase>
typename facet_gen<FacetS, HalfedgeDescriptor, FacetBase>::facet_iterator
facets_begin(facet_gen<FacetS, HalfedgeDescriptor, FacetBase>& hds)
    // Returns a facet iterator to the beginning of the container member of 'hds'. The value type
    // of the facet iterator is a facet descriptor.
{
    typedef typename facet_gen<FacetS,
                               HalfedgeDescriptor,
                               FacetBase>::ContainerGen ContainerGen;
    return ContainerGen::container_begin(hds.m_container);
}

template <typename FacetS, typename HalfedgeDescriptor, typename FacetBase>
typename facet_gen<FacetS, HalfedgeDescriptor, FacetBase>::facet_iterator
facets_end(facet_gen<FacetS, HalfedgeDescriptor, FacetBase>& hds)
    // Returns a facet iterator to the end of the container member of 'hds'. The value type
    // of the facet iterator is a facet descriptor.
{
    typedef typename facet_gen<FacetS,
                               HalfedgeDescriptor,
                               FacetBase>::ContainerGen ContainerGen;
    return ContainerGen::container_end(hds.m_container);
}

template <typename FacetS, typename HalfedgeDescriptor, typename FacetBase>
typename facet_gen<FacetS, HalfedgeDescriptor, FacetBase>::size_type
num_facets(facet_gen<FacetS, HalfedgeDescriptor, FacetBase> const& hds)
    // Returns the number of facets in the 'hds'. Return type is the size type specific to 
    // the container used, specified in container selectors.
{
    return hds.m_container.size();
}

                   // =======================
                   // FacetHDS Concept
                   // =======================

template <typename FacetS, typename HalfedgeDescriptor, typename FacetBase>
HalfedgeDescriptor
halfedge(typename facet_gen<FacetS, HalfedgeDescriptor, FacetBase>::facet_descriptor const& f,
                  facet_gen<FacetS, HalfedgeDescriptor, FacetBase>& hds)
    // Returns a halfedge descriptor to one of the halfedges around this facet in the 'hds'.
{
    return facet_gen<FacetS, HalfedgeDescriptor, FacetBase>::ContainerGen
                                       ::value(f, hds.m_container).m_facetLink;
}

template <typename FacetS, typename HalfedgeDescriptor, typename FacetBase>
typename facet_gen<FacetS, HalfedgeDescriptor, FacetBase>::facet_descriptor
facet(HalfedgeDescriptor h, facet_gen<FacetS, HalfedgeDescriptor, FacetBase>& hds)
    // Returns a facet descriptor to the facet left of the halfedge given by the descriptor 'h' in the 'hds'.
{
    return facet_gen<FacetS, HalfedgeDescriptor, FacetBase>::ContainerGen
                                       ::value(h, hds.m_container).m_facet;
}
                   
                   // =======================
                   // MutableFacetHDS Concept
                   // =======================

template <typename FacetS, typename HalfedgeDescriptor, typename FacetBase>
void
set_facet(HalfedgeDescriptor h, typename facet_gen<FacetS, HalfedgeDescriptor, FacetBase>::facet_descriptor f,
                                               facet_gen<FacetS, HalfedgeDescriptor, FacetBase>& hds)
    // Sets the facet specified by 'f' as the facet on the left for halfedge specified by 'h' in the 'hds'.
{
    facet_gen<FacetS, HalfedgeDescriptor, FacetBase>::ContainerGen::value(h, hds.m_container).m_facet = f;
}

template <typename FacetS, typename HalfedgeDescriptor, typename FacetBase>
typename facet_gen<FacetS, HalfedgeDescriptor, FacetBase>::facet_descriptor
new_facet(facet_gen<FacetS, HalfedgeDescriptor, FacetBase>& hds)
    // Create a new facet, add it to the 'hds' and return the descriptor of this new facet.
{
    typedef typename facet_gen<FacetS, HalfedgeDescriptor, FacetBase>::ContainerGen ContainerGen;
    typedef typename facet_gen<FacetS, HalfedgeDescriptor, FacetBase>::facet_type facet_type;
    
    facet_type facet_f;
    return *ContainerGen::container_add(facet_f, hds.m_container);
}

} // namespace hdstl
} // namespace boost

#endif
