// facet_list_hds_concept_archetype.hpp   -*- C++ -*-
//
//@PURPOSE: Provide a 'FacetListHDS' concept archetype 
//
//@DESCRIPTION: This file provides an archetype class for compile-time testing
// of the 'FacetListHDSConcept' class.  Namely, this class can be used wherever
// a template parameter of a class or of a function template is required to be
// a model of the 'FacetListHDS' concept, in order to ensure that no further
// requirements are placed on the template parameter than are stated in the
// 'FacetListHDS' concept.
//
///Archetype class
///---------------
//..
//  template <>
//  struct hds_traits<FacetListHDSConcept_archetype> {
//      typedef hdstl_detail::MultiPassInputIteratorConcept<> facet_iterator;
//      typedef size_type                      size_type;
//  };
//  class FacetListHDSConcept_archetype : public HDSConcept_archetype, 
//                                        public FacetHDSConcept_archetype {
//      typedef typename hds_traits<FacetListHDSConcept_archetype
//                                 >::facet_iterator facet_iterator;
//      typedef typename hds_traits<FacetHDSConcept_archetype
//                                 >::facet_descriptor facet_descriptor;
//      typedef typename hds_traits<FacetListHDSConcept_archetype
//                                 >::size_type size_type;
//      FacetListHDSConcept_archetype();
//      FacetListHDSConcept_archetype(const FacetListHDSConcept_archetype&);
//    public:
//      facet_iterator
//      facets_begin(FacetListHDSConcept_archetype const& hds) const;
//      facet_iterator
//      facets_end(FacetListHDSConcept_archetype const& hds) const;
//      size_type
//      num_facets(FacetListHDSConcept_archetype const& hds) const;
//  };
//..

#ifndef BOOST_HDSTL_FACET_LIST_HDS_CONCEPT_ARCHETYPE_HPP
#define BOOST_HDSTL_FACET_LIST_HDS_CONCEPT_ARCHETYPE_HPP

#include <boost/hds_archetype/hds_concept_archetypes.hpp>

namespace boost {
namespace hdstl {

class FacetListHDSConcept_archetype;  // forward declaration

template <>
struct hds_traits<FacetListHDSConcept_archetype> {
    // This template specialization of 'hds_traits' for the
    // 'FacetListHDSConcept_archetype' provides the 'facet_iterator'
    // and 'size_type' types.
    
    // TYPES
    typedef hdstl_detail::MultiPassInputIteratorConcept<> facet_iterator;
        // Facet iterator type for the 'FacetListHDSConcept' archetype.

    typedef size_type                      size_type;
        // Facet size type for the 'FacetListHDSConcept' archetype.
};


class FacetListHDSConcept_archetype : public HDSConcept_archetype, 
                                      public FacetHDSConcept_archetype {
    // This class provides an exact implementation (no more, no less) of the
    // 'FacetListHDS' concept.  It can be used to instantiate class and function
    // templates that require their template arguments to be a model of this
    // concept in order to verify that there are no extra syntactic
    // requirements.

    // PRIVATE TYPES
    typedef typename hds_traits<FacetListHDSConcept_archetype
                               >::facet_iterator facet_iterator;
    
    typedef typename hds_traits<FacetHDSConcept_archetype
                               >::facet_descriptor facet_descriptor;
        //facet_descriptor from the FacetHDSConcept used here.

    typedef typename hds_traits<FacetListHDSConcept_archetype
                               >::size_type size_type;
    
    // NOT IMPLEMENTED
    FacetListHDSConcept_archetype();
    FacetListHDSConcept_archetype(const FacetListHDSConcept_archetype&);

  public:
    // MANIPULATORS
    facet_iterator
    facets_begin(FacetListHDSConcept_archetype const& hds) const;
        // Returns a 'facet_iterator' pointing to the beginning of the 
        // 'facet list'.

    facet_iterator
    facets_end(FacetListHDSConcept_archetype const& hds) const;
        // Returns a 'facet_iterator' pointing to the end of the 
        // 'facet list'.

    size_type
    num_facets(FacetListHDSConcept_archetype const& hds) const;
        // Returns the number of facets in the 'hds' data structure.
};

// MANIPULATORS
typename hds_traits<FacetListHDSConcept_archetype>::facet_iterator
FacetListHDSConcept_archetype<>::facets_begin(
                               FacetListHDSConcept_archetype const& hds) const
{
    return facet_iterator();
}

typename hds_traits<FacetListHDSConcept_archetype>::facet_iterator
FacetListHDSConcept_archetype<>::facets_end(
                               FacetListHDSConcept_archetype const& hds) const
{
    return facet_iterator();
}

typename hds_traits<FacetListHDSConcept_archetype>::size_type
FacetListHDSConcept_archetype<>::num_facets(
                               FacetListHDSConcept_archetype const& hds) const
{
    return size_type();
}

} // end namespace hdstl
} // end namespace boost

#endif
