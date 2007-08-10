//base_functions.hpp   -*- C++ -*-
//
//@PURPOSE: Provide base free functions for accessing and manipulating halfedges, vertices and facets.
//
//@DESCRIPTION: This file provides base free functions for accessing and manipulating halfedges, vertices and facets. 
// The base functions are the building blocks for the Euler operators defined in 'euler_operators.hpp'.
// The list of these functions are as follows:
//
//  -   stitch_next: Sets the next link of the given two halfedges 'h' and 'g' based on the configuration
//                   of data structure 'hds'. 
//  -   stitch_prev: Sets the next link of the given two halfedges 'h' and 'g' based on the configuration
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


template <typename HalfedgeGen, typename Tag>
void
stitch_next_in_facet(typename HalfedgeGen::halfedge_descriptor , typename HalfedgeGen::halfedge_descriptor 
                , HalfedgeGen& , Tag const& )
{
    // No-op
}

template <typename HalfedgeGen>
void
stitch_next_in_facet(typename HalfedgeGen::halfedge_descriptor h, typename HalfedgeGen::halfedge_descriptor g
                , HalfedgeGen& hds,  next_in_facet_tag const&)
{
    set_next_in_facet(h,g,hds);
}

template <typename HalfedgeGen, typename Tag>
void
stitch_prev_in_facet(typename HalfedgeGen::halfedge_descriptor , typename HalfedgeGen::halfedge_descriptor 
                , HalfedgeGen& , Tag const& )
{
    // No-op
}

template <typename HalfedgeGen>
void
stitch_prev_in_facet(typename HalfedgeGen::halfedge_descriptor h, typename HalfedgeGen::halfedge_descriptor g
                , HalfedgeGen& hds,  prev_in_facet_tag const&)
{
    set_prev_in_facet(h,g,hds);
}

template <typename HalfedgeGen, typename Tag>
void
stitch_next_at_source(typename HalfedgeGen::halfedge_descriptor , typename HalfedgeGen::halfedge_descriptor 
               , HalfedgeGen& , Tag const& )
{
    // No-op
}

template <typename HalfedgeGen>
void
stitch_next_at_source(typename HalfedgeGen::halfedge_descriptor h, typename HalfedgeGen::halfedge_descriptor g
                 , HalfedgeGen& hds,  next_at_source_tag const&)
{
    set_next_at_source(h,g,hds);
}

template <typename HalfedgeGen, typename Tag>
void
stitch_prev_at_source(typename HalfedgeGen::halfedge_descriptor , typename HalfedgeGen::halfedge_descriptor 
               , HalfedgeGen& , Tag const& )
{
    // No-op
}

template <typename HalfedgeGen>
void
stitch_prev_at_source(typename HalfedgeGen::halfedge_descriptor h, typename HalfedgeGen::halfedge_descriptor g
                 , HalfedgeGen& hds,  prev_at_source_tag const&)
{
    set_prev_at_source(h,g,hds);
}

template <typename HalfedgeGen, typename Tag>
void
stitch_next_at_target(typename HalfedgeGen::halfedge_descriptor , typename HalfedgeGen::halfedge_descriptor 
               , HalfedgeGen& , Tag const& )
{
    // No-op
}

template <typename HalfedgeGen>
void
stitch_next_at_target(typename HalfedgeGen::halfedge_descriptor h, typename HalfedgeGen::halfedge_descriptor g
                 , HalfedgeGen& hds,  next_at_target_tag const&)
{
    set_next_at_target(h,g,hds);
}

template <typename HalfedgeGen, typename Tag>
void
stitch_prev_at_target(typename HalfedgeGen::halfedge_descriptor , typename HalfedgeGen::halfedge_descriptor 
               , HalfedgeGen& , Tag const& )
{
    // No-op
}

template <typename HalfedgeGen>
void
stitch_prev_at_target(typename HalfedgeGen::halfedge_descriptor h, typename HalfedgeGen::halfedge_descriptor g
                 , HalfedgeGen& hds,  prev_at_target_tag const&)
{
    set_prev_at_target(h,g,hds);
}

template <typename HalfedgeGen, typename ForwardCategory>
typename HalfedgeGen::halfedge_descriptor
get_next_in_facet(typename HalfedgeGen::halfedge_descriptor h, HalfedgeGen& hds, ForwardCategory const&)
{
    return static_cast<typename HalfedgeGen::halfedge_descriptor>(next_in_facet(h, hds));
}

template <typename HalfedgeGen>
typename HalfedgeGen::halfedge_descriptor
get_next_in_facet(typename HalfedgeGen::halfedge_descriptor h, HalfedgeGen& hds, empty_tag const&)
{
    // No-op
}

