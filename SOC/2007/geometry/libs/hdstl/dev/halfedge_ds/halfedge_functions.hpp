//halfedge_functions.hpp   -*- C++ -*-
//
//@PURPOSE: Provide free functions for accessing and manipulating halfedges.
//
//@DESCRIPTION: This file provides free functions for accessing and
// manipulating halfedges. The list of these functions are as follows:
//
//  - halfedges_begin: returns an iterator to the first halfedge in the 'hds' 
//  -   halfedges_end: returns an iterator after the last halfedge in the 'hds'
//  -   num_halfedges: returns the number of halfedges in the 'hds'
//  -        opposite: returns the opposite of the given halfedge in the 'hds'
//  -   next_in_facet: returns a halfedge descriptor to the halfedge succeeding 
//                     the given halfedge in the adjacent facet cycle, when facet 
//                     cycles are oriented in counter-clockwise order.
//  -  next_at_source: returns a halfedge descriptor to the halfedge succeeding 
//                     the given halfedge around the source vertex, when halfedges 
//                     are ordered around a given vertex in clockwise order
//  -  next_at_target: returns a halfedge descriptor to the halfedge succeeding 
//                     the given halfedge around the target vertex, when halfedges 
//                     are ordered around a given vertex in clockwise order
//  -   prev_in_facet: returns a halfedge descriptor to the halfedge preceeding 
//                     the given halfedge in the adjacent facet cycle, when facet 
//                     cycles are oriented in counter-clockwise order.
//  -  prev_at_source: returns a halfedge descriptor to the halfedge preceeding 
//                     the given halfedge around the source vertex, when halfedges 
//                     are ordered around a given vertex in clockwise order
//  -  prev_at_target: returns a halfedge descriptor to the halfedge preceeding 
//                     the given halfedge around the target vertex, when halfedges 
//                     are ordered around a given vertex in clockwise order
//  - set_next_in_facet: sets 'g' as the halfedge succeeding 'h' in 
//                     the adjacent facet cycle, when facet cycles are oriented in 
//                     counter-clockwise order.
//  - set_next_at_source: sets 'g' as the halfedge succeeding 'h' 
//                     around the source vertex of 'h', when halfedges are ordered 
//                     around a given vertex in clockwise order.
//  - set_next_at_target: sets 'g' as the halfedge succeeding 'h' 
//                     around the target vertex of 'h', when halfedges are ordered 
//                     around a given vertex in clockwise order.
//  - set_prev_in_facet: sets 'g' as the halfedge preceeding 'h' in 
//                     the adjacent facet cycle, when facet cycles are oriented in 
//                     counter-clockwise order.
//  - set_prev_at_source: sets 'g' as the halfedge preceeding 'h' 
//                     around the source vertex of 'h', when halfedges are ordered 
//                     around a given vertex in clockwise order.
//  - set_prev_at_target: sets 'g' as the halfedge preceeding 'h' 
//                     around the target vertex of 'h', when halfedges are ordered 
//                     around a given vertex in clockwise order.
//  -        add_edge: creates two halfedges and adds them to 'hds' as an opposite
//                     pair
//
//@SEE_ALSO: {hds_concepts.hpp, container_selectors.hpp, halfedge_selectors.hpp}

#ifndef BOOST_HDSTL_HALFEDGE_FUNCTIONS_HPP
#define BOOST_HDSTL_HALFEDGE_FUNCTIONS_HPP 1

#include <boost/hdstl/hds_traits.hpp>
#include <boost/hdstl/halfedge_ds/halfedge_selectors.hpp>

