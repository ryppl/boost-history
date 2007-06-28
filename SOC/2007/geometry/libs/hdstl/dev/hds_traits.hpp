//hds_traits.hpp   -*- C++ -*-
//
//@PURPOSE: Provide all the basic types and choices of the 'HDS'.
//
//@CLASSES:
//             'hds_traits<HDS>':  parameterized traits class for 'HDS' types
//      'backward_traversal_tag':  tag type for models of 'BackwardHds' concept
// 'bidirectional_traversal_tag':  tag type for models of 'BidirectionalHds' concept
//       'forward_traversal_tag':  tag type for models of 'ForwardHds' concept
//          'next_at_source_tag':  tag type for forward primary access
//          'next_at_target_tag':  tag type for forward primary access
//           'next_in_facet_tag':  tag type for forward primary access
//          'prev_at_source_tag':  tag type for backward primary access
//          'prev_at_target_tag':  tag type for backward primary access
//           'prev_in_facet_tag':  tag type for backward primary access
//  
//@SEE_ALSO: {hds_concepts.hpp}
//
//@DESCRIPTION: This file provides the default 'HDS' traits class, which 
// provides information about the 'HDS' template parameter.  The members of the
// traits class are as follows, grouped by concepts.  Note that, since
// 'hds_traits' is a template, the types which have no meaning (e.g.,
// 'facet_descriptor' when the 'HDS' template parameter does not model
// the 'FacetHds' concept) can remain undefined, so long as they are not
// used at instantiation.
//..
//  Hds concept
//  -----------
//  halfedge_descriptor     A type that contains information to access the
//                          halfedge.  Note that the halfedge descriptor type
//                          is an opaque type.  All that can be done to it is
//                          passing it to the function opposite and letting the
//                          hds use the information it contains to find the
//                          opposite halfedge.  In particular, there is no
//                          guarantee that the descriptor be a pointer type.
//  traversal_category      A type that contains information about the 
//                          direction of the traversal, which is forward,
//                          backward or bidirectional.
//  supports_facets         A value usable as a Boolean which is true if 'HDS'
//                          models the 'FacetHds' concept.
//  supports_vertices       A value usable as a Boolean which is true if 'HDS'
//                          models the 'VertexHds' concept.
//
//  ForwardHds concept
//  ------------------
//  forward_category        A tag type to describe the primary accessor of the
//                          forward information for use by algorithms which
//                          desire the most direct access.  Only for models
//                          of 'ForwardHds' concept.
//
//  BackwardHds concept
//  ------------------
//  backward_category       A tag type to describe the primary accessor of the
//                          backward information for use by algorithms which
//                          desire the most direct access.  Only for models of
//                          'BackwardHds' concept.
//
//  FacetHds concept
//  ----------------
//  facet_descriptor        A type that contains information to access the
//                          facet to the left of the halfedge, if forward facet
//                          cycles are ordered in counter clockwise rotation
//                          around the facet.
//
//  FacetListHds concept
//  --------------------
//  facet_iterator          An iterator type for the facets.
//  facet_const_iterator    A 'const' iterator type for the facets.
//                          Only for 'FacetListHds' concept.
//  size_type               An integral type to return the number of facets,
//                          vertices and halfedges.  Only for 'FacetListHDS',
//                          'HalfedgeListHds' and/or 'VertexListHds' concepts.
//                          Note that the size type must then be the same for
//                          each in the subset of those concepts modeled.
//
//  HalfedgeListHds concept
//  -----------------------
//  halfedge_iterator       An iterator type for the halfedges.
//  halfedge_const_iterator A 'const' iterator type for the halfedges.
//                          Only for 'HalfedgeListHds' concept.
//  size_type               An integral type to return the number of halfedges,
//                          See above for 'FacetListHds' concept.
//
//  VertexHds concept
//  -----------------
//  vertex_descriptor       A type that contains information to access the
//                          vertex at the source or the target of the halfedge.
//
//  VertexListHds concept
//  ---------------------
//  vertex_iterator         An iterator type for the vertices.
//  vertex_const_iterator   A 'const' iterator type for the vertices.
//  size_type               An integral type to return the number of facets,
//                          See above for 'FacetListHds' concept.
//..
//
///Synopsis
///--------
//..
//  template <typename HDS>
//  struct hds_traits{
//      typedef typename HDS::backward_category         backward_category;
//      typedef typename HDS::const_halfedge_iterator   const_halfedge_iterator;
//      typedef typename HDS::const_facet_iterator      const_facet_iterator;
//      typedef typename HDS::const_vertex_iterator     const_vertex_iterator;
//      typedef typename HDS::facet_descriptor          facet_descriptor;
//      typedef typename HDS::facet_iterator            facet_iterator;
//      typedef typename HDS::forward_category          forward_category;
//      typedef typename HDS::halfedge_descriptor       halfedge_descriptor;
//      typedef typename HDS::halfedge_iterator         halfedge_iterator;
//      typedef typename HDS::size_type                 size_type;
//      typedef typename HDS::traversal_category        traversal_category; 
//      typedef typename HDS::vertex_descriptor         vertex_descriptor;
//      typedef typename HDS::vertex_iterator           vertex_iterator;
//  };
//  struct backward_traversal_tag {};
//  struct forward_traversal_tag {};
//  struct bidirectional_traversal_tag
//  : public backward_traversal_tag, public forward_traversal_tag {};
//  struct next_at_source_tag {};
//  struct next_at_target_tag {};
//  struct next_in_facet_tag {};
//  struct prev_at_source_tag {};
//  struct prev_at_target_tag {};
//  struct prev_in_facet_tag {};
//..

