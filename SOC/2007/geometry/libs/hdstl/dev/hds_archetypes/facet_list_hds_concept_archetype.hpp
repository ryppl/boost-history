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
//      typedef hdstl_detail::ForwardIteratorConcept<> facet_iterator;
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

#include <boost/hdstl/hds_archetypes/facet_hds_concept_archetype.hpp>
#include <boost/hdstl/hds_traits.hpp>
#include <boost/concept_archetype.hpp>

namespace boost {
namespace hdstl {

class FacetListHDSConcept_archetype;  // forward declaration

template <>
struct hds_traits<FacetListHDSConcept_archetype> {
    // This template specialization of 'hds_traits' for the
    // 'FacetListHDSConcept_archetype' provides the 'facet_iterator'
    // and 'size_type' types.
    
    // TYPES
    typedef boost::default_constructible_archetype<
            boost::copy_constructible_archetype<
            boost::equality_comparable_archetype<
            boost::assignable_archetype<> > > > halfedge_descriptor;
        // Halfedge descriptor type for the 'FacetListHDSConcept' archetype.

    typedef boost::default_constructible_archetype<
            boost::copy_constructible_archetype<
            boost::equality_comparable_archetype<
            boost::assignable_archetype<> > > > facet_descriptor;
        // Facet descriptor type for the 'FacetHDSConcept' archetype.

    typedef boost::forward_iterator_archetype<facet_descriptor> facet_iterator;
        // Facet iterator type for the 'FacetListHDSConcept' archetype.

    typedef int size_type;
        // Facet size type for the 'FacetListHDSConcept' archetype.
    
    enum {supports_vertices = false};
    enum {supports_facets = true};
};


class FacetListHDSConcept_archetype : public FacetHDSConcept_archetype {
    // This class provides an exact implementation (no more, no less) of the
    // 'FacetListHDS' concept.  It can be used to instantiate class and function
    // templates that require their template arguments to be a model of this
    // concept in order to verify that there are no extra syntactic
    // requirements.

    // PRIVATE TYPES
    typedef hds_traits<FacetListHDSConcept_archetype
                               >::facet_iterator facet_iterator;
    
    typedef hds_traits<FacetHDSConcept_archetype
                               >::halfedge_descriptor halfedge_descriptor;

    typedef hds_traits<FacetHDSConcept_archetype
                               >::facet_descriptor facet_descriptor;
        //facet_descriptor from the FacetHDSConcept used here.

    typedef hds_traits<FacetListHDSConcept_archetype
                               >::size_type size_type;
    
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
inline
hds_traits<FacetListHDSConcept_archetype>::facet_iterator
facets_begin(FacetListHDSConcept_archetype const& hds) 
{
    (void)hds;  // eliminate unused variable warning
    return hds_traits<FacetListHDSConcept_archetype>::facet_iterator();
}

inline
hds_traits<FacetListHDSConcept_archetype>::facet_iterator
facets_end(FacetListHDSConcept_archetype const& hds) 
{
    (void)hds;  // eliminate unused variable warning
    return hds_traits<FacetListHDSConcept_archetype>::facet_iterator();
}

inline
hds_traits<FacetListHDSConcept_archetype>::size_type
num_facets(FacetListHDSConcept_archetype const& hds)
{
    (void)hds;  // eliminate unused variable warning
    return (int)0;
}

} // end namespace hdstl
} // end namespace boost

#endif
