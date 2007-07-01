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
//  class FacetHDSConcept_archetype : virtual public HDSConcept_archetype {
//      typedef typename hds_traits<FacetHDSConcept_archetype
//                                 >::facet_descriptor facet_descriptor;
//      facet_descriptor&
//      facet(halfedge_descriptor &h, FacetHDSConcept_archetype const& hds);
//  };
//
//  template<>
//  struct hds_traits<FacetHDSConcept_archetype> {
//      typedef boost::default_constructible_archetype<
//              boost::copy_constructible_archetype<
//              boost::equality_comparable_archetype<
//              boost::assignable_archetype<> > > > halfedge_descriptor;
//      typedef boost::default_constructible_archetype<
//              boost::copy_constructible_archetype<
//              boost::equality_comparable_archetype<
//              boost::assignable_archetype<> > > > facet_descriptor;
//      enum {supports_vertices = false};
//      enum {supports_facets = true};
//  };
//..

#ifndef BOOST_HDSTL_FACET_HDS_CONCEPT_ARCHETYPE_HPP
#define BOOST_HDSTL_FACET_HDS_CONCEPT_ARCHETYPE_HPP

#include <boost/hdstl/hds_archetypes/hds_concept_archetype.hpp>
#include <boost/hdstl/hds_traits.hpp>
#include <boost/concept_archetype.hpp>

namespace boost {
namespace hdstl {

class FacetHDSConcept_archetype;  // forward declaration

template<>
struct hds_traits<FacetHDSConcept_archetype> {
    // This template specialization of 'hds_traits' for the
    // 'FacetHDSConcept_archetype' provides the 'facet_descriptor' type.

    // TYPES
    typedef boost::default_constructible_archetype<
            boost::copy_constructible_archetype<
            boost::equality_comparable_archetype<
            boost::assignable_archetype<> > > > halfedge_descriptor;
        // Halfedge descriptor type for the 'FacetHDSConcept' archetype.

    typedef boost::default_constructible_archetype<
            boost::copy_constructible_archetype<
            boost::equality_comparable_archetype<
            boost::assignable_archetype<> > > > facet_descriptor;
        // Facet descriptor type for the 'FacetHDSConcept' archetype.

    enum {supports_vertices = false};
    enum {supports_facets = true};
};

class FacetHDSConcept_archetype : virtual public HDSConcept_archetype {
    // This archetype class for the 'FacetHDSConcept' class can be used
    // wherever a template parameter of a class or of a function template is
    // required to be a model of the 'FacetHDS' concept, in order to ensure
    // that no further requirements are placed on the template parameter than
    // are stated in the 'FacetHDS' concept.

    // PRIVATE TYPES
    typedef hds_traits<FacetHDSConcept_archetype>::halfedge_descriptor 
                                                          halfedge_descriptor;
    typedef hds_traits<FacetHDSConcept_archetype>::facet_descriptor 
                                                             facet_descriptor;
  
  public:
    // MANIPULATORS
    facet_descriptor&
    facet(halfedge_descriptor &h, FacetHDSConcept_archetype const& hds);
        // returns the facet descriptor of the facet to the left of 'h', when
        // forward facet cycles are oriented in counter-clockwise order.

};

// MANIPULATORS
inline
hds_traits<FacetHDSConcept_archetype>::facet_descriptor
facet(hds_traits<FacetHDSConcept_archetype>::halfedge_descriptor&, 
                                         FacetHDSConcept_archetype const& hds)
{
    (void)hds;  // eliminate unused variable warning
    return hds_traits<FacetHDSConcept_archetype>::facet_descriptor();
}

} // end namespace hdstl
} // end namespace boost

#endif
