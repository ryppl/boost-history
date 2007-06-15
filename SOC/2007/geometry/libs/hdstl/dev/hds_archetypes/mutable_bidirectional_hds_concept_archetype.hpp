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
//  class MutableBidirectionalHDSConcept_archetype 
//  : public MutableForwardHDSConcept_archetype<ForwardCategory>, 
//    public MutableBackwardHDSConcept_archetype<BackwardCategory>,
//    public BidirectionalHDSConcept_archetype {
//      typedef typename hds_traits<HDSConcept_archetype
//                                 >::halfedge_descriptor halfedge_descriptor;
//      typedef typename hds_traits<ForwardHDSConcept_archetype
//                                 >::forward_category       forward_category;
//      typedef typename hds_traits<BackwardHDSConcept_archetype
//                                 >::backward_category      backward_category;
//      typedef typename hds_traits<BidirectionalHDSConcept_archetype
//                                 >::traversal_category    traversal_category;
//      MutableBidirectionalHDSConcept_archetype();
//      MutableBidirectionalHDSConcept_archetype(
//                            const MutableBidirectionalHDSConcept_archetype&);
//  };
//..

#ifndef BOOST_HDSTL_MUTABLE_BIDIRECTIONAL_HDS_CONCEPT_ARCHETYPE_HPP
#define BOOST_HDSTL_MUTABLE_BIDIRECTIONAL_HDS_CONCEPT_ARCHETYPE_HPP

#include <boost/hds_archetype/hds_concept_archetypes.hpp>

namespace boost {
namespace hdstl {

template <typename ForwardCategory, typename BackwardCategory>
class MutableBidirectionalHDSConcept_archetype;  // forward declaration

template <typename ForwardCategory, typename BackwardCategory>
class MutableBidirectionalHDSConcept_archetype 
: public MutableForwardHDSConcept_archetype<ForwardCategory>, 
  public MutableBackwardHDSConcept_archetype<BackwardCategory>,
  public BidirectionalHDSConcept_archetype {
    // This class provides an exact implementation (no more, no less) of the
    // 'MutableBidirectionalHDS' concept.  It can be used to instantiate class
    // and function templates that require their template arguments to be a
    // model of this concept in order to verify that there are no extra
    // syntactic requirements.

    // PRIVATE TYPES
    typedef typename hds_traits<HDSConcept_archetype
                               >::halfedge_descriptor halfedge_descriptor;
    
    typedef typename hds_traits<ForwardHDSConcept_archetype
                               >::forward_category       forward_category;

    typedef typename hds_traits<BackwardHDSConcept_archetype
                               >::backward_category      backward_category;
    
    typedef typename hds_traits<BidirectionalHDSConcept_archetype
                               >::traversal_category    traversal_category;

    // NOT IMPLEMENTED
    MutableBidirectionalHDSConcept_archetype();
    MutableBidirectionalHDSConcept_archetype(
                               const MutableBidirectionalHDSConcept_archetype&);
};

} // end namespace hdstl
} // end namespace boost

#endif
