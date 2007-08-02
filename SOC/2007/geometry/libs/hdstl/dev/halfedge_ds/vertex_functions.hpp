//vertex_functions.hpp   -*- C++ -*-
//
//@PURPOSE: Provide free functions for accessing and manipulating vertices.
//
//@DESCRIPTION: This file provides free functions for accessing and
// manipulating vertices. The list of these functions are as follows:
//
//  - vertices_begin: returns an iterator to the first vertex in the 'hds' as part of the 'VertexListHDS' concept. 
//  -   vertices_end: returns an iterator after the last vertex in the 'hds' as part of the 'VertexListHDS' concept.
//  -   num_vertices: returns the number of vertices in the 'hds' as part of the 'VertexListHDS' concept.
//  -       halfedge: returns a descriptor to one of the halfedges around this vertex cycle as part of the 'VertexHDS'
//                    concept.
//  -         source: returns the descriptor of the source vertex of the given halfedge 'h' in the 'hds' as part of 
//                    the 'VertexHDS' concept.
//  -         target: returns the descriptor of the target vertex of the given halfedge 'h' in the 'hds' as part of 
//                    the 'VertexHDS' concept.
//  -     set_vertex: sets the vertex 'v' as the vertex on the left for halfedge 'h' in the 'hds' as part of the 
//                    'MutableVertexHDS' concept.
//  -     new_vertex: creates a vertex and adds it into the 'hds' as part of the 'MutableVertexHDS' concept.
//  -  delete_vertex: removes the given vertex 'v' from the 'hds' as part of the 'MutableVertexHDS' concept.
//
//@SEE_ALSO: {hds_concepts.hpp, container_selectors.hpp, vertex_selectors.hpp}

#ifndef BOOST_HDSTL_VERTEX_FUNCTIONS_HPP
#define BOOST_HDSTL_VERTEX_FUNCTIONS_HPP 1

#include <boost/hdstl/halfedge_ds/vertex_selectors.hpp>

namespace boost {
namespace hdstl {
                   // =====================
                   // VertexListHDS Concept
                   // =====================

// FREE FUNCTIONS
template <typename VertexS, typename HalfedgeDescriptor, typename VertexBase>
typename vertex_gen<VertexS, HalfedgeDescriptor,VertexBase>::vertex_iterator
vertices_begin(vertex_gen<VertexS, HalfedgeDescriptor, VertexBase>& hds) 
    // Returns a vertex iterator to the beginning of the container member of 'hds'. The value type
    // of the vertex iterator is a vertex descriptor.
{
    typedef typename vertex_gen<VertexS, HalfedgeDescriptor, 
                                        VertexBase>::ContainerGen ContainerGen;
    return ContainerGen::container_begin(hds.m_container);
}

template <typename VertexS, typename HalfedgeDescriptor, typename VertexBase>
typename vertex_gen<VertexS, HalfedgeDescriptor, VertexBase>::vertex_iterator
vertices_end(vertex_gen<VertexS, HalfedgeDescriptor, VertexBase>& hds) 
    // Returns a vertex iterator to the end of the container member of 'hds'. The value type
    // of the vertex iterator is a vertex descriptor.
{
    typedef typename vertex_gen<VertexS, HalfedgeDescriptor, 
                                        VertexBase>::ContainerGen ContainerGen;
    return ContainerGen::container_end(hds.m_container);
}

template <typename VertexS, typename HalfedgeDescriptor, typename VertexBase>
typename vertex_gen<VertexS, HalfedgeDescriptor, VertexBase>::size_type
num_vertices(vertex_gen<VertexS, HalfedgeDescriptor, VertexBase>& hds) 
    // Returns the number of vertexs in the 'hds'. Return type is the size type specific to 
    // the container used, specified in container selectors.
{
    return hds.m_container.size();
}

template <typename VertexS, typename HalfedgeDescriptor, typename VertexBase>
HalfedgeDescriptor
halfedge(typename vertex_gen<VertexS, HalfedgeDescriptor, VertexBase>::vertex_descriptor const& v, 
         vertex_gen<VertexS, HalfedgeDescriptor, VertexBase>& hds)
    // Returns a halfedge descriptor to one of the halfedges around this vertex in the 'hds'.
{
    return vertex_gen<VertexS, HalfedgeDescriptor, VertexBase>::ContainerGen
                                       ::value(v, hds.m_container).m_vertexLink;
}

