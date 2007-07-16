//halfedge_selectors.hpp   -*- C++ -*-
//
//@PURPOSE: Provide a concrete implementation of 'stored_halfedge' based on various configurations of 'halfedge_ds'.
//
//@CLASSES:
//  'stored_halfedge': defines the storage for a given halfedge configuration
//     'halfedge_gen': defines the stored type and accesors for a halfedge 
//                     configuration
//        'halfedgeS':  inherits the options from one of 'forwardS', 'backwardS'
//                      , or 'bidirS', and defined the container type.
//  
//@SEE_ALSO: {hds_concepts.hpp, halfedge_ds.hpp}
//
//@DESCRIPTION: This file provides the 'stored_halfedge',
//and the helper classes, which accepts many template parameters that can be
//used to govern its implementation.  These parameters can be chosen using the
//'Config' class members (see 'halfedge_ds' component). Currently the
// helper classes are as follows:
//
//  - 'opposite_helper':  class for configuring opposite access methods
//  - 'vertex_helper':    class for configuring vertex access methods
//  - 'facet_helper':     class for configuring facet access methods
//  - 'traversal_helper': class for configuring traversal methods
//  
//  and the selectors are as follows:
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
//  - 'vertex_helper' If vertices are supported in the 'halfedge_ds', this class
//    defines 'source' or 'target' members, based on the configuration
//    options. If vertices are not supported, this class does not define a 
//    vertex access member.
//  - 'facet_helper' If facets are supported in the 'halfedge_ds', this class
//    defines facet members, based on the configuration
//    options. If facets are not supported, this class does not define a 
//    facet access member.
//  - 'traversal_helper' Based on the traversal configuration, this class
//    defines 'm_next', 'm_prev' or both traversal access members.


#ifndef BOOST_HDSTL_HALFEDGE_SELECTORS_HPP
#define BOOST_HDSTL_HALFEDGE_SELECTORS_HPP 1

#include <boost/hdstl/hds_traits.hpp>
#include <boost/hdstl/halfedge_ds/halfedge_ds.hpp>

