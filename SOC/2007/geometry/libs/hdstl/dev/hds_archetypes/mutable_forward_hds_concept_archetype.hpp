// mutable_forward_hds_concept_archetype.hpp   -*- C++ -*-
//
//@PURPOSE: Provide a 'MutableForwardHDS' concept archetype 
//
//@DESCRIPTION: This file provides an archetype class for compile-time testing
// of the 'MutableForwardHDSConcept' class.  Namely, this class can be used
// wherever a template parameter of a class or of a function template is
// required to be a model of the 'MutableForwardHDS' concept, in order to
// ensure that no further requirements are placed on the template parameter
// than are stated in the 'MutableForwardHDS' concept.
//
///Archetype class
///---------------
//..
//  template <typename ForwardCategory>
//  class MutableForwardHDSConcept_archetype 
//  : public ForwardHDSConcept_archetype<ForwardCategory>,
//    public MutableHDSConcept_archetype {
//      typedef typename hds_traits<HDSConcept_archetype
//                                 >::halfedge_descriptor halfedge_descriptor;
//      typedef typename hds_traits<ForwardHDSConcept_archetype<ForwardCategory>
//                                 >::traversal_category traversal_category;
//      typedef ForwardCategory                     forward_category;
//      MutableForwardHDSConcept_archetype();
//      MutableForwardHDSConcept_archetype(
//                                   const MutableForwardHDSConcept_archetype&);
//    public:
//      void
//      set_next_in_facet(MutableForwardHDSConcept_archetype& hds,
//                    halfedge_descriptor h, halfedge_descriptor g);
//      void
//      set_next_at_source(MutableForwardHDSConcept_archetype& hds,
//                    halfedge_descriptor h, halfedge_descriptor g);
//      void
//      set_next_at_target(MutableForwardHDSConcept_archetype& hds,
//                    halfedge_descriptor h, halfedge_descriptor g);
//  };
//..

#ifndef BOOST_HDSTL_MUTABLE_FORWARD_HDS_CONCEPT_ARCHETYPE_HPP
#define BOOST_HDSTL_MUTABLE_FORWARD_HDS_CONCEPT_ARCHETYPE_HPP

#include <boost/hds_archetype/hds_concept_archetypes.hpp>

namespace boost {
namespace hdstl {

template <typename ForwardCategory>
class MutableForwardHDSConcept_archetype;  // forward declaration

template <typename ForwardCategory>
class MutableForwardHDSConcept_archetype 
: public ForwardHDSConcept_archetype<ForwardCategory>,
  public MutableHDSConcept_archetype {
    // This class provides an exact implementation (no more, no less) of the
    // 'MutableForwardHDS' concept.  It can be used to instantiate class and
    // function templates that require their template arguments to be a model
    // of this concept in order to verify that there are no extra syntactic
    // requirements.

    // PRIVATE TYPES
    typedef typename hds_traits<HDSConcept_archetype
                               >::halfedge_descriptor halfedge_descriptor;
        //Halfedge descriptor type from 'HDSConcept' 

    typedef typename hds_traits<ForwardHDSConcept_archetype<ForwardCategory>
                               >::traversal_category traversal_category;
        //traversal category type from from 'ForwardHDSConcept', 
        //has to be convertible to 'forward_traversal_tag'.
    
    typedef ForwardCategory                     forward_category;
        // This type, convertible to one or more of 'next_in_facet_tag',
        // 'next_at_source_tag', or 'next_at_target_tag', indicates which is
        // the primary accessor(s) for which the 'set_...' methods are defined.
                               
    // NOT IMPLEMENTED
    MutableForwardHDSConcept_archetype();
    MutableForwardHDSConcept_archetype(
                                 const MutableForwardHDSConcept_archetype&);

  public:
    // MANIPULATORS
    void
    set_next_in_facet(MutableForwardHDSConcept_archetype& hds,
                  halfedge_descriptor h, halfedge_descriptor g);
        // set 'g' as the halfedge following 'h' in the (counter-clockwise)
        // facet cycle of 'h' in 'hds'.

    void
    set_next_at_source(MutableForwardHDSConcept_archetype& hds,
                  halfedge_descriptor h, halfedge_descriptor g);
        // set 'g' as the halfedge following 'h' in the (clockwise) vertex
        // cycle of the source of 'h' in 'hds'.

    void
    set_next_at_target(MutableForwardHDSConcept_archetype& hds,
                  halfedge_descriptor h, halfedge_descriptor g);
        // set 'g' as the halfedge following 'h' in the (clockwise) vertex
        // cycle of the target of 'h' in 'hds'.
};

// MANIPULATORS
template <typename ForwardCategory>
void
MutableForwardHDSConcept_archetype<ForwardCategory>::set_next_in_facet(
                                MutableForwardHDSConcept_archetype& hds,
                                halfedge_descriptor h, halfedge_descriptor g)
{}

template <typename ForwardCategory>
void
MutableForwardHDSConcept_archetype<ForwardCategory>::set_next_at_source(
                                MutableForwardHDSConcept_archetype& hds,
                                halfedge_descriptor h, halfedge_descriptor g)
{}

template <typename ForwardCategory>
void
MutableForwardHDSConcept_archetype<ForwardCategory>::set_next_at_target(
                                MutableForwardHDSConcept_archetype& hds,
                                halfedge_descriptor h, halfedge_descriptor g)
{}

#endif