#ifndef BOOST_HDSTL_HDS_TRAITS_HPP
#define BOOST_HDSTL_HDS_TRAITS_HPP 1

namespace boost{
namespace hdstl {

    template <typename HDS>
    struct hds_traits{
        // A traits class for describing the types and features associated with
        // the 'HDS' template parameter.  By default, all these types and
        // values are defined within the 'HDS' class.  

        // TYPES
        typedef typename HDS::backward_category         backward_category;
            // A tag type to describe the primary accessor of the backward
            // information for use by algorithms which desire the most direct
            // access.
        typedef typename HDS::const_facet_iterator      const_facet_iterator;
            // A 'const' iterator type for the facets.
        typedef typename HDS::const_halfedge_iterator   const_halfedge_iterator;
            // A 'const' iterator type for the halfedges.
        typedef typename HDS::const_vertex_iterator     const_vertex_iterator;
            // A 'const' iterator type for the vertices.
        typedef typename HDS::facet_descriptor          facet_descriptor;
            // A type that contains information to access the facet to the left
            // of the halfedge, if forward facet cycles are ordered in counter
            // clockwise rotation around the facet.
        typedef typename HDS::facet_iterator            facet_iterator;
            // An iterator type for the facets.
        typedef typename HDS::forward_category          forward_category;
            // A tag type to describe the primary accessor of the forward
            // information for use by algorithms which desire the most direct
            // access.
        typedef typename HDS::halfedge_descriptor       halfedge_descriptor;
            // A type that contains information to access the halfedge. 
        typedef typename HDS::halfedge_iterator         halfedge_iterator;
            // An iterator type for the halfedges.
        typedef typename HDS::vertex_descriptor         vertex_descriptor;
            // A type that contains information to access the vertex at the
            // source or the target of the halfedge.
        typedef typename HDS::vertex_iterator           vertex_iterator;
            // An iterator type for the vertices.
        typedef typename HDS::size_type                 size_type;
            // An integral type to return the number of facets, vertices and/or
            // halfedges.  Note that the size type must then be the same for
            // all three quantities.
        typedef typename HDS::traversal_category        traversal_category; 
            // A type that contains information about the direction of the
            // traversal, which is forward, backward or bidirectional.

        enum {
            // An anonymous enumeration type defining values usable as Boolean
            // to indicate various features associated with the 'HDS' template
            // parameter.

            supports_facets = HDS::supports_facets,
                // A value usable as a Boolean which is true if 'HDS' models
                // the 'FacetHds' concept.
            supports_vertices = HDS::supports_vertices
                // A value usable as a Boolean which is true if 'HDS' models
                // the 'VertexHds' concept.
        };
    };

