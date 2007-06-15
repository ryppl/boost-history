// hds_concept_archetype.hpp   -*- C++ -*-
//
//@PURPOSE: Provide a 'HDSConcept' archetype 
//
//@DESCRIPTION: This file provides an archetype class for compile-time testing
// of the 'HDSConcept' class.  Namely, this class can be used wherever a
// template parameter of a class or of a function template is required to be a
// model of the 'HDS' concept, in order to ensure that no further
// requirements are placed on the template parameter than are stated in the
// 'HDS' concept.
//
///Archetype class
///---------------
//..
//  class HDSConcept_archetype {
//      typedef typename hds_traits<HDSConcept_archetype
//                                 >::halfedge_descriptor halfedge_descriptor;
//    public:
//      halfedge_descriptor& opposite(halfedge_descriptor&) const;
//  };
//
//  template<>
//  struct hds_traits<HDSConcept_archetype> {
//      typedef boost::default_constructible_archetype<
//              boost::copy_constructible_archetype<
//              boost::equality_comparable_archetype<
//              boost::assignable_archetype<> > > > halfedge_descriptor;
//  };
//..

#ifndef BOOST_HDSTL_HDS_CONCEPT_ARCHETYPE_HPP
#define BOOST_HDSTL_HDS_CONCEPT_ARCHETYPE_HPP

#include <boost/concept_archetype.hpp>

namespace boost {
namespace hdstl {

class HDSConcept_archetype;  // forward declaration

template<>
struct hds_traits<HDSConcept_archetype> {
    // This template specialization of 'hds_traits' for the
    // 'HDSConcept_archetype' provides the 'halfedge_descriptor' type.

    // TYPES
    typedef boost::default_constructible_archetype<
            boost::copy_constructible_archetype<
            boost::equality_comparable_archetype<
            boost::assignable_archetype<> > > > halfedge_descriptor;
        // Halfedge descriptor type for the 'HDSConcept' archetype.
};

class HDSConcept_archetype {
    // This archetype class for the 'HDSConcept' class can be used wherever a
    // template parameter of a class or of a function template is required to be a
    // model of the 'HDS' concept, in order to ensure that no further
    // requirements are placed on the template parameter than are stated in the
    // 'HDS' concept.

    // PRIVATE TYPES
    typedef typename hds_traits<HDSConcept_archetype
                               >::halfedge_descriptor halfedge_descriptor;

  public:
    // MANIPULATORS
    halfedge_descriptor& opposite(halfedge_descriptor&) const;
        // Return the halfedge opposite 'h' in 'hds'.
};

// MANIPULATORS
typename hds_traits<HDSConcept_archetype>::halfedge_descriptor
HDSConcept_archetype::opposite(ForwardHDSConcept_archetype const& hds,
                               halfedge_descriptor                h)
{
    return halfedge_descriptor();
}

} // end namespace hdstl
} // end namespace boost

#endif
