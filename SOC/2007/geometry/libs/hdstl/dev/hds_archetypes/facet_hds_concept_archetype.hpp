// facet_hds_concept_archetype.hpp   -*- C++ -*-
//
//@PURPOSE: Provide a 'FacetHDSConcept' archetype 
//
//@DESCRIPTION: This file provides an archetype class for compile-time testing
// of the 'FacetHDSConcept' class.  Namely, this class can be used wherever a
// template parameter of a class or of a function template is required to be a
// model of the 'FacetHDS' concept, in order to ensure that no further
// requirements are placed on the template parameter than are stated in the
// 'FacetHDS' concept.
//
///Archetype class
///---------------
//..
//  class FacetHDSConcept_archetype : public HDSConcept_archetype {
//      typedef typename hds_traits<FacetHDSConcept_archetype
//                                 >::facet_descriptor facet_descriptor;
//  };
//
//  template<>
//  struct hds_traits<FacetHDSConcept_archetype> {
//      typedef boost::default_constructible_archetype<
//              boost::copy_constructible_archetype<
//              boost::equality_comparable_archetype<
//              boost::assignable_archetype<> > > > facet_descriptor;
//  };
//..

#ifndef BOOST_HDSTL_FACET_HDS_CONCEPT_ARCHETYPE_HPP
#define BOOST_HDSTL_FACET_HDS_CONCEPT_ARCHETYPE_HPP

#include <boost/concept_archetype.hpp>

class FacetHDSConcept_archetype;  // forward declaration

template<>
struct hds_traits<FacetHDSConcept_archetype> {
    // This template specialization of 'hds_traits' for the
    // 'FacetHDSConcept_archetype' provides the 'facet_descriptor' type.

    // TYPES
    typedef boost::default_constructible_archetype<
            boost::copy_constructible_archetype<
            boost::equality_comparable_archetype<
            boost::assignable_archetype<> > > > facet_descriptor;
        // Facet descriptor type for the 'FacetHDSConcept' archetype.
};

class FacetHDSConcept_archetype : public HDSConcept_archetype {
    // This archetype class for the 'FacetHDSConcept' class can be used
    // wherever a template parameter of a class or of a function template is
    // required to be a model of the 'FacetHDS' concept, in order to ensure
    // that no further requirements are placed on the template parameter than
    // are stated in the 'FacetHDS' concept.

    // PRIVATE TYPES
    typedef typename hds_traits<FacetHDSConcept_archetype
                               >::facet_descriptor facet_descriptor;
};

#endif
