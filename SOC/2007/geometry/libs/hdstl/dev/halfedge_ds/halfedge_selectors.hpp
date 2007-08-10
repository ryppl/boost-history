//halfedge_selectors.hpp   -*- C++ -*-
//
//@PURPOSE: Provide a concrete implementation of 'stored_halfedge' based on various configurations of 'halfedge_ds'.
//
//@CLASSES:
//        'halfedgeS': defines configuration options for haledge storage and access
//     'halfedge_gen': defines the stored type and accesors for a halfedge configuration
//  'stored_halfedge': defines the storage for a given halfedge configuration
//  
//@SEE_ALSO: {hds_concepts.hpp, halfedge_ds.hpp}
//
//@DESCRIPTION: This file provides the 'stored_halfedge',
// and the helper classes, which accepts many template parameters that can be
// used to govern its implementation.  These parameters can be chosen using the
// 'Config' class members (see 'halfedge_ds' component). Currently the 
// selectors are as follows:
//
//  - 'forwardS':  defines configuration options for forward access
//  - 'backwardS':  defines configuration options for backward access
//  - 'bidirS':  defines configuration options for bidirectional access
//
///Definition
///----------
//  - 'stored_halfedge'  This class inherits from four helper classes, 
//    'opposite_helper', 'vertex_helper', 'facet_helper', 'traversal_helper'.
//    Based on the configurations of these classes, members are added or omited
//    from 'stored_halfedge' for optimization purposes.

#ifndef BOOST_HDSTL_HALFEDGE_SELECTORS_HPP
#define BOOST_HDSTL_HALFEDGE_SELECTORS_HPP 1

#include <boost/hdstl/hds_traits.hpp>
#include <boost/hdstl/halfedge_ds/container_selectors.hpp>

namespace boost {
namespace hdstl {

                   // =========================
                   // halfedge selector classes
                  // =========================
struct empty_tag {};

template <typename ForwardCategory>
struct forwardS {
    typedef forward_traversal_tag tag;
    typedef ForwardCategory next_tag;
    typedef empty_tag prev_tag;
    enum { is_forward = true };
    enum { is_backward = false };
};

template <typename BackwardCategory>
struct backwardS {
    typedef backward_traversal_tag tag;
    typedef BackwardCategory prev_tag;
    typedef empty_tag next_tag;
    enum { is_forward = false };
    enum { is_backward = true };
};

template <typename ForwardCategory, typename BackwardCategory>
struct bidirS {
    typedef bidirectional_traversal_tag tag;
    typedef ForwardCategory next_tag;
    typedef BackwardCategory prev_tag;
    enum { is_forward = true };
    enum { is_backward = true };
};

template <typename ContainerS, 
          typename TraversalS = forwardS<next_in_facet_tag> >
struct halfedgeS 
: public TraversalS
{
    typedef ContainerS container_selector;
};

                   // =======================
                   // halfedge helper classes
                   // =======================

template<bool HasOppositeMember = false, typename HalfedgeDescriptor=int>
struct opposite_helper 
{
};

template<typename HalfedgeDescriptor>
struct opposite_helper<true, HalfedgeDescriptor> 
{
    HalfedgeDescriptor m_opposite;
};

template<bool IsSource=false, typename VertexDescriptor=int>
struct source_helper 
{
};

template<typename VertexDescriptor>
struct source_helper<true, VertexDescriptor> 
{
    VertexDescriptor m_source;
};

template<bool IsTarget=false, typename VertexDescriptor=int>
struct target_helper 
{
};

template<typename VertexDescriptor>
struct target_helper<true, VertexDescriptor> 
{
    VertexDescriptor m_target;
};

template<bool SupportsVertices, bool IsSource, typename VertexDescriptor=int>
struct vertex_helper 
{
};

template<bool IsSource, typename VertexDescriptor>
struct vertex_helper<true, IsSource, VertexDescriptor> 
: public source_helper<IsSource>
, public target_helper<!IsSource>
{
};

template<bool supports_facets =false, typename FacetDescriptor=int>
struct facet_helper 
{
};

template<typename FacetDescriptor>
struct facet_helper<true, FacetDescriptor>
{
    FacetDescriptor m_facet;
};

template<bool IsForward, bool IsBackward, typename HalfedgeDescriptor>
struct traversal_helper 
{
};

template<typename HalfedgeDescriptor>
struct traversal_helper<true, false, HalfedgeDescriptor> 
{
    HalfedgeDescriptor m_next;
};

template<typename HalfedgeDescriptor>
struct traversal_helper<false, true, HalfedgeDescriptor> 
{
    HalfedgeDescriptor m_prev;
};

template<typename HalfedgeDescriptor>
struct traversal_helper<true, true, HalfedgeDescriptor>
{
    HalfedgeDescriptor m_next;
    HalfedgeDescriptor m_prev;
};

                   // =====================
                   // class stored_halfedge 
                   // =====================

template<typename HalfedgeDescriptor, typename VertexDescriptor, 
                                     typename FacetDescriptor, typename Config>
struct stored_halfedge 
: public opposite_helper<Config::halfedge_has_opposite_member, HalfedgeDescriptor>,
  public vertex_helper<Config::halfedge_supports_vertices,
                       Config::is_source, VertexDescriptor>,
  public facet_helper<Config::halfedge_supports_facets, FacetDescriptor>,
  public traversal_helper<Config::is_forward, Config::is_backward, HalfedgeDescriptor>
{
    // This struct implements a stored_halfedge which can be configured based
    // on the 'halfedge_ds::Config' selections.  This stored_halfedge has the
    // following options as members:
    // - having an opposite member or not
    // - having a next/prev pointer or not, or having both
    // - having a vertex descriptor
    // - having a facet descriptor
    // All the selections are done in the ..._helper<> functions and these
    // functions form a base for the stored_halfedge.
};

                   // ==================
                   // class halfedge_gen
                   // ==================

template <typename HalfedgeS,
          typename VertexDescriptor, typename FacetDescriptor, typename Config>
struct halfedge_gen {
};

// SPECIALIZATIONS
template <typename ContainerS, typename TraversalS, 
          typename VertexDescriptor, typename FacetDescriptor, typename Config>
struct halfedge_gen<halfedgeS<ContainerS,TraversalS>,  
                    VertexDescriptor, FacetDescriptor, Config> {
    // TYPES
    typedef halfedgeS<ContainerS,TraversalS>            halfedge_selector;
    
    typedef stored_halfedge<typename Config::halfedge_descriptor, 
                            VertexDescriptor, FacetDescriptor, 
                            Config> halfedge_type;
        // The stored halfedge type for this halfedge generator.

    typedef container_gen<ContainerS, halfedge_type>    ContainerGen;
        // The container generator for this halfdedge generator.

    typedef typename ContainerGen::type                 container_type;
        // The halfedge container type for this halfedge generator.

    typedef typename ContainerGen::size_type            size_type;
        // The halfedge container size type for this halfedge generator.
    
    typedef typename ContainerGen::iterator             halfedge_iterator;
        // The halfedge iterator type for this halfedge generator.
    
    typedef typename ContainerGen::descriptor           halfedge_descriptor;
        // The halfedge descriptor type for this halfedge generator.
    
    typedef Config                                      config;

    // DATA
    container_type m_container;
        // The actual container for the halfedges, from which halfedges can be
        // obtained.
};


} // namespace hdstl
} // namespace boost

#endif
