// mutable_backward_hds_concept_archetype.hpp   -*- C++ -*-
//
//@PURPOSE: Provide a 'MutableBackwardHDS' concept archetype 
//
//@DESCRIPTION: This file provides an archetype class for compile-time testing
// of the 'MutableBackwardHDSConcept' class.  Namely, this class can be used
// wherever a template parameter of a class or of a function template is
// required to be a model of the 'MutableBackwardHDS' concept, in order to
// ensure that no further requirements are placed on the template parameter
// than are stated in the 'MutableBackwardHDS' concept.
//
///Archetype class
///---------------
//..
//  template <typename BackwardCategory>
//  class MutableBackwardHDSConcept_archetype 
//  : public BackwardHDSConcept_archetype<BackwardCategory>,
//    public MutableHDSConcept_archetype {
//      typedef typename hds_traits<HDSConcept_archetype
//                                 >::halfedge_descriptor halfedge_descriptor;
//    typedef typename hds_traits<BackwardHDSConcept_archetype<BackwardCategory>
//                                 >::traversal_category traversal_category;
//      typedef BackwardCategory                     backward_category;
//      MutableBackwardHDSConcept_archetype();
//      MutableBackwardHDSConcept_archetype(
//                                  const MutableBackwardHDSConcept_archetype&);
//    public:
//      void
//      set_prev_in_facet(MutableBackwardHDSConcept_archetype& hds,
//                    halfedge_descriptor h, halfedge_descriptor g);
//      void
//      set_prev_at_source(MutableBackwardHDSConcept_archetype& hds,
//                    halfedge_descriptor h, halfedge_descriptor g);
//      void
//      set_prev_at_target(MutableBackwardHDSConcept_archetype& hds,
//                    halfedge_descriptor h, halfedge_descriptor g);
//  };
//..

#ifndef BOOST_HDSTL_MUTABLE_BACKWARD_HDS_CONCEPT_ARCHETYPE_HPP
#define BOOST_HDSTL_MUTABLE_BACKWARD_HDS_CONCEPT_ARCHETYPE_HPP

#include <boost/hds_archetype/hds_concept_archetypes.hpp>

namespace boost {
namespace hdstl {

template <typename BackwardCategory>
class MutableBackwardHDSConcept_archetype;  // forward declaration

template <typename BackwardCategory>
class MutableBackwardHDSConcept_archetype 
: public BackwardHDSConcept_archetype<BackwardCategory>,
  public MutableHDSConcept_archetype {
    // This class provides an exact implementation (no more, no less) of the
    // 'MutableBackwardHDS' concept.  It can be used to instantiate class and
    // function templates that require their template arguments to be a model
    // of this concept in order to verify that there are no extra syntactic
    // requirements.

    // PRIVATE TYPES
    typedef typename hds_traits<HDSConcept_archetype
                               >::halfedge_descriptor halfedge_descriptor;
        //Halfedge descriptor type from 'HDSConcept' 

    typedef typename hds_traits<BackwardHDSConcept_archetype<BackwardCategory>
                               >::traversal_category traversal_category;
        //traversal category type from from 'BackwardHDSConcept', 
        //has to be convertible to 'backward_traversal_tag'.
    
    typedef BackwardCategory                     backward_category;
        // This type, convertible to one or more of 'prev_in_facet_tag',
        // 'prev_at_source_tag', or 'prev_at_target_tag', indicates which is
        // the primary accessor(s) for which the 'set_...' methods are defined.
                               
    // NOT IMPLEMENTED
    MutableBackwardHDSConcept_archetype();
    MutableBackwardHDSConcept_archetype(
                                 const MutableBackwardHDSConcept_archetype&);

  public:
    // MANIPULATORS
    void
    set_prev_in_facet(MutableBackwardHDSConcept_archetype& hds,
                  halfedge_descriptor h, halfedge_descriptor g);
        // set 'g' as the halfedge preceding 'h' in the (counter-clockwise)
        // facet cycle of 'h' in 'hds'.

    void
    set_prev_at_source(MutableBackwardHDSConcept_archetype& hds,
                  halfedge_descriptor h, halfedge_descriptor g);
        // set 'g' as the halfedge preceding 'h' in the (clockwise) vertex
        // cycle of the source of 'h' in 'hds'.

    void
    set_prev_at_target(MutableBackwardHDSConcept_archetype& hds,
                  halfedge_descriptor h, halfedge_descriptor g);
        // set 'g' as the halfedge preceding 'h' in the (clockwise) vertex
        // cycle of the target of 'h' in 'hds'.
};

// MANIPULATORS
template <typename BackwardCategory>
void
MutableBackwardHDSConcept_archetype<BackwardCategory>::set_prev_in_facet(
                                MutableBackwardHDSConcept_archetype& hds,
                                halfedge_descriptor h, halfedge_descriptor g)
{}

template <typename BackwardCategory>
void
MutableBackwardHDSConcept_archetype<BackwardCategory>::set_prev_at_source(
                                MutableBackwardHDSConcept_archetype& hds,
                                halfedge_descriptor h, halfedge_descriptor g)
{}

template <typename BackwardCategory>
void
MutableBackwardHDSConcept_archetype<BackwardCategory>::set_prev_at_target(
                                MutableBackwardHDSConcept_archetype& hds,
                                halfedge_descriptor h, halfedge_descriptor g)
{}

#endif
