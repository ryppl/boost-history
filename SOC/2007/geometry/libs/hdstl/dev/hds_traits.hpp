//hds_traits.hpp   -*- C++ -*-
//
//@PURPOSE: Provide all the basic types and choices of the 'HDS'.
//
//@DESCRIPTION: This file provides the 'HDS' traits concept as a class, which 
// provides all the component types for 'HDS' and defines the choices.
//
//@CONCEPT: [hdstraits] HDS traits definition
//
///Notation
///--------
//  - 'HDS'    A type that is a model of 'HDSConcept'
//
///Associated types
///----------------
//  - 'HDS::halfedge_descriptor': must be 'DefaultConstructible',
//    'CopyConstructible', 'EqualityComparable', and 'Assignable'.
//  - 'HDS::vertex_descriptor': must be 'DefaultConstructible', 
//    'CopyConstructible', 'EqualityComparable', and 'Assignable'.
//  - 'HDS::facet_descriptor': must be 'DefaultConstructible', 
//    'CopyConstructible', 'EqualityComparable', and 'Assignable'.
//  - 'HDS::vertex_iterator': must be a model of the 
//    'ForwardIterator' concept. The value type of the vertex iterator must
//    be the same as the 'vertex_descriptor'.
//  - 'HDS::facet_iterator': must be a model of the
//    'ForwardIterator' concept.  The value type of the facet iterator
//    must be the same as the 'facet_descriptor'.
//  - 'HDS::halfedge_iterator': must be a model of the
//    'ForwardIterator' concept. The value type of the halfedge iterator
//    must be the same as the 'halfedge_descriptor'.
//  - 'HDS::const_vertex_iterator': must be a model of the 
//    'ForwardIterator' concept. The value type of the vertex iterator must
//    be the same as the 'vertex_descriptor'.
//  - 'HDS::facet_iterator': must be a model of the
//    'ForwardIterator' concept.  The value type of the facet iterator
//    must be the same as the 'facet_descriptor'.
//  - 'HDS::halfedge_iterator': must be a model of the
//    'ForwardIterator' concept. The value type of the halfedge iterator
//    must be the same as the 'halfedge_descriptor'.
//  - 'HDS::size_type': integral size type.
//  - 'HDS::traversal_category': must be convertible to one of
//     'hdstl::forward_traversal_tag', 'hdstl::backward_traversal_tag' or
//     'hdstl::bidirectional_traversal_tag'
//  - 'HDS::forward_category': defines the pointer type for forward iteration.
//  - 'HDS::backward_category': defines the pointer type for backward iteration.
//
///Definitions
///-----------
//  - 'halfedge_descriptor' is a type that contains information to access the
//    halfedge.  Note that the halfedge descriptor type is an opaque type.  All
//    that can be done to it is passing it to the function opposite and letting
//    the hds use the information it contains to find the opposite halfedge.  In
//    particular, there is no guarantee that the descriptor be a pointer type.
//  - 'vertex_descriptor' is a type that contains information to access the
//     vertex at the source or the target of the halfedge.
//  - 'facet_descriptor' is a type that contains information to access the
//     facet to the left of the halfedge, if forward facet cycles are
//     ordered in counter clockwise rotation around the facet.
//  - 'vertex_iterator' is an iterator type for the vertices.
//  - 'facet_iterator' is an iterator type for the facets.
//  - 'halfedge_iterator' is an iterator type for the halfedges.
//  - 'vertex_iterator' is a const iterator type for the vertices.
//  - 'facet_iterator' is a const iterator type for the facets.
//  - 'halfedge_iterator' is a const iterator type for the halfedges.
//  - 'size_type' defines the size type.
//  - 'traversal_category' is a type that contains information about the 
//     direction of the traversal, which is forward, backward or bidirectional.
//  - 'forward_category' defines the primary accessor of the forward
//     information for use by algorithms which desire the most direct access. 
//  - 'backward_category' defines the primary accessor of the backward
//     information for use by algorithms which desire the most direct access. 


///HDS Traits class
///----------------
//..
//  template <typename HDS>
//  struct hds_traits{
//      typedef typename HDS::vertex_descriptor         vertex_descriptor;
//      typedef typename HDS::halfedge_descriptor       halfedge_descriptor;
//      typedef typename HDS::facet_descriptor          facet_descriptor;
//      typedef typename HDS::vertex_iterator           vertex_iterator;
//      typedef typename HDS::halfedge_iterator         halfedge_iterator;
//      typedef typename HDS::facet_iterator            facet_iterator;
//      typedef typename HDS::const_vertex_iterator     const_vertex_iterator;
//      typedef typename HDS::const_halfedge_iterator   const_halfedge_iterator;
//      typedef typename HDS::const_facet_iterator      const_facet_iterator;
//      typedef typename HDS::size_type                 size_type;
//      typedef typename HDS::traversal_category        traversal_category; 
//      typedef typename HDS::forward_category          forward_category;
//      typedef typename HDS::backward_category         backward_category;
//  };
//  struct forward_traversal_tag {};
//  struct backward_traversal_tag {};
//  struct next_in_facet_tag {};
//  struct next_at_source_tag {};
//  struct next_at_target_tag {};
//  struct prev_in_facet_tag {};
//  struct prev_at_source_tag {};
//  struct prev_at_target_tag {};
//..
#ifndef BOOST_HDSTL_HDS_TRAITS_HPP
#define BOOST_HDSTL_HDS_TRAITS_HPP 1

namespace boost{
namespace hdstl {

    template <typename HDS>
    struct hds_traits{

        typedef typename HDS::vertex_descriptor         vertex_descriptor;
        typedef typename HDS::halfedge_descriptor       halfedge_descriptor;
        typedef typename HDS::facet_descriptor          facet_descriptor;

        typedef typename HDS::vertex_iterator           vertex_iterator;
        typedef typename HDS::halfedge_iterator         halfedge_iterator;
        typedef typename HDS::facet_iterator            facet_iterator;

        typedef typename HDS::const_vertex_iterator     const_vertex_iterator;
        typedef typename HDS::const_halfedge_iterator   const_halfedge_iterator;
        typedef typename HDS::const_facet_iterator      const_facet_iterator;

        typedef typename HDS::size_type                 size_type;

        typedef typename HDS::traversal_category        traversal_category; 
        typedef typename HDS::forward_category          forward_category;
        typedef typename HDS::backward_category         backward_category;
    };

    struct forward_traversal_tag {};
    struct backward_traversal_tag {};

    struct next_in_facet_tag {};
    struct next_at_source_tag {};
    struct next_at_target_tag {};
    
    struct prev_in_facet_tag {};
    struct prev_at_source_tag {};
    struct prev_at_target_tag {};

} // namespace hdstl
} // namespace boost

#endif

