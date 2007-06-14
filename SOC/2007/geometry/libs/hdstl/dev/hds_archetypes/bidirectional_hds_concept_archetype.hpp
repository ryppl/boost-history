// bidirectional_hds_concept_archetype.hpp   -*- C++ -*-
//
//@PURPOSE: Provide a 'BidirectionalHDS' concept archetype 
//
//@DESCRIPTION: This file provides an archetype class for compile-time testing
// of the 'BidirectionalHDSConcept' class.  Namely, this class can be used
// wherever a template parameter of a class or of a function template is
// required to be a model of the 'BidirectionalHDS' concept, in order to ensure
// that no further requirements are placed on the template parameter than are
// stated in the 'BidirectionalHDS' concept.
//
///Archetype class
///---------------
//..
//  template <typename ForwardCategory, typename BackwardCategory>
//  struct hds_traits<BidirectionalHDSConcept_archetype<ForwardCategory, 
//                                                      BackwardCategory> > {
//      typedef boost::default_constructible_archetype<
//              boost::copy_constructible_archetype<
//              boost::equality_comparable_archetype<
//              boost::assignable_archetype<> > > > halfedge_descriptor;
//      typedef boost::convertible_to_archetype<bidirectional_traversal_tag>
//                                                  traversal_category;
//      typedef ForwardCategory                     forward_category;
//      typedef BackwardCategory                     backward_category;
//  };
//  template <typename ForwardCategory, typename BackwardCategory>
//  class BidirectionalHDSConcept_archetype 
//  : public ForwardHDSConcept_archetype<ForwardCategory>, 
//           BackwardHDSConcept_archetype<BackwardCategory> {
//      typedef typename hds_traits<BidirectionalHDSConcept_archetype
//                                 >::halfedge_descriptor halfedge_descriptor;
//      BidirectionalHDSConcept_archetype();
//      BidirectionalHDSConcept_archetype(
//                                    const BidirectionalHDSConcept_archetype&);
//  };
//..

#ifndef BOOST_HDSTL_BIDIRECTIONAL_HDS_CONCEPT_ARCHETYPE_HPP
#define BOOST_HDSTL_BIDIRECTIONAL_HDS_CONCEPT_ARCHETYPE_HPP

#include <boost/hds_archetype/hds_concept_archetypes.hpp>

namespace boost {
namespace hdstl {

template <typename ForwardCategory, typename BackwardCategory>
class BidirectionalHDSConcept_archetype;  // forward declaration

template <typename ForwardCategory, typename BackwardCategory>
struct hds_traits<BidirectionalHDSConcept_archetype<ForwardCategory, 
                                                    BackwardCategory> > {
    // This template specialization of 'hds_traits' for the
    // 'BidirectionalHDSConcept_archetype' provides the 'halfedge_descriptor' 
    // and 'traversal_category' types.
    
    // TYPES
    typedef boost::default_constructible_archetype<
            boost::copy_constructible_archetype<
            boost::equality_comparable_archetype<
            boost::assignable_archetype<> > > > halfedge_descriptor;
        // Halfedge descriptor type for the 'BidirectionalHDSConcept' archetype.

    typedef boost::convertible_to_archetype<bidirectional_traversal_tag>
                                                traversal_category;
        // This type, convertible to 'bidirectional_traversal_tag', indicates
        // that the 'BidirectionalHDSConcept' archetype is a model of
        // 'BidirectionalHDSConcept'.

    typedef ForwardCategory                     forward_category;
        // This type, convertible to one or more of 'next_in_facet_tag',
        // 'next_at_source_tag', or 'next_at_target_tag', indicates which is
        // the primary accessor(s) for which the 'set_...' methods are defined.

    typedef BackwardCategory                     backward_category;
        // This type, convertible to one or more of 'prev_in_facet_tag',
        // 'prev_at_source_tag', or 'prev_at_target_tag', indicates which is
        // the primary accessor(s) for which the 'set_...' methods are defined.
};


template <typename ForwardCategory, typename BackwardCategory>
class BidirectionalHDSConcept_archetype 
: public ForwardHDSConcept_archetype<ForwardCategory>, 
         BackwardHDSConcept_archetype<BackwardCategory> {
    // This class provides an exact implementation (no more, no less) of the
    // 'BidirectionalHDS' concept.  It can be used to instantiate class and
    // function templates that require their template arguments to be a model
    // of this concept in order to verify that there are no extra syntactic
    // requirements.

    // PRIVATE TYPES
    typedef typename hds_traits<BidirectionalHDSConcept_archetype
                               >::halfedge_descriptor halfedge_descriptor;

    // NOT IMPLEMENTED
    BidirectionalHDSConcept_archetype();
    BidirectionalHDSConcept_archetype(const BidirectionalHDSConcept_archetype&);
};

#endif
