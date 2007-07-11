//hds_selectors.hpp   -*- C++ -*-
//
//@PURPOSE: Provide selector classes for 'HalfedgeS'.
//
//@CLASSES:
//  'forwardS':  defines configuration options for forward access
//  'backwardS':  defines configuration options for backward access
//  'bidirS':  defines configuration options for bidirectional access
//  'halfedgeS':  inherits the options from one of 'forwardS', 'backwardS', or
//   'bidirS', and defined the container type.
//  
//@SEE_ALSO: {hds_concepts.hpp, halfedge_ds.hpp}
//
//@DESCRIPTION: This file provides the halfedgeS selector class for
//configurations related to halfedge properties. Uses the 'forwardS',
//'backwardS' and 'bidirS' selector classes to inherit the configuration
//options based on the traversal type.

#ifndef BOOST_HDSTL_HDS_SELECTORS_HPP
#define BOOST_HDSTL_HDS_SELECTORS_HPP 1

namespace boost;
namespace hdstl;

template <typename ForwardCategory>
struct forwardS {
    typedef forward_traversal_tag tag;
    typedef ForwardCategory next_tag;
    typedef true_type is_forward;
    typedef false_type is_backward;
    typedef false_type is_bidir;
};

template <typename BackwardCategory>
struct backwardS {
    typedef backward_traversal_tag tag;
    typedef BackwardCategory prev_tag;
    typedef false_type is_forward;
    typedef true_type is_backward;
    typedef false_type is_bidir;
};

template <typename ForwardCategory, typename BackwardCategory>
struct bidirS {
    typedef bidirectional_traversal_tag tag;
    typedef ForwardCategory next_tag;
    typedef BackwardCategory prev_tag;
    typedef false_type is_forward;
    typedef false_type is_backward;
    typedef true_type is_bidir;
};

template <typename Selector, 
          typename TraversalS = forwardS<next_in_facet_tag> >
struct halfedgeS 
: public TraversalS {
    typedef Selector container_type;
};

} // namespace hdstl
} // namespace boost

#endif