template <typename HalfedgeGen, typename BackwardCategory>
typename HalfedgeGen::halfedge_descriptor
get_prev_in_facet(typename HalfedgeGen::halfedge_descriptor h, HalfedgeGen& hds, BackwardCategory const&)
{
    return static_cast<typename HalfedgeGen::halfedge_descriptor>(prev_in_facet(h, hds));
}

template <typename HalfedgeGen>
typename HalfedgeGen::halfedge_descriptor
get_prev_in_facet(typename HalfedgeGen::halfedge_descriptor h, HalfedgeGen& hds, empty_tag const&)
{
    // No-op
}

template <typename HalfedgeGen, typename FacetS>
typename HalfedgeGen::facet_descriptor
get_facet(typename HalfedgeGen::halfedge_descriptor h, HalfedgeGen& hds, FacetS const&)
{
    return facet(h, hds);
}

template <typename HalfedgeGen>
typename HalfedgeGen::facet_descriptor
get_facet(typename HalfedgeGen::halfedge_descriptor h, HalfedgeGen& hds, noFacetS const&)
{
    // No-op
}

template <typename HalfedgeGen, typename Type>
typename HalfedgeGen::vertex_descriptor
get_source(typename HalfedgeGen::halfedge_descriptor h, HalfedgeGen& hds, Type const&)
{
    // No-op
}

template <typename HalfedgeGen>
typename HalfedgeGen::vertex_descriptor
get_source(typename HalfedgeGen::halfedge_descriptor h, HalfedgeGen& hds, sourceS const&)
{
    return source(h, hds);
}

template <typename HalfedgeGen, typename Type>
typename HalfedgeGen::vertex_descriptor
get_target(typename HalfedgeGen::halfedge_descriptor h, HalfedgeGen& hds, Type const&)
{
    // No-op
}

template <typename HalfedgeGen>
typename HalfedgeGen::vertex_descriptor
get_target(typename HalfedgeGen::halfedge_descriptor h, HalfedgeGen& hds, targetS const&)
{
    return target(h, hds);
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

template <typename HalfedgeGen>
void
splice(typename HalfedgeGen::halfedge_descriptor h, typename HalfedgeGen::halfedge_descritor g
     , typename HalfedgeGen::halfedge_descriptor i, typename HalfedgeGen::halfedge_descritor j, HalfedgeGen& hds)
{
    typedef typename HalfedgeGen::halfedge_selector::next_tag next_tag;
    typedef typename HalfedgeGen::halfedge_selector::prev_tag prev_tag; 

    stitch_next_in_facet(h, j, hds, next_tag());                    
    stitch_prev_in_facet(j, h, hds, prev_tag());
    
    stitch_next_in_facet(i, g, hds, next_tag());                    
    stitch_prev_in_facet(g, i, hds, prev_tag());
    
    stitch_next_at_source(opposite(h, hds), j, hds, next_tag()); 
    stitch_prev_at_source(j, opposite(h, hds), hds, prev_tag());
    
    stitch_next_at_source(opposite(i, hds), g, hds, next_tag());    
    stitch_prev_at_source(g, opposite(i, hds), hds, prev_tag());
    
    stitch_next_at_target(h, opposite(j, hds), hds, next_tag());    
    stitch_prev_at_target(opposite(j, hds), h, hds, prev_tag());
    
    stitch_next_at_target(i, opposite(g, hds), hds, next_tag());    
    stitch_prev_at_target(opposite(g, hds), i, hds, prev_tag());
}

template <typename HalfedgeGen>
void
detach(typename HalfedgeGen::halfedge_descriptor h, typename HalfedgeGen::halfedge_descritor g
     , typename HalfedgeGen::halfedge_descriptor i, typename HalfedgeGen::halfedge_descritor j, HalfedgeGen& hds)
{
    typedef typename HalfedgeGen::halfedge_selector::next_tag next_tag;
    typedef typename HalfedgeGen::halfedge_selector::prev_tag prev_tag; 

    stitch_next_in_facet(h, g, hds, next_tag());                    
    stitch_prev_in_facet(g, h, hds, prev_tag());
    
    stitch_next_in_facet(i, j, hds, next_tag());                    
    stitch_prev_in_facet(j, i, hds, prev_tag());
    
    stitch_next_at_source(opposite(h, hds), g, hds, next_tag());    
    stitch_prev_at_source(g, opposite(h, hds), hds, prev_tag());
    
    stitch_next_at_source(opposite(i, hds), j, hds, next_tag());    
    stitch_prev_at_source(j, opposite(i, hds), hds, prev_tag());
    
    stitch_next_at_target(h, opposite(g, hds), hds, next_tag());    
    stitch_prev_at_target(opposite(g, hds), h, hds, prev_tag());
    
    stitch_next_at_target(i, opposite(j, hds), hds, next_tag());    
    stitch_prev_at_target(opposite(j, hds), i, hds, prev_tag());
}
} // end namespace hdstl
} // end namespace boost

#endif
