// backward_hds_concept_archetype.hpp   -*- C++ -*-
//
//@PURPOSE: Provide a 'BackwardHDS' concept archetype 
//
//@DESCRIPTION: This file provides an archetype class for compile-time testing
// of the 'BackwardHDSConcept' class.  Namely, this class can be used wherever a
// template parameter of a class or of a function template is required to be a
// model of the 'BackwardHDS' concept, in order to ensure that no further
// requirements are placed on the template parameter than are stated in the
// 'BackwardHDS' concept.
//
///Archetype class
///---------------
//..
//  template <typename BackwardCategory>
//  struct hds_traits<BackwardHDSConcept_archetype<BackwardCategory> > {
//      typedef boost::default_constructible_archetype<
//              boost::copy_constructible_archetype<
//              boost::equality_comparable_archetype<
//              boost::assignable_archetype<> > > > halfedge_descriptor;
//      typedef boost::convertible_to_archetype<backward_traversal_tag>
//                                                  traversal_category;
//      typedef BackwardCategory                     backward_category;
//  };
//  template <typename BackwardCategory>
//  class BackwardHDSConcept_archetype : public HDSConcept_archetype {
//      typedef typename hds_traits<BackwardHDSConcept_archetype
//                                 >::halfedge_descriptor halfedge_descriptor;
//      BackwardHDSConcept_archetype();
//      BackwardHDSConcept_archetype(const BackwardHDSConcept_archetype&);
//    public:
//      halfedge_descriptor
//      prev_in_facet(BackwardHDSConcept_archetype const& hds,
//                    halfedge_descriptor                h) const;
//      halfedge_descriptor
//      prev_at_source(BackwardHDSConcept_archetype const& hds,
//                     halfedge_descriptor                h) const;
//      halfedge_descriptor
//      prev_at_target(BackwardHDSConcept_archetype const& hds,
//                     halfedge_descriptor                h) const;
//  };
//..

#ifndef BOOST_HDSTL_BACKWARD_HDS_CONCEPT_ARCHETYPE_HPP
#define BOOST_HDSTL_BACKWARD_HDS_CONCEPT_ARCHETYPE_HPP

#include <boost/hds_archetype/hds_concept_archetypes.hpp>

namespace boost {
namespace hdstl {

template <typename BackwardCategory>
class BackwardHDSConcept_archetype;  // forward declaration

template <typename BackwardCategory>
struct hds_traits<BackwardHDSConcept_archetype<BackwardCategory> > {
    // This template specialization of 'hds_traits' for the
    // 'BackwardHDSConcept_archetype' provides the 'halfedge_descriptor' and
    // 'traversal_category' types.
    
    // TYPES
    typedef boost::default_constructible_archetype<
            boost::copy_constructible_archetype<
            boost::equality_comparable_archetype<
            boost::assignable_archetype<> > > > halfedge_descriptor;
        // Halfedge descriptor type for the 'BackwardHDSConcept' archetype.

    typedef boost::convertible_to_archetype<backward_traversal_tag>
                                                traversal_category;
        // This type, convertible to 'backward_traversal_tag', indicates that
        // the 'BackwardHDSConcept' archetype is a model of 'BackwardHDSConcept'.

    typedef BackwardCategory                     backward_category;
        // This type, convertible to one or more of 'prev_in_facet_tag',
        // 'prev_at_source_tag', or 'prev_at_target_tag', indicates which is
        // the primary accessor(s) for which the 'set_...' methods are defined.
};


template <typename BackwardCategory>
class BackwardHDSConcept_archetype : public HDSConcept_archetype {
    // This class provides an exact implementation (no more, no less) of the
    // 'BackwardHDS' concept.  It can be used to instantiate class and function
    // templates that require their template arguments to be a model of this
    // concept in order to verify that there are no extra syntactic
    // requirements.

    // PRIVATE TYPES
    typedef typename hds_traits<BackwardHDSConcept_archetype
                               >::halfedge_descriptor halfedge_descriptor;

    // NOT IMPLEMENTED
    BackwardHDSConcept_archetype();
    BackwardHDSConcept_archetype(const BackwardHDSConcept_archetype&);

  public:
    // MANIPULATORS
    halfedge_descriptor
    prev_in_facet(BackwardHDSConcept_archetype const& hds,
                  halfedge_descriptor                h) const;
        // Return the halfedge preceding 'h' in the (counter-clockwise) facet
        // cycle of 'h' in 'hds'.

    halfedge_descriptor
    prev_at_source(BackwardHDSConcept_archetype const& hds,
                   halfedge_descriptor                h) const;
        // Returns the halfedge preceding 'h' in the (clockwise) vertex
        // cycle of the source of 'h' in 'hds'.

     halfedge_descriptor
     prev_at_target(BackwardHDSConcept_archetype const& hds,
                    halfedge_descriptor                h) const;
        // Returns the halfedge preceding 'h' in the (clockwise) vertex
        // cycle of the target of 'h' in 'hds'.
};

// MANIPULATORS
template <typename BackwardCategory>
typename hds_traits<BackwardHDSConcept_archetype>::halfedge_descriptor
BackwardHDSConcept_archetype<BackwardCategory>::prev_in_facet(
                                        BackwardHDSConcept_archetype const& hds,
                                        halfedge_descriptor                h)
{
    return halfedge_descriptor();
}

template <typename BackwardCategory>
typename hds_traits<BackwardHDSConcept_archetype>::halfedge_descriptor
BackwardHDSConcept_archetype<BackwardCategory>::prev_at_source(
                                        BackwardHDSConcept_archetype const& hds,
                                        halfedge_descriptor                h)
{
    return halfedge_descriptor();
}

template <typename BackwardCategory>
typename hds_traits<BackwardHDSConcept_archetype>::halfedge_descriptor
BackwardHDSConcept_archetype<BackwardCategory>::prev_at_target(
                                        BackwardHDSConcept_archetype const& hds,
                                        halfedge_descriptor                h)
{
    return halfedge_descriptor();
}

#endif
