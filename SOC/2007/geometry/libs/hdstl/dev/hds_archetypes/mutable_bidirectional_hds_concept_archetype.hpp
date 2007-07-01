// mutable_bidirectional_hds_concept_archetype.hpp   -*- C++ -*-
//
//@PURPOSE: Provide a 'MutableBidirectionalHDS' concept archetype 
//
//@DESCRIPTION: This file provides an archetype class for compile-time testing
// of the 'MutableBidirectionalHDSConcept' class.  Namely, this class can be
// used wherever a template parameter of a class or of a function template is
// required to be a model of the 'MutableBidirectionalHDS' concept, in order to
// ensure that no further requirements are placed on the template parameter
// than are stated in the 'MutableBidirectionalHDS' concept.
//
///Archetype class
///---------------
//..
//  template <typename ForwardCategory, typename BackwardCategory>
//  struct hds_traits<MutableBidirectionalHDSConcept_archetype<
//                                       ForwardCategory, BackwardCategory> > {
//      typedef boost::default_constructible_archetype<
//              boost::copy_constructible_archetype<
//              boost::equality_comparable_archetype<
//              boost::assignable_archetype<> > > > halfedge_descriptor;
//      typedef boost::convertible_to_archetype<bidirectional_traversal_tag>
//                                                         traversal_category;
//      typedef ForwardCategory                     forward_category;
//      typedef BackwardCategory                     backward_category;
//      enum {supports_vertices = false};
//      enum {supports_facets = false};
//  };
//  template <typename ForwardCategory, typename BackwardCategory> 
//  class MutableBidirectionalHDSConcept_archetype  
//        : public MutableForwardHDSConcept_archetype<ForwardCategory>, 
//          public MutableBackwardHDSConcept_archetype<BackwardCategory>, 
//          public BidirectionalHDSConcept_archetype<
//                                         ForwardCategory, BackwardCategory> {
//      typedef typename hds_traits<MutableBidirectionalHDSConcept_archetype<
//                    ForwardCategory, BackwardCategory> >::halfedge_descriptor
//                                                          halfedge_descriptor;
//      typedef typename hds_traits<MutableBidirectionalHDSConcept_archetype<
//                        ForwardCategory, BackwardCategory> >::forward_category
//                                                             forward_category;
//      typedef typename hds_traits<MutableBidirectionalHDSConcept_archetype<
//                       ForwardCategory, BackwardCategory> >::backward_category
//                                                            backward_category;
//      typedef typename hds_traits<MutableBidirectionalHDSConcept_archetype<
//                     ForwardCategory, BackwardCategory> >::traversal_category
//                                                          traversal_category;
//  };
//..

#ifndef BOOST_HDSTL_MUTABLE_BIDIRECTIONAL_HDS_CONCEPT_ARCHETYPE_HPP
#define BOOST_HDSTL_MUTABLE_BIDIRECTIONAL_HDS_CONCEPT_ARCHETYPE_HPP

#include <boost/hdstl/hds_archetypes/mutable_forward_hds_concept_archetype.hpp>
#include <boost/hdstl/hds_archetypes/mutable_backward_hds_concept_archetype.hpp>
#include <boost/hdstl/hds_archetypes/bidirectional_hds_concept_archetype.hpp>
#include <boost/hdstl/hds_traits.hpp>
#include <boost/concept_archetype.hpp>

namespace boost {
namespace hdstl {

template <typename ForwardCategory, typename BackwardCategory>
class MutableBidirectionalHDSConcept_archetype;  // forward declaration

template <typename ForwardCategory, typename BackwardCategory>
struct hds_traits<MutableBidirectionalHDSConcept_archetype<
                                         ForwardCategory, BackwardCategory> > {
    // This template specialization of 'hds_traits' for the
    // 'MutableBidirectionalHDSConcept_archetype' provides the
    // 'halfedge_descriptor' and 'traversal_category' types.
   
    // TYPES
    typedef boost::default_constructible_archetype<
            boost::copy_constructible_archetype<
            boost::equality_comparable_archetype<
            boost::assignable_archetype<> > > > halfedge_descriptor;
        // Halfedge descriptor type for the 'MutableBidirectionalHDSConcept'
        // archetype.

    typedef boost::convertible_to_archetype<bidirectional_traversal_tag>
                                                       traversal_category;
        // This type, convertible to 'backward_traversal_tag', indicates that
        // the 'MutableBidirectionalHDSConcept' archetype is a model of
        // 'MutableBidirectionalHDSConcept'.

    typedef ForwardCategory                     forward_category;
        // This type, convertible to one or more of 'next_in_facet_tag',
        // 'next_at_source_tag', or 'next_at_target_tag', indicates which is
        // the primary accessor(s) for which the 'set_...' methods are defined.
    
    typedef BackwardCategory                     backward_category;
        // This type, convertible to one or more of 'prev_in_facet_tag',
        // 'prev_at_source_tag', or 'prev_at_target_tag', indicates which is
        // the primary accessor(s) for which the 'set_...' methods are defined.
    
    enum {supports_vertices = false};
    enum {supports_facets = false};
};

template <typename ForwardCategory, typename BackwardCategory> 
class MutableBidirectionalHDSConcept_archetype  
      : public MutableForwardHDSConcept_archetype<ForwardCategory>, 
        public MutableBackwardHDSConcept_archetype<BackwardCategory>, 
        public BidirectionalHDSConcept_archetype<
                                           ForwardCategory, BackwardCategory> {
    // This class provides an exact implementation (no more, no less) of the
    // 'MutableBidirectionalHDS' concept.  It can be used to instantiate class
    // and function templates that require their template arguments to be a
    // model of this concept in order to verify that there are no extra
    // syntactic requirements.

    // PRIVATE TYPES
    typedef typename hds_traits<MutableBidirectionalHDSConcept_archetype<
                      ForwardCategory, BackwardCategory> >::halfedge_descriptor
                                                           halfedge_descriptor;
    
    typedef typename hds_traits<MutableBidirectionalHDSConcept_archetype<
                      ForwardCategory, BackwardCategory> >::forward_category
                                                           forward_category;
    
    typedef typename hds_traits<MutableBidirectionalHDSConcept_archetype<
                      ForwardCategory, BackwardCategory> >::backward_category
                                                           backward_category;
    
    typedef typename hds_traits<MutableBidirectionalHDSConcept_archetype<
                      ForwardCategory, BackwardCategory> >::traversal_category
                                                           traversal_category;

};

} // end namespace hdstl
} // end namespace boost

#endif
