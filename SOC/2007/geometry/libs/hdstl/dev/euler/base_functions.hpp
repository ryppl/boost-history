//base_functions.hpp   -*- C++ -*-
//
//@PURPOSE: Provide base free functions for accessing and manipulating halfedges, vertices and facets.
//
//@DESCRIPTION: This file provides base free functions for accessing and manipulating halfedges, vertices and facets. 
// The base functions are the building blocks for the Euler operators defined in 'euler_operators.hpp'.
// The list of these functions are as follows:
//
//  -  stitch_cycle: Sets the next and prev links of the given two halfedges 'h' and 'g' based on the configuration
//                   of data structure 'hds'. 
//  - stitch_vertex: Sets the vertex 'v' as the 'source' or the 'target' vertex of halfedge 'h' and defines a vertex
//                   link from the halfedge 'h' to vertex 'v' if the vertex links are defined in 'hds'
//  -  stitch_facet: Sets the facet 'f' as the facet on the left for halfedge 'h' and defines a facet link from 
//                   the halfedge 'h' to facet 'f' if the facet links are defined in 'hds'
//
//@SEE_ALSO: {hds_concepts.hpp, halfedge_selectors.hpp, vertex_selectors.hpp, facet_selectors.hpp}


#ifndef BOOST_HDSTL_BASE_FUNCTIONS_HPP
#define BOOST_HDSTL_BASE_FUNCTIONS_HPP 1

#include <boost/hdstl/halfedge_ds/halfedge_functions.hpp>
#include <boost/hdstl/halfedge_ds/facet_functions.hpp>
#include <boost/hdstl/halfedge_ds/vertex_functions.hpp>