    struct forward_traversal_tag {
        // A tag type for indicating a model of 'ForwardHds'.  For types 'HDS'
        // that model the 'ForwardHds' concept, the type
        // 'hds_traits<HDS>::traversal_category' is convertible to this
        // 'forward_traversal_tag' type.
    };

    struct backward_traversal_tag {
        // A tag type for indicating a model of 'BackwardHds'.  For types 'HDS'
        // that model the 'BackwardHds' concept, the type
        // 'hds_traits<HDS>::traversal_category' is convertible to this
        // 'backward_traversal_tag' type.

    struct bidirectional_traversal_tag : public backward_traversal_tag, public forward_traversal_tag {
        // A tag type for indicating a model of both 'ForwardHds' and
        // 'BackwardHds' concepts.  This type if convertible to both
        // 'forward_traversal_tag' and 'backward_traversal_tag'.
    };

    struct next_in_facet_tag {
        // A tag type for indicating the primary pointer used by a model of
        // the 'ForwardHds' concept.  Given a type 'HDS' that models the
        // 'ForwardHds' concept, if the 'hds_traits<HDS>::forward_category'
        // is convertible to this 'next_in_facet_tag' type, then
        // 'set_next_in_facet(h, g, hds)' has the post-condition that
        // 'next_in_fact(h, hds) == g', otherwise 'next_in_facet(h, hds)' is
        // not modified.
    };

    struct next_at_source_tag {
        // A tag type for indicating the primary pointer used by a model of
        // the 'ForwardHds' concept.  Given a type 'HDS' that models the
        // 'ForwardHds' concept, if the 'hds_traits<HDS>::forward_category'
        // is convertible to this 'next_at_source_tag' type, then
        // 'set_next_at_source(h, g, hds)' has the post-condition that
        // 'next_in_fact(h, hds) == g', otherwise 'next_at_source(h, hds)' is
        // not modified.
    };

    struct next_at_target_tag {
        // A tag type for indicating the primary pointer used by a model of
        // the 'ForwardHds' concept.  Given a type 'HDS' that models the
        // 'ForwardHds' concept, if the 'hds_traits<HDS>::forward_category'
        // is convertible to this 'next_at_target_tag' type, then
        // 'set_next_at_target(h, g, hds)' has the post-condition that
        // 'next_in_fact(h, hds) == g', otherwise 'next_at_target(h, hds)' is
        // not modified.
    };

    struct prev_in_facet_tag {
        // A tag type for indicating the primary pointer used by a model of
        // the 'BackwardHds' concept.  Given a type 'HDS' that models the
        // 'BackwardHds' concept, if the 'hds_traits<HDS>::forward_category'
        // is convertible to this 'prev_in_facet_tag' type, then
        // 'set_prev_in_facet(h, g, hds)' has the post-condition that
        // 'prev_in_fact(h, hds) == g', otherwise 'prev_in_facet(h, hds)' is
        // not modified.
    };

    struct prev_at_source_tag {
        // A tag type for indicating the primary pointer used by a model of
        // the 'BackwardHds' concept.  Given a type 'HDS' that models the
        // 'BackwardHds' concept, if the 'hds_traits<HDS>::forward_category'
        // is convertible to this 'prev_at_source_tag' type, then
        // 'set_prev_at_source(h, g, hds)' has the post-condition that
        // 'prev_in_fact(h, hds) == g', otherwise 'prev_at_source(h, hds)' is
        // not modified.
    };

    struct prev_at_target_tag {
        // A tag type for indicating the primary pointer used by a model of
        // the 'BackwardHds' concept.  Given a type 'HDS' that models the
        // 'BackwardHds' concept, if the 'hds_traits<HDS>::forward_category'
        // is convertible to this 'prev_at_target_tag' type, then
        // 'set_prev_at_target(h, g, hds)' has the post-condition that
        // 'prev_in_fact(h, hds) == g', otherwise 'prev_at_target(h, hds)' is
        // not modified.
    };

} // namespace hdstl
} // namespace boost

#endif