                   // =======================
                   // VertexHDS Concept
                   // =======================

template <typename VertexS, typename HalfedgeDescriptor, typename VertexBase>
typename vertex_gen<VertexS, HalfedgeDescriptor, VertexBase>::vertex_descriptor
source(HalfedgeDescriptor h, vertex_gen<VertexS, HalfedgeDescriptor, VertexBase>& hds)
    // Returns a vertex descriptor to the source vertex of the halfedge given by the descriptor 'h' in the 'hds'.
{
    return vertex_gen<VertexS, HalfedgeDescriptor, VertexBase>::ContainerGen
                                       ::value(h, hds.m_container).m_source;
}

template <typename VertexS, typename HalfedgeDescriptor, typename VertexBase>
typename vertex_gen<VertexS, HalfedgeDescriptor, VertexBase>::vertex_descriptor
target(HalfedgeDescriptor h, vertex_gen<VertexS, HalfedgeDescriptor, VertexBase>& hds)
    // Returns a vertex descriptor to the target vertex of the halfedge given by the descriptor 'h' in the 'hds'.
{
    return vertex_gen<VertexS, HalfedgeDescriptor, VertexBase>::ContainerGen
                                       ::value(h, hds.m_container).m_target;
}
                   
                   // ========================
                   // MutableVertexHDS Concept
                   // ========================

namespace detail {

template <typename HalfedgeDescriptor, typename VertexDescriptor, typename VertexGen, bool IsSource>
struct set_vertex_helper 
    // Helper function for setting the vertex 'v' as the source vertex for halfedge 'h' in 'hds'.
{
    static void
    set_vertex(HalfedgeDescriptor h, VertexDescriptor v, VertexGen& hds)
    {
        VertexGen::ContainerGen::value(h, hds.m_container).m_source = v;
    }
};

template <typename HalfedgeDescriptor, typename VertexDescriptor, typename VertexGen>
struct set_vertex_helper<HalfedgeDescriptor, VertexDescriptor, VertexGen, false> 
    // Helper function for setting the vertex 'v' as the target vertex for halfedge 'h' in 'hds'.
{
    static void
    set_vertex(HalfedgeDescriptor h, VertexDescriptor v, VertexGen& hds)
    {
        VertexGen::ContainerGen::value(h, hds.m_container).m_target = v;
    }
};

} // end namespace detail

template <typename VertexS, typename HalfedgeDescriptor, typename VertexBase>
void
set_vertex(HalfedgeDescriptor h, typename vertex_gen<VertexS, HalfedgeDescriptor, VertexBase>::vertex_descriptor v,
                                               vertex_gen<VertexS, HalfedgeDescriptor, VertexBase>& hds)
    // Sets the vertex specified by 'v' as the source or target vertex of the halfedge specified by 'h' in the 'hds'.
    // The selection is based on the VertexS::is_source selector.
{
    typedef vertex_gen<VertexS, HalfedgeDescriptor, VertexBase> VertexGen;
    typedef typename VertexGen::vertex_descriptor vertex_descriptor;
    detail::set_vertex_helper<HalfedgeDescriptor, vertex_descriptor, VertexGen, VertexS::is_source>::set_vertex(h,v,hds);
}

template <typename VertexS, typename HalfedgeDescriptor, typename VertexBase>
typename vertex_gen<VertexS, HalfedgeDescriptor, VertexBase>::vertex_descriptor
new_vertex(vertex_gen<VertexS, HalfedgeDescriptor, VertexBase>& hds)
    // Create a new vertex, add it to the 'hds' and return the descriptor of this new vertex.
{
    typedef typename vertex_gen<VertexS, HalfedgeDescriptor, VertexBase>::ContainerGen ContainerGen;
    typedef typename vertex_gen<VertexS, HalfedgeDescriptor, VertexBase>::vertex_type vertex_type;
    
    vertex_type vertex_f;
    return *ContainerGen::container_add(vertex_f, hds.m_container);
}

} // namespace hdstl
} // namespace boost

#endif