namespace boost {
namespace hdstl {

// FREE FUNCTIONS
template <typename HalfedgeS, typename VertexDescriptor, typename FacetDescriptor, typename Config>
typename halfedge_gen<HalfedgeS, VertexDescriptor, FacetDescriptor, Config>::halfedge_iterator
halfedges_begin(halfedge_gen<HalfedgeS,  VertexDescriptor, FacetDescriptor, Config>& hds)
    // Returns a halfedge iterator to the beginning of the container member of 'hds'. The value type
    // of the halfedge iterator is a halfedge descriptor.
{
    typedef typename halfedge_gen<HalfedgeS, VertexDescriptor,FacetDescriptor, Config>
                ::ContainerGen ContainerGen;
    return ContainerGen::container_begin(hds.m_container);
}

template <typename HalfedgeS,  typename VertexDescriptor, typename FacetDescriptor, typename Config>
typename halfedge_gen<HalfedgeS, VertexDescriptor, FacetDescriptor, Config>::halfedge_iterator
halfedges_end(halfedge_gen<HalfedgeS, VertexDescriptor, FacetDescriptor, Config>& hds)
{
    // Returns a halfedge iterator to the end of the container member of 'hds'. The value type
    // of the halfedge iterator is a halfedge descriptor. Note that 'end' here follows the STL
    // convention and returns an iterator one past the last element, instead of the last element.
    typedef typename halfedge_gen<HalfedgeS, VertexDescriptor,FacetDescriptor, Config>
                ::ContainerGen ContainerGen;
    return ContainerGen::container_end(hds.m_container);
}

template <typename HalfedgeS, typename VertexDescriptor, typename FacetDescriptor, typename Config>
typename halfedge_gen<HalfedgeS,  VertexDescriptor, FacetDescriptor, Config>::size_type
    // Returns the number of halfedges in the 'hds'. Return type is the size type specific to 
    // the container used, specified in container selectors.
num_halfedges(halfedge_gen<HalfedgeS,  VertexDescriptor, FacetDescriptor, Config>& hds)
{
    return hds.m_container.size();
}

// \cond
namespace detail {

template<typename HalfedgeS, typename HalfedgeDescriptor>
struct opposite_function_helper 
    // Helper struct for the opposite function. This is
    // the general case where halfedges are stored in 
    // any container except a 'vector', so halfedge
    // descriptors can be used as pointers.
{
    static
    HalfedgeDescriptor
    opposite(HalfedgeDescriptor h)
    {
        return static_cast<HalfedgeDescriptor>(h->m_opposite);
    }
};

template<typename HalfedgeDescriptor, typename TraversalS>
struct opposite_function_helper<halfedgeS<vecS,TraversalS>, HalfedgeDescriptor> 
    // This is the specialization of the helper for 'vector'. 
    // In this case no opposite member is used to access the
    // opposite halfedge. Halfedges having even numbered descriptors
    // select the next halfedge in the vector as opposite, and 
    // halfedges with odd numbered descriptors select the previous
    // halfedge in the vector as opposite. Note that halfedge
    // descriptors start from '0'.
{
    static
    HalfedgeDescriptor
    opposite(HalfedgeDescriptor h)
    {
        return h^1;
    }
};

} // end namespace detail
// \endcond

template <typename HalfedgeS,  typename VertexDescriptor, typename FacetDescriptor, typename Config>
typename Config::halfedge_descriptor
opposite(typename halfedge_gen<HalfedgeS, VertexDescriptor,FacetDescriptor, Config>::halfedge_descriptor h,
         const halfedge_gen<HalfedgeS, VertexDescriptor, FacetDescriptor, Config>&)
    // Return the halfedge descriptor of the opposite of halfedge 'h' in 'hds'. The opposite is
    // determined based on the helper functions, which are specialized based on container type.
{
    typedef typename halfedge_gen<HalfedgeS, 
                                  VertexDescriptor,FacetDescriptor, Config
                                 >::halfedge_selector halfedge_selector;
    typedef typename halfedge_gen<HalfedgeS, 
                                  VertexDescriptor,FacetDescriptor, Config
                                 >::halfedge_descriptor halfedge_descriptor;
    return detail::opposite_function_helper<halfedge_selector,
                                    halfedge_descriptor
                                   >::opposite(h);
}

// \cond
namespace detail {

template<typename HalfedgeGen, typename HalfedgeDescriptor, typename Tag = next_in_facet_tag>
struct next_in_facet_helper
    // Helper struct for returning next_in_facet link of 'h'. The selection is based on 
    // the Tag being passed in the function, which in this case is next_in_facet_tag, so
    // simply return the m_next member of 'h'.
{
    static
    HalfedgeDescriptor
    next_in_facet(HalfedgeDescriptor h, HalfedgeGen& hds)
    {
        typedef typename HalfedgeGen::ContainerGen containerGen;
        return static_cast<HalfedgeDescriptor>(containerGen::value(h,hds.m_container).m_next);
    }
};

template<typename HalfedgeGen, typename HalfedgeDescriptor>
struct next_in_facet_helper<HalfedgeGen, HalfedgeDescriptor, next_at_source_tag>
    // Specialization for the next_in_facet helper. In this case the next_in_facet function
    // is called with an 'hds' which has organized its 'm_next' links as 'next_at_source'.
    // In this case, the next_in_facet of 'h' is determined by converting 'next_at_source'
    // links to 'next_in_facet' links. See 'forward_hds_concept.hpp' for more details 
    // concerning these conversions.
{
    static
    HalfedgeDescriptor
    next_in_facet(HalfedgeDescriptor h, HalfedgeGen& hds)
    {
        typedef typename HalfedgeGen::ContainerGen containerGen;
        return static_cast<HalfedgeDescriptor>(containerGen::value(
                   static_cast<HalfedgeDescriptor>(opposite(h, hds))
                         ,hds.m_container).m_next);
    }
};

template<typename HalfedgeGen, typename HalfedgeDescriptor>
struct next_in_facet_helper<HalfedgeGen, HalfedgeDescriptor, next_at_target_tag>
    // Specialization for the next_in_facet helper. In this case the next_in_facet function
    // is called with an 'hds' which has organized its 'm_next' links as 'next_at_target'.
    // In this case, the next_in_facet of 'h' is determined by converting 'next_at_target'
    // links to 'next_in_facet' links. See 'forward_hds_concept.hpp' for more details 
    // concerning these conversions.
{
    static
    HalfedgeDescriptor
    next_in_facet(HalfedgeDescriptor h, HalfedgeGen& hds)
    {
        typedef typename HalfedgeGen::ContainerGen containerGen;
        return static_cast<HalfedgeDescriptor>(opposite(
                   static_cast<HalfedgeDescriptor>(containerGen::value(h,hds.m_container).m_next)
                         , hds));
    }
};

template<typename HalfedgeGen, typename HalfedgeDescriptor, typename Tag = next_at_source_tag>
struct next_at_source_helper
    // Helper struct for returning next_in_source link of 'h'. The selection is based on 
    // the Tag being passed in the function, which in this case is next_in_source_tag, so
    // simply return the m_next member of 'h'.
{
    static
    HalfedgeDescriptor
    next_at_source(HalfedgeDescriptor h, HalfedgeGen& hds)
    {
        typedef typename HalfedgeGen::ContainerGen containerGen;
        return static_cast<HalfedgeDescriptor>(containerGen::value(h,hds.m_container).m_next);
    }
};

template<typename HalfedgeGen, typename HalfedgeDescriptor>
struct next_at_source_helper<HalfedgeGen, HalfedgeDescriptor, next_in_facet_tag>
    // Specialization for the next_at_source helper. In this case the next_at_source function
    // is called with an 'hds' which has organized its 'm_next' links as 'next_in_facet'.
    // In this case, the next_at_source of 'h' is determined by converting 'next_in_facet'
    // links to 'next_at_source' links. See 'forward_hds_concept.hpp' for more details 
    // concerning these conversions.
{
    static
    HalfedgeDescriptor
    next_at_source(HalfedgeDescriptor h, HalfedgeGen& hds)
    {
        typedef typename HalfedgeGen::ContainerGen containerGen;
        return static_cast<HalfedgeDescriptor>(containerGen::value(
                   static_cast<HalfedgeDescriptor>(opposite(h, hds))
                         ,hds.m_container).m_next);
    }
};

template<typename HalfedgeGen, typename HalfedgeDescriptor>
struct next_at_source_helper<HalfedgeGen, HalfedgeDescriptor, next_at_target_tag>
    // Specialization for the next_at_source helper. In this case the next_at_source function
    // is called with an 'hds' which has organized its 'm_next' links as 'next_at_target'.
    // In this case, the next_at_source of 'h' is determined by converting 'next_at_target'
    // links to 'next_at_source' links. See 'forward_hds_concept.hpp' for more details 
    // concerning these conversions.
{
    static
    HalfedgeDescriptor
    next_at_source(HalfedgeDescriptor h, HalfedgeGen& hds)
    {
        typedef typename HalfedgeGen::ContainerGen containerGen;
        return static_cast<HalfedgeDescriptor>(opposite(
                    static_cast<HalfedgeDescriptor>(containerGen::value(
                        static_cast<HalfedgeDescriptor>(opposite(h, hds))
                            ,hds.m_container).m_next)
                                , hds));
    }
};

template<typename HalfedgeGen, typename HalfedgeDescriptor, typename Tag = next_at_target_tag>
struct next_at_target_helper
    // Helper struct for returning next_at_target link of 'h'. The selection is based on 
    // the Tag being passed in the function, which in this case is next_at_target_tag, so
    // simply return the m_next member of 'h'.
{
    static
    HalfedgeDescriptor
    next_at_target(HalfedgeDescriptor h, HalfedgeGen& hds)
    {
        typedef typename HalfedgeGen::ContainerGen containerGen;
        return static_cast<HalfedgeDescriptor>(containerGen::value(h,hds.m_container).m_next);
    }
};

template<typename HalfedgeGen, typename HalfedgeDescriptor>
struct next_at_target_helper<HalfedgeGen, HalfedgeDescriptor, next_in_facet_tag>
    // Specialization for the next_at_target helper. In this case the next_at_target function
    // is called with an 'hds' which has organized its 'm_next' links as 'next_in_facet'.
    // In this case, the next_at_target of 'h' is determined by converting 'next_in_facet'
    // links to 'next_at_target' links. See 'forward_hds_concept.hpp' for more details 
    // concerning these conversions.
{
    static
    HalfedgeDescriptor
    next_at_target(HalfedgeDescriptor h, HalfedgeGen& hds)
    {
        typedef typename HalfedgeGen::ContainerGen containerGen;
        return static_cast<HalfedgeDescriptor>(opposite(
                   static_cast<HalfedgeDescriptor>(containerGen::value(h,hds.m_container).m_next)
                         , hds));
    }
};

template<typename HalfedgeGen, typename HalfedgeDescriptor>
struct next_at_target_helper<HalfedgeGen, HalfedgeDescriptor, next_at_source_tag>
    // Specialization for the next_at_target helper. In this case the next_at_target function
    // is called with an 'hds' which has organized its 'm_next' links as 'next_at_source'.
    // In this case, the next_at_target of 'h' is determined by converting 'next_at_source'
    // links to 'next_at_target' links. See 'forward_hds_concept.hpp' for more details 
    // concerning these conversions.
{
    static
    HalfedgeDescriptor
    next_at_target(HalfedgeDescriptor h, HalfedgeGen& hds)
    {
        typedef typename HalfedgeGen::ContainerGen containerGen;
        return static_cast<HalfedgeDescriptor>(opposite(
                    static_cast<HalfedgeDescriptor>(containerGen::value(
                        static_cast<HalfedgeDescriptor>(opposite(h, hds))
                            ,hds.m_container).m_next)
                                , hds));
    }
};

} // end namespace detail
// \endcond

template <typename HalfedgeS, typename VertexDescriptor, typename FacetDescriptor, typename Config>
typename Config::halfedge_descriptor
next_in_facet(typename halfedge_gen<HalfedgeS, VertexDescriptor,FacetDescriptor, Config>::halfedge_descriptor h,
               halfedge_gen<HalfedgeS, VertexDescriptor, FacetDescriptor, Config>& hds)
    // returns a halfedge descriptor to the halfedge succeeding the given halfedge in the adjacent facet cycle, when
    // facet cycles are oriented in counter-clockwise order. Uses the 'HalfedgeS::next_tag' to determine
    // the primary way the next links are set, and does conversion using the helper classes if necessary.
{
    typedef typename halfedge_gen<HalfedgeS, 
                                  VertexDescriptor,FacetDescriptor, Config
                                 >::halfedge_descriptor halfedge_descriptor;
    typedef  halfedge_gen<HalfedgeS, 
                                  VertexDescriptor,FacetDescriptor, Config> halfedgeGen;

    return detail::next_in_facet_helper<halfedgeGen, halfedge_descriptor, typename HalfedgeS::next_tag
                                       >::next_in_facet(h, hds);
}

template <typename HalfedgeS, typename VertexDescriptor, typename FacetDescriptor, typename Config>
typename Config::halfedge_descriptor
next_at_source(typename halfedge_gen<HalfedgeS, VertexDescriptor,FacetDescriptor, Config>::halfedge_descriptor h,
               halfedge_gen<HalfedgeS, VertexDescriptor, FacetDescriptor, Config>& hds)
    // returns a halfedge descriptor to the halfedge succeeding the given halfedge around the source vertex, when
    // halfedges are ordered around a given vertex in clockwise order. Uses the 'HalfedgeS::next_tag' to determine
    // the primary way the next links are set, and does conversion using the helper classes if necessary.
{
    typedef typename halfedge_gen<HalfedgeS, 
                                  VertexDescriptor,FacetDescriptor, Config
                                 >::halfedge_descriptor halfedge_descriptor;
    typedef  halfedge_gen<HalfedgeS, 
                                  VertexDescriptor,FacetDescriptor, Config> halfedgeGen;

    return detail::next_at_source_helper<halfedgeGen, halfedge_descriptor, typename HalfedgeS::next_tag
                                        >::next_at_source(h,hds);
}

template <typename HalfedgeS, typename VertexDescriptor, typename FacetDescriptor, typename Config>
typename Config::halfedge_descriptor
next_at_target(typename halfedge_gen<HalfedgeS, VertexDescriptor,FacetDescriptor, Config>::halfedge_descriptor h,
               halfedge_gen<HalfedgeS, VertexDescriptor, FacetDescriptor, Config>& hds)
    // returns a halfedge descriptor to the halfedge succeeding the given halfedge around the target vertex, when
    // halfedges are ordered around a given vertex in clockwise order. Uses the 'HalfedgeS::next_tag' to determine
    // the primary way the next links are set, and does conversion using the helper classes if necessary.
{
    typedef typename halfedge_gen<HalfedgeS, 
                                  VertexDescriptor,FacetDescriptor, Config
                                 >::halfedge_descriptor halfedge_descriptor;
    typedef  halfedge_gen<HalfedgeS, 
                                  VertexDescriptor,FacetDescriptor, Config> halfedgeGen;

    return detail::next_at_target_helper<halfedgeGen, halfedge_descriptor, typename HalfedgeS::next_tag
                                        >::next_at_target(h,hds);
}

// \cond
namespace detail {

template<typename HalfedgeGen, typename HalfedgeDescriptor, typename Tag = prev_in_facet_tag>
struct prev_in_facet_helper
    // Helper struct for returning prev_in_facet link of 'h'. The selection is based on 
    // the Tag being passed in the function, which in this case is prev_in_facet_tag, so
    // simply return the m_prev member of 'h'.
{
    static
    HalfedgeDescriptor
    prev_in_facet(HalfedgeDescriptor h, HalfedgeGen& hds)
    {
        typedef typename HalfedgeGen::ContainerGen containerGen;
        return static_cast<HalfedgeDescriptor>(containerGen::value(h,hds.m_container).m_prev);
    }
};

template<typename HalfedgeGen, typename HalfedgeDescriptor>
struct prev_in_facet_helper<HalfedgeGen, HalfedgeDescriptor, prev_at_source_tag>
    // Specialization for the prev_in_facet helper. In this case the prev_in_facet function
    // is called with an 'hds' which has organized its 'm_prev' links as 'prev_at_source'.
    // In this case, the prev_in_facet of 'h' is determined by converting 'prev_at_source'
    // links to 'prev_in_facet' links. See 'backward_hds_concept.hpp' for more details 
    // concerning these conversions.
{
    static
    HalfedgeDescriptor
    prev_in_facet(HalfedgeDescriptor h, HalfedgeGen& hds)
    {
        typedef typename HalfedgeGen::ContainerGen containerGen;
        return static_cast<HalfedgeDescriptor>(opposite(
                   static_cast<HalfedgeDescriptor>(containerGen::value(h,hds.m_container).m_prev)
                         , hds));
    }
};

template<typename HalfedgeGen, typename HalfedgeDescriptor>
struct prev_in_facet_helper<HalfedgeGen, HalfedgeDescriptor, prev_at_target_tag>
    // Specialization for the prev_in_facet helper. In this case the prev_in_facet function
    // is called with an 'hds' which has organized its 'm_prev' links as 'prev_at_target'.
    // In this case, the prev_in_facet of 'h' is determined by converting 'prev_at_target'
    // links to 'prev_in_facet' links. See 'backward_hds_concept.hpp' for more details 
    // concerning these conversions.
{
    static
    HalfedgeDescriptor
    prev_in_facet(HalfedgeDescriptor h, HalfedgeGen& hds)
    {
        typedef typename HalfedgeGen::ContainerGen containerGen;
        return static_cast<HalfedgeDescriptor>(containerGen::value(
                   static_cast<HalfedgeDescriptor>(opposite(h, hds))
                         ,hds.m_container).m_prev);
    }
};

template<typename HalfedgeGen, typename HalfedgeDescriptor, typename Tag = prev_at_source_tag>
struct prev_at_source_helper
    // Helper struct for returning prev_in_source link of 'h'. The selection is based on 
    // the Tag being passed in the function, which in this case is prev_in_source_tag, so
    // simply return the m_prev member of 'h'.
{
    static
    HalfedgeDescriptor
    prev_at_source(HalfedgeDescriptor h, HalfedgeGen& hds)
    {
        typedef typename HalfedgeGen::ContainerGen containerGen;
        return static_cast<HalfedgeDescriptor>(containerGen::value(h,hds.m_container).m_prev);
    }
};

template<typename HalfedgeGen, typename HalfedgeDescriptor>
struct prev_at_source_helper<HalfedgeGen, HalfedgeDescriptor, prev_in_facet_tag>
    // Specialization for the prev_in_source helper. In this case the prev_in_source function
    // is called with an 'hds' which has organized its 'm_prev' links as 'prev_at_facet'.
    // In this case, the prev_in_source of 'h' is determined by converting 'prev_at_facet'
    // links to 'prev_in_source' links. See 'backward_hds_concept.hpp' for more details 
    // concerning these conversions.
{
    static
    HalfedgeDescriptor
    prev_at_source(HalfedgeDescriptor h, HalfedgeGen& hds)
    {
        typedef typename HalfedgeGen::ContainerGen containerGen;
        return static_cast<HalfedgeDescriptor>(opposite(
                   static_cast<HalfedgeDescriptor>(containerGen::value(h,hds.m_container).m_prev)
                         , hds));
    }
};

template<typename HalfedgeGen, typename HalfedgeDescriptor>
struct prev_at_source_helper<HalfedgeGen, HalfedgeDescriptor, prev_at_target_tag>
    // Specialization for the prev_in_source helper. In this case the prev_in_source function
    // is called with an 'hds' which has organized its 'm_prev' links as 'prev_at_target'.
    // In this case, the prev_in_source of 'h' is determined by converting 'prev_at_target'
    // links to 'prev_in_source' links. See 'backward_hds_concept.hpp' for more details 
    // concerning these conversions.
{
    static
    HalfedgeDescriptor
    prev_at_source(HalfedgeDescriptor h, HalfedgeGen& hds)
    {
        typedef typename HalfedgeGen::ContainerGen containerGen;
        return static_cast<HalfedgeDescriptor>(opposite(
                    static_cast<HalfedgeDescriptor>(containerGen::value(
                        static_cast<HalfedgeDescriptor>(opposite(h, hds))
                            ,hds.m_container).m_prev)
                                , hds));
    }
};

template<typename HalfedgeGen, typename HalfedgeDescriptor, typename Tag = prev_at_target_tag>
struct prev_at_target_helper
    // Helper struct for returning prev_in_target link of 'h'. The selection is based on 
    // the Tag being passed in the function, which in this case is prev_in_target_tag, so
    // simply return the m_prev member of 'h'.
{
    static
    HalfedgeDescriptor
    prev_at_target(HalfedgeDescriptor h, HalfedgeGen& hds)
    {
        typedef typename HalfedgeGen::ContainerGen containerGen;
        return static_cast<HalfedgeDescriptor>(containerGen::value(h,hds.m_container).m_prev);
    }
};

template<typename HalfedgeGen, typename HalfedgeDescriptor>
struct prev_at_target_helper<HalfedgeGen, HalfedgeDescriptor, prev_in_facet_tag>
    // Specialization for the prev_in_target helper. In this case the prev_in_target function
    // is called with an 'hds' which has organized its 'm_prev' links as 'prev_at_facet'.
    // In this case, the prev_in_target of 'h' is determined by converting 'prev_at_facet'
    // links to 'prev_in_target' links. See 'backward_hds_concept.hpp' for more details 
    // concerning these conversions.
{
    static
    HalfedgeDescriptor
    prev_at_target(HalfedgeDescriptor h, HalfedgeGen& hds)
    {
        typedef typename HalfedgeGen::ContainerGen containerGen;
        return static_cast<HalfedgeDescriptor>(containerGen::value(
                   static_cast<HalfedgeDescriptor>(opposite(h, hds))
                         ,hds.m_container).m_prev);
    }
};

template<typename HalfedgeGen, typename HalfedgeDescriptor>
struct prev_at_target_helper<HalfedgeGen, HalfedgeDescriptor, prev_at_source_tag>
    // Specialization for the prev_in_target helper. In this case the prev_in_target function
    // is called with an 'hds' which has organized its 'm_prev' links as 'prev_at_source'.
    // In this case, the prev_in_target of 'h' is determined by converting 'prev_at_source'
    // links to 'prev_in_target' links. See 'backward_hds_concept.hpp' for more details 
    // concerning these conversions.
{
    static
    HalfedgeDescriptor
    prev_at_target(HalfedgeDescriptor h, HalfedgeGen& hds)
    {
        typedef typename HalfedgeGen::ContainerGen containerGen;
        return static_cast<HalfedgeDescriptor>(opposite(
                    static_cast<HalfedgeDescriptor>(containerGen::value(
                        static_cast<HalfedgeDescriptor>(opposite(h, hds))
                            ,hds.m_container).m_prev)
                                , hds));
    }
};

} // end namespace detail
// \endcond

template <typename HalfedgeS, typename VertexDescriptor, typename FacetDescriptor, typename Config>
typename Config::halfedge_descriptor
prev_in_facet(typename halfedge_gen<HalfedgeS, VertexDescriptor,FacetDescriptor, Config>::halfedge_descriptor h,
               halfedge_gen<HalfedgeS, VertexDescriptor, FacetDescriptor, Config>& hds)
    // returns a halfedge descriptor to the halfedge preceeding the given halfedge in the adjacent facet cycle, when
    // facet cycles are oriented in counter-clockwise order. Uses the 'HalfedgeS::prev_tag' to determine
    // the primary way the prev links are set, and does conversion using the helper classes if necessary.
{
    typedef typename halfedge_gen<HalfedgeS, 
                                  VertexDescriptor,FacetDescriptor, Config
                                 >::halfedge_descriptor halfedge_descriptor;
    typedef  halfedge_gen<HalfedgeS, 
                                  VertexDescriptor,FacetDescriptor, Config> halfedgeGen;

    return detail::prev_in_facet_helper<halfedgeGen, halfedge_descriptor, typename HalfedgeS::prev_tag
                                       >::prev_in_facet(h, hds);
}

template <typename HalfedgeS, typename VertexDescriptor, typename FacetDescriptor, typename Config>
typename Config::halfedge_descriptor
prev_at_source(typename halfedge_gen<HalfedgeS, VertexDescriptor,FacetDescriptor, Config>::halfedge_descriptor h,
               halfedge_gen<HalfedgeS, VertexDescriptor, FacetDescriptor, Config>& hds)
    // returns a halfedge descriptor to the halfedge preceeding the given halfedge around the source vertex, when
    // halfedges are ordered around a given vertex in clockwise order. Uses the 'HalfedgeS::prev_tag' to determine
    // the primary way the prev links are set, and does conversion using the helper classes if necessary.
{
    typedef typename halfedge_gen<HalfedgeS, 
                                  VertexDescriptor,FacetDescriptor, Config
                                 >::halfedge_descriptor halfedge_descriptor;
    typedef  halfedge_gen<HalfedgeS, 
                                  VertexDescriptor,FacetDescriptor, Config> halfedgeGen;

    return detail::prev_at_source_helper<halfedgeGen, halfedge_descriptor, typename HalfedgeS::prev_tag
                                        >::prev_at_source(h,hds);
}

template <typename HalfedgeS, typename VertexDescriptor, typename FacetDescriptor, typename Config>
typename Config::halfedge_descriptor
prev_at_target(typename halfedge_gen<HalfedgeS, VertexDescriptor,FacetDescriptor, Config>::halfedge_descriptor h,
               halfedge_gen<HalfedgeS, VertexDescriptor, FacetDescriptor, Config>& hds)
    // returns a halfedge descriptor to the halfedge preceeding the given halfedge around the target vertex, when
    // halfedges are ordered around a given vertex in clockwise order. Uses the 'HalfedgeS::prev_tag' to determine
    // the primary way the prev links are set, and does conversion using the helper classes if necessary.
{
    typedef typename halfedge_gen<HalfedgeS, 
                                  VertexDescriptor,FacetDescriptor, Config
                                 >::halfedge_descriptor halfedge_descriptor;
    typedef  halfedge_gen<HalfedgeS, 
                                  VertexDescriptor,FacetDescriptor, Config> halfedgeGen;

    return detail::prev_at_target_helper<halfedgeGen, halfedge_descriptor, typename HalfedgeS::prev_tag
                                        >::prev_at_target(h,hds);
}

// \cond
namespace detail {

template<typename HalfedgeGen, typename HalfedgeDescriptor, typename ContainerS, typename ThisTag, typename Tag>
struct set_next_helper
    // Helper struct to set the next link. In this case the 'ThisTag' which represents how the next 
    // links are assigned in the 'hds', and 'Tag' which represents what modifier method is called, do not
    // match, so a compile time error should follow. E.g. if the data structure points to next halfedge in the facet
    // cycle as the prefered next member, trying to modify the dataset with 'set_next_at_source' or 
    // 'set_next_at_target' methods should cause an error.
{
    static void
    set_next(HalfedgeDescriptor, HalfedgeDescriptor, HalfedgeGen&)
    {
        // ThisTag and Tag do not match, 
        // What you are trying to do is  setting the next tag 
        // with a different function than it is supposed to 
        // be used.
        // Compile time error should follow.
    }
};

template<typename HalfedgeGen, typename HalfedgeDescriptor, typename Tag>
struct set_next_helper<HalfedgeGen, HalfedgeDescriptor, vecS, Tag, Tag>
    // Helper struct to set the next link. This is a specialization for the vector. 
{
    static void
    set_next(HalfedgeDescriptor h, HalfedgeDescriptor g, HalfedgeGen& hds)
    {
        hds.m_container[h].m_next = g;
    }
};

template<typename HalfedgeGen, typename HalfedgeDescriptor, typename ContainerS, typename Tag>
struct set_next_helper<HalfedgeGen, HalfedgeDescriptor, ContainerS, Tag, Tag>
    // Helper struct to set the next link. This is a specialization for the containers other than the vector. 
{
    static void
    set_next(HalfedgeDescriptor h, HalfedgeDescriptor g, HalfedgeGen& hds)
    {
        (void) hds;
        h->m_next = g;
    }
};

} // end namespace detail
// \endcond

template <typename HalfedgeS, typename VertexDescriptor, typename FacetDescriptor, typename Config>
void
set_next_in_facet(typename halfedge_gen<HalfedgeS, VertexDescriptor,FacetDescriptor, Config>::halfedge_descriptor h,
                  typename halfedge_gen<HalfedgeS, VertexDescriptor,FacetDescriptor, Config>::halfedge_descriptor g,
                  halfedge_gen<HalfedgeS, VertexDescriptor, FacetDescriptor, Config>& hds)
    // sets 'g' as the halfedge succeeding 'h' in the adjacent facet cycle, when facet cycles are oriented in
    // counter-clockwise order. The helper functions do selections based on containerS and the 'Halfedge::next_tag'.
    // Also the 'this_tag' and the 'Halfedge::next_tag' should match for properly setting the next link. Otherwise
    // compile time error will follow.
{
    typedef typename halfedge_gen<HalfedgeS,
                                  VertexDescriptor,FacetDescriptor, Config
                                 >::halfedge_descriptor halfedge_descriptor;
    typedef halfedge_gen<HalfedgeS, VertexDescriptor, FacetDescriptor, Config> halfedgeGen;

    typedef typename halfedgeGen::halfedge_selector::container_selector containerS;

    typedef next_in_facet_tag this_tag;

    detail::set_next_helper<halfedgeGen, halfedge_descriptor, containerS, this_tag, typename HalfedgeS::next_tag>
                    ::set_next(h, g, hds);
}

template <typename HalfedgeS, typename VertexDescriptor, typename FacetDescriptor, typename Config>
void
set_next_at_source(typename halfedge_gen<HalfedgeS, VertexDescriptor,FacetDescriptor, Config>::halfedge_descriptor h,
                  typename halfedge_gen<HalfedgeS, VertexDescriptor,FacetDescriptor, Config>::halfedge_descriptor g,
                  halfedge_gen<HalfedgeS, VertexDescriptor, FacetDescriptor, Config>& hds)
    // sets 'g' as the halfedge succeeding 'h' around the source vertex of 'h', when halfedges are ordered around a
    // given vertex in clockwise order. The helper functions do selections based on containerS and the
    // 'Halfedge::next_tag'.  Also the 'this_tag' and the 'Halfedge::next_tag' should match for properly setting the
    // next link. Otherwise compile time error will follow.
{
    typedef typename halfedge_gen<HalfedgeS,
                                  VertexDescriptor,FacetDescriptor, Config
                                 >::halfedge_descriptor halfedge_descriptor;
    typedef halfedge_gen<HalfedgeS, VertexDescriptor, FacetDescriptor, Config> halfedgeGen;

    typedef typename halfedgeGen::halfedge_selector::container_selector containerS;

    typedef next_at_source_tag this_tag;

    detail::set_next_helper<halfedgeGen, halfedge_descriptor, containerS, this_tag, typename HalfedgeS::next_tag>
                    ::set_next(h, g, hds);
}

template <typename HalfedgeS, typename VertexDescriptor, typename FacetDescriptor, typename Config>
void
set_next_at_target(typename halfedge_gen<HalfedgeS, VertexDescriptor,FacetDescriptor, Config>::halfedge_descriptor h,
                  typename halfedge_gen<HalfedgeS, VertexDescriptor,FacetDescriptor, Config>::halfedge_descriptor g,
                  halfedge_gen<HalfedgeS, VertexDescriptor, FacetDescriptor, Config>& hds)
    // sets 'g' as the halfedge succeeding 'h' around the target vertex of 'h', when halfedges are ordered around a
    // given vertex in clockwise order. The helper functions do selections based on containerS and the
    // 'Halfedge::next_tag'.  Also the 'this_tag' and the 'Halfedge::next_tag' should match for properly setting the
    // next link. Otherwise compile time error will follow.
{
    typedef typename halfedge_gen<HalfedgeS,
                                  VertexDescriptor,FacetDescriptor, Config
                                 >::halfedge_descriptor halfedge_descriptor;
    typedef halfedge_gen<HalfedgeS, VertexDescriptor, FacetDescriptor, Config> halfedgeGen;

    typedef typename halfedgeGen::halfedge_selector::container_selector containerS;

    typedef next_at_target_tag this_tag;

    detail::set_next_helper<halfedgeGen, halfedge_descriptor, containerS, this_tag, typename HalfedgeS::next_tag>
                    ::set_next(h, g, hds);
}

// \cond
namespace detail {

template<typename HalfedgeGen, typename HalfedgeDescriptor, typename ContainerS, typename ThisTag, typename Tag>
struct set_prev_helper
    // Helper struct to set the prev link. In this case the 'ThisTag' which represents how the prev 
    // links are assigned in the 'hds', and 'Tag' which represents what modifier method is called, do not
    // match, so a compile time error should follow. E.g. if the data structure points to prev halfedge in the facet
    // cycle as the prefered prev member, trying to modify the dataset with 'set_prev_at_source' or 
    // 'set_prev_at_target' methods should cause an error.
{
    static void
    set_prev(HalfedgeDescriptor h, HalfedgeDescriptor g, HalfedgeGen& hds)
    {
        // ThisTag and Tag do not match, 
        // What you are trying to do is  setting the prev tag 
        // with a different function than it is supposed to 
        // be used.
        // Compile time error should follow.
    }
};

template<typename HalfedgeGen, typename HalfedgeDescriptor, typename Tag>
struct set_prev_helper<HalfedgeGen, HalfedgeDescriptor, vecS, Tag, Tag>
    // Helper struct to set the prev link. This is a specialization for the vector. 
{
    static void
    set_prev(HalfedgeDescriptor h, HalfedgeDescriptor g, HalfedgeGen& hds)
    {
        hds.m_container[h].m_prev = g;
    }
};

template<typename HalfedgeGen, typename HalfedgeDescriptor, typename ContainerS, typename Tag>
struct set_prev_helper<HalfedgeGen, HalfedgeDescriptor, ContainerS, Tag, Tag>
    // Helper struct to set the prev link. This is a specialization for the containers other than the vector. 
{
    static void
    set_prev(HalfedgeDescriptor h, HalfedgeDescriptor g, HalfedgeGen& hds)
    {
        (void) hds;
        h->m_prev = g;
    }
};

} // end namespace detail
// \endcond

template <typename HalfedgeS, typename VertexDescriptor, typename FacetDescriptor, typename Config>
void
set_prev_in_facet(typename halfedge_gen<HalfedgeS, VertexDescriptor,FacetDescriptor, Config>::halfedge_descriptor h,
                  typename halfedge_gen<HalfedgeS, VertexDescriptor,FacetDescriptor, Config>::halfedge_descriptor g,
                  halfedge_gen<HalfedgeS, VertexDescriptor, FacetDescriptor, Config>& hds)
    // sets 'g' as the halfedge preceeding 'h' in the adjacent facet cycle, when facet cycles are oriented in
    // counter-clockwise order. The helper functions do selections based on containerS and the 'Halfedge::prev_tag'.
    // Also the 'this_tag' and the 'Halfedge::prev_tag' should match for properly setting the prev link. Otherwise
    // compile time error will follow.
{
    typedef typename halfedge_gen<HalfedgeS,
                                  VertexDescriptor,FacetDescriptor, Config
                                 >::halfedge_descriptor halfedge_descriptor;
    typedef halfedge_gen<HalfedgeS, VertexDescriptor, FacetDescriptor, Config> halfedgeGen;

    typedef typename halfedgeGen::halfedge_selector::container_selector containerS;

    typedef prev_in_facet_tag this_tag;

    detail::set_prev_helper<halfedgeGen, halfedge_descriptor, containerS, this_tag, typename HalfedgeS::prev_tag>
                    ::set_prev(h, g, hds);
}

template <typename HalfedgeS, typename VertexDescriptor, typename FacetDescriptor, typename Config>
void
set_prev_at_source(typename halfedge_gen<HalfedgeS, VertexDescriptor,FacetDescriptor, Config>::halfedge_descriptor h,
                  typename halfedge_gen<HalfedgeS, VertexDescriptor,FacetDescriptor, Config>::halfedge_descriptor g,
                  halfedge_gen<HalfedgeS, VertexDescriptor, FacetDescriptor, Config>& hds)
    // sets 'g' as the halfedge preceeding 'h' around the source vertex of 'h', when halfedges are ordered around a
    // given vertex in clockwise order. The helper functions do selections based on containerS and the
    // 'Halfedge::prev_tag'.  Also the 'this_tag' and the 'Halfedge::prev_tag' should match for properly setting the
    // prev link. Otherwise compile time error will follow.
{
    typedef typename halfedge_gen<HalfedgeS,
                                  VertexDescriptor,FacetDescriptor, Config
                                 >::halfedge_descriptor halfedge_descriptor;
    typedef halfedge_gen<HalfedgeS, VertexDescriptor, FacetDescriptor, Config> halfedgeGen;

    typedef typename halfedgeGen::halfedge_selector::container_selector containerS;

    typedef prev_at_source_tag this_tag;

    detail::set_prev_helper<halfedgeGen, halfedge_descriptor, containerS, this_tag, typename HalfedgeS::prev_tag>
                    ::set_prev(h, g, hds);
}

template <typename HalfedgeS, typename VertexDescriptor, typename FacetDescriptor, typename Config>
void
set_prev_at_target(typename halfedge_gen<HalfedgeS, VertexDescriptor,FacetDescriptor, Config>::halfedge_descriptor h,
                  typename halfedge_gen<HalfedgeS, VertexDescriptor,FacetDescriptor, Config>::halfedge_descriptor g,
                  halfedge_gen<HalfedgeS, VertexDescriptor, FacetDescriptor, Config>& hds)
    // sets 'g' as the halfedge preceeding 'h' around the target vertex of 'h', when halfedges are ordered around a
    // given vertex in clockwise order. The helper functions do selections based on containerS and the
    // 'Halfedge::prev_tag'.  Also the 'this_tag' and the 'Halfedge::prev_tag' should match for properly setting the
    // prev link. Otherwise compile time error will follow.
{
    typedef typename halfedge_gen<HalfedgeS,
                                  VertexDescriptor,FacetDescriptor, Config
                                 >::halfedge_descriptor halfedge_descriptor;
    typedef halfedge_gen<HalfedgeS, VertexDescriptor, FacetDescriptor, Config> halfedgeGen;

    typedef typename halfedgeGen::halfedge_selector::container_selector containerS;

    typedef prev_at_target_tag this_tag;

    detail::set_prev_helper<halfedgeGen, halfedge_descriptor, containerS, this_tag, typename HalfedgeS::prev_tag>
                    ::set_prev(h, g, hds);
}

// \cond
namespace detail {

template<typename HalfedgeDescriptor, typename ContainerS>
struct set_opposite_pair_helper
   // Helper struct for setting two halfedges opposites of each other.
   // In this case, any container but vector.
{
    static void
    set_opposite(HalfedgeDescriptor& h, HalfedgeDescriptor& g)
    {
        h->m_opposite = g;
        g->m_opposite = h;
    }
};

template<typename HalfedgeDescriptor>
struct set_opposite_pair_helper<HalfedgeDescriptor, vecS> 
   // Helper struct for setting two halfedges opposites of each other.
   // In this case vector as the container in which no opposite 
   // pointers are used so nothing further to do.
{
    static void
    set_opposite(HalfedgeDescriptor& , HalfedgeDescriptor&) 
    {
        // nothing to do
    }
};

} // end namespace detail
// \endcond

template <typename HalfedgeS, typename VertexDescriptor, typename FacetDescriptor, typename Config>
typename Config::halfedge_descriptor
new_edge(halfedge_gen<HalfedgeS, VertexDescriptor, FacetDescriptor, Config>& hds)
    // Create two halfedges, add them to 'hds' and set them opposites of each other.
    // Return one of these halfedge descriptors. This method enforces halfedges created
    // in pairs.
{
    typedef halfedge_gen<HalfedgeS, VertexDescriptor, FacetDescriptor, Config> halfedgeGen;
    typedef typename halfedgeGen::halfedge_descriptor halfedge_descriptor;
    typedef typename halfedgeGen::halfedge_type halfedge_type;
    typedef typename halfedgeGen::ContainerGen ContainerGen;
    
    typedef typename halfedgeGen::halfedge_selector::container_selector containerS;
    
    halfedge_type half_h;
    halfedge_type half_g;
    halfedge_descriptor h = *ContainerGen::container_add(half_h, hds.m_container);
    halfedge_descriptor g = *ContainerGen::container_add(half_g, hds.m_container);
    detail::set_opposite_pair_helper<halfedge_descriptor, containerS>::set_opposite(h,g);
    return g;
}



} // namespace hdstl
} // namespace boost

#endif