namespace boost {
namespace hdstl {

// \cond
namespace detail {

template<typename HalfedgeGen, typename Tag=next_in_facet_tag>
struct stitch_cycle_helper
{
    static void
    stitch(typename HalfedgeGen::halfedge_descriptor h, typename HalfedgeGen::halfedge_descriptor g, HalfedgeGen& hds)
    {
        set_next_in_facet(h,g,hds);
    }
};

template<typename HalfedgeGen>
struct stitch_cycle_helper<HalfedgeGen, prev_in_facet_tag>
{
    static void
    stitch(typename HalfedgeGen::halfedge_descriptor h, typename HalfedgeGen::halfedge_descriptor g, HalfedgeGen& hds)
    {
        set_prev_in_facet(h,g,hds);
    }
};

template<typename HalfedgeGen>
struct stitch_cycle_helper<HalfedgeGen, next_at_source_tag>
{
    static void
    stitch(typename HalfedgeGen::halfedge_descriptor h, typename HalfedgeGen::halfedge_descriptor g, HalfedgeGen& hds)
    {
        set_next_at_source(h,g,hds);
    }
};

template<typename HalfedgeGen>
struct stitch_cycle_helper<HalfedgeGen, prev_at_source_tag>
{
    static void
    stitch(typename HalfedgeGen::halfedge_descriptor h, typename HalfedgeGen::halfedge_descriptor g, HalfedgeGen& hds)
    {
        set_prev_at_source(h,g,hds);
    }
};

template<typename HalfedgeGen>
struct stitch_cycle_helper<HalfedgeGen, next_at_target_tag>
{
    static void
    stitch(typename HalfedgeGen::halfedge_descriptor h, typename HalfedgeGen::halfedge_descriptor g, HalfedgeGen& hds)
    {
        set_next_at_target(h,g,hds);
    }
};

template<typename HalfedgeGen>
struct stitch_cycle_helper<HalfedgeGen, prev_at_target_tag>
{
    static void
    stitch(typename HalfedgeGen::halfedge_descriptor h, typename HalfedgeGen::halfedge_descriptor g, HalfedgeGen& hds)
    {
        set_prev_at_target(h,g,hds);
    }
};

template <typename HalfedgeGen, bool IsForward=true, bool IsBackward=false>
struct stitch_cycle_dir_helper
    // Helper for forward HDS, uses the nested 'next_tag' to identify the primary accessor and selects
    // the set functions for the appropriate accessor.
{
    static void
    stitch_cycle_dir(typename HalfedgeGen::halfedge_descriptor h, typename HalfedgeGen::halfedge_descriptor g, 
                                                                                          HalfedgeGen& hds)
    {
        stitch_cycle_helper<HalfedgeGen, typename HalfedgeGen::halfedge_selector::next_tag>::stitch(h,g,hds);
    }
};

template <typename HalfedgeGen>
struct stitch_cycle_dir_helper<HalfedgeGen, false, true>
    // Helper for backward HDS, uses the nested 'prev_tag' to identify the primary accessor and selects
    // the set functions for the appropriate accessor.
{
    static void
    stitch_cycle_dir(typename HalfedgeGen::halfedge_descriptor h, typename HalfedgeGen::halfedge_descriptor g, 
                                                                                          HalfedgeGen& hds)
    {
        stitch_cycle_helper<HalfedgeGen, typename HalfedgeGen::halfedge_selector::prev_tag>::stitch(h,g,hds);
    }
};

template <typename HalfedgeGen>
struct stitch_cycle_dir_helper<HalfedgeGen, true, true>
    // Helper for bidirectional HDS, uses the nested 'next_tag' and 'prev_tag' to identify the primary accessors 
    // and selects the set functions for the appropriate accessors.
{
    static void
    stitch_cycle_dir(typename HalfedgeGen::halfedge_descriptor h, typename HalfedgeGen::halfedge_descriptor g, 
                                                                                          HalfedgeGen& hds)
    {
        stitch_cycle_helper<HalfedgeGen, typename HalfedgeGen::halfedge_selector::next_tag>::stitch(h,g,hds);
        stitch_cycle_helper<HalfedgeGen, typename HalfedgeGen::halfedge_selector::prev_tag>::stitch(h,g,hds);
    }
};

} // end namespace detail
// \endcond

template <typename HalfedgeGen>
void
stitch_cycle(typename HalfedgeGen::halfedge_descriptor h, typename HalfedgeGen::halfedge_descriptor g, HalfedgeGen& hds)
    // Sets the appropriate accessor relations between h and g, based on the configuration of the 'hds'
{
    typedef typename HalfedgeGen::halfedge_selector halfedge_selector;
    detail::stitch_cycle_dir_helper<HalfedgeGen, halfedge_selector::is_forward,
                                                 halfedge_selector::is_backward
                                                 >::stitch_cycle_dir(h,g,hds);
}

// \cond
namespace detail {

template <typename HalfedgeGen, typename FacetGen, bool HasFacetLink=false>
struct stitch_facet_helper 
    // Helper for setting 'f' as the facet on the left of halfedge 'h', when 'fds' is configured without facet links.
{
    static void
    stitch(typename HalfedgeGen::halfedge_descriptor h, typename FacetGen::facet_descriptor f, HalfedgeGen& hds, 
                                                                                                  FacetGen& fds)
    {
        (void) fds;
        set_facet(h,f,hds);
    }
};

template <typename HalfedgeGen, typename FacetGen>
struct stitch_facet_helper<HalfedgeGen, FacetGen, true> 
    // Helper for setting 'f' as the facet on the left of halfedge 'h', when 'fds' is configured with facet links.
{
    static void
    stitch(typename HalfedgeGen::halfedge_descriptor h, typename FacetGen::facet_descriptor f, HalfedgeGen& hds, 
                                                                                                  FacetGen& fds)
    {
        set_facet(h,f,hds);
        FacetGen::ContainerGen::value(f, fds.m_container).m_facetLink = h;
    }
};

} // end namespace detail

// \endcond

template<typename HalfedgeGen, typename FacetGen>
void
stitch_facet(typename HalfedgeGen::halfedge_descriptor h, typename FacetGen::facet_descriptor f, 
                                                                 HalfedgeGen& hds, FacetGen& fds)
    // Set facet 'f' as the facet on the left of halfedge 'h' and set the facet link from 'h' to 'f' if the
    // 'fds' is configured with facet links. 
{
    detail::stitch_facet_helper<HalfedgeGen, FacetGen, FacetGen::facet_selector::has_facet_links
                                >::stitch(h,f,hds,fds);
}

// \cond
namespace detail {

template <typename HalfedgeGen, typename VertexGen, bool HasVertexLink=false>
struct stitch_vertex_helper 
    // Helper for setting 'v' as the source or target vertex of halfedge 'h', when 'vds' is configured without 
    // vertex links.
{
    static void
    stitch(typename HalfedgeGen::halfedge_descriptor h, typename VertexGen::vertex_descriptor v, HalfedgeGen& hds, 
                                                                                                   VertexGen& vds)
    {
        (void) vds;
        set_vertex(h,v,hds);
    }
};

template <typename HalfedgeGen, typename VertexGen>
struct stitch_vertex_helper<HalfedgeGen, VertexGen, true> 
    // Helper for setting 'v' as the source or target vertex of halfedge 'h', when 'vds' is configured with 
    // vertex links.
{
    static void
    stitch(typename HalfedgeGen::halfedge_descriptor h, typename VertexGen::vertex_descriptor v, HalfedgeGen& hds, 
                                                                                                   VertexGen& vds)
    {
        set_vertex(h,v,hds);
        VertexGen::ContainerGen::value(v, vds.m_container).m_vertexLink = h;
    }
};

} // end namespace detail

// \endcond

template<typename HalfedgeGen, typename VertexGen>
void
stitch_vertex(typename HalfedgeGen::halfedge_descriptor h, typename VertexGen::vertex_descriptor v, 
                                                                   HalfedgeGen& hds, VertexGen& vds)
    // Set vertex 'v' as the source or target vertex of halfedge 'h' and set the vertex link from 'h' to 'v' 
    // if the 'vds' is configured with vertex links.
{
    detail::stitch_vertex_helper<HalfedgeGen, VertexGen, VertexGen::vertex_selector::has_vertex_links
                                >::stitch(h,v,hds,vds);
}

} // end namespace hdstl
} // end namespace boost

#endif