namespace boost {
namespace hdstl {

template <typename ForwardCategory>
struct forwardS {
    typedef forward_traversal_tag tag;
    typedef ForwardCategory next_tag;
    enum { is_forward = true };
    enum { is_backward = false };
    enum { is_bidir = false };
};

template <typename BackwardCategory>
struct backwardS {
    typedef backward_traversal_tag tag;
    typedef BackwardCategory prev_tag;
    enum { is_forward = false };
    enum { is_backward = true };
    enum { is_bidir = false };
};

template <typename ForwardCategory, typename BackwardCategory>
struct bidirS {
    typedef bidirectional_traversal_tag tag;
    typedef ForwardCategory next_tag;
    typedef BackwardCategory prev_tag;
    enum { is_forward = false };
    enum { is_backward = false };
    enum { is_bidir = true };
};

template <typename ContainerS, 
          typename TraversalS = forwardS<next_in_facet_tag> >
struct halfedgeS 
: public TraversalS {
    typedef ContainerS container_selector;
};
template<bool has_opposite_member = false, typename HalfedgeDescriptor=int>
struct opposite_helper 
{
};

template<typename HalfedgeDescriptor>
struct opposite_helper<true, HalfedgeDescriptor> 
{
    HalfedgeDescriptor m_opposite;
};

template<bool is_source=false, typename VertexDescriptor=int>
struct source_helper 
{
};

template<typename VertexDescriptor>
struct source_helper<true, VertexDescriptor> 
{
    VertexDescriptor m_source;
};

template<bool is_target=false, typename VertexDescriptor=int>
struct target_helper 
{
};

template<typename VertexDescriptor>
struct target_helper<true, VertexDescriptor> 
{
    VertexDescriptor m_target;
};

template<bool supports_vertices, bool is_source, bool is_target,
                                                typename VertexDescriptor=int>
struct vertex_helper 
{
};

template<bool is_source, bool is_target, typename VertexDescriptor>
struct vertex_helper<true, is_source, is_target, VertexDescriptor> 
: public source_helper<is_source>,
  public target_helper<is_target> {
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

template<bool is_forward, bool is_backward, bool isbidir, 
                                                   typename HalfedgeDescriptor>
struct traversal_helper 
{
};

template<typename HalfedgeDescriptor>
struct traversal_helper<true, false, false, HalfedgeDescriptor> 
{
    HalfedgeDescriptor m_next;
};

template<typename HalfedgeDescriptor>
struct traversal_helper<false, true, false, HalfedgeDescriptor> 
{
    HalfedgeDescriptor m_prev;
};

template<typename HalfedgeDescriptor>
struct traversal_helper<false, false, true, HalfedgeDescriptor>
: public traversal_helper<true,false,false,HalfedgeDescriptor>,
  public traversal_helper<false,true,false,HalfedgeDescriptor>
{
};

template<typename HalfedgeDescriptor, typename VertexDescriptor, 
                                     typename FacetDescriptor, typename Config>
struct stored_halfedge 
: public opposite_helper<Config::halfedge_has_opposite_member, 
                                                           HalfedgeDescriptor>,
  public vertex_helper<Config::halfedge_supports_vertices,
                       Config::is_source, Config::is_target, VertexDescriptor>,
  public facet_helper<Config::halfedge_supports_facets, FacetDescriptor>,
  public traversal_helper<Config::is_forward, Config::is_backward, 
                                          Config::is_bidir, HalfedgeDescriptor>
{
    // This struct implements a stored_halfedge which can be configured based
    // on the 'halfedge_ds::config' selections. Based on the configuration
    // options, this stored_halfedge has the following options as members:
    // - having an opposite member or not
    // - having a next/prev pointer or not, or having both
    // - having a vertex descriptor
    // - having a facet descriptor
    // All the selections are done in the ..._helper<> functions and these
    // functions form a base for the stored_halfedge.

    // CREATORS
    stored_halfedge() {} 

};

template <typename HalfedgeS, typename HalfedgeDescriptor, 
          typename VertexDescriptor, typename FacetDescriptor, typename Config>
struct halfedge_gen {
};

template <typename ContainerS, typename TraversalS,typename HalfedgeDescriptor, 
          typename VertexDescriptor, typename FacetDescriptor, typename Config>
struct halfedge_gen<halfedgeS<ContainerS,TraversalS>, HalfedgeDescriptor,
                    VertexDescriptor, FacetDescriptor, Config> {
    // TYPES
    typedef stored_halfedge<HalfedgeDescriptor, VertexDescriptor, 
                                        FacetDescriptor, Config> halfedge_type;
        // The stored halfedge type for this halfedge generator.

    typedef container_gen<ContainerS, halfedge_type>    ContainerGen;
        // The container generator for this halfdedge generator.

    typedef typename ContainerGen::type                 container_type;
        // The halfedge container type for this halfedge generator.

    typedef typename ContainerGen::descriptor           halfedge_descriptor;
        // The halfedge descriptor type for this halfedge generator.

    typedef typename ContainerGen::iterator             halfedge_iterator;
        // The halfedge iterator type for this halfedge generator.

    // DATA
    container_type m_container;
        // The actual container for the halfedges, from which halfedges can be
        // obtained.
};

// FREE FUNCTIONS
template <typename HalfedgeS, typename HalfedgeDescriptor, 
          typename VertexDescriptor, typename FacetDescriptor, typename Config>
typename halfedge_gen<HalfedgeS, HalfedgeDescriptor, VertexDescriptor, 
                                             FacetDescriptor, Config>::iterator
halfedges_begin(halfedge_gen<HalfedgeS, HalfedgeDescriptor, VertexDescriptor, 
                                         FacetDescriptor, Config> const& hds) {
    typedef typename halfedge_gen<HalfedgeS, HalfedgeDescriptor,
          VertexDescriptor,FacetDescriptor, Config>::ContainerGen ContainerGen;
    return ContainerGen::container_begin(hds.m_container);
}

template <typename HalfedgeS, typename HalfedgeDescriptor, 
          typename VertexDescriptor, typename FacetDescriptor, typename Config>
typename halfedge_gen<HalfedgeS, HalfedgeDescriptor, VertexDescriptor, 
                                             FacetDescriptor, Config>::iterator
halfedges_end(halfedge_gen<HalfedgeS, HalfedgeDescriptor, VertexDescriptor, 
                                         FacetDescriptor, Config> const& hds) {
    typedef typename halfedge_gen<HalfedgeS, HalfedgeDescriptor,
          VertexDescriptor,FacetDescriptor, Config>::ContainerGen ContainerGen;
    return ContainerGen::container_end(hds.m_container);
}

template <typename HalfedgeS, typename HalfedgeDescriptor, 
          typename VertexDescriptor, typename FacetDescriptor, typename Config>
typename halfedge_gen<HalfedgeS, HalfedgeDescriptor, VertexDescriptor, 
                                             FacetDescriptor, Config>::iterator
num_halfedges(halfedge_gen<HalfedgeS, HalfedgeDescriptor, VertexDescriptor, 
                                         FacetDescriptor, Config> const& hds) {
    return hds.m_container.size();
}
} // namespace hdstl
} // namespace boost

#endif
