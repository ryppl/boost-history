// halfedge_list_hds_concept_archetype.hpp   -*- C++ -*-
//
//@PURPOSE: Provide a 'HalfedgeListHDS' concept archetype 
//
//@DESCRIPTION: This file provides an archetype class for compile-time testing
// of the 'HalfedgeListHDSConcept' class.  Namely, this class can be used
// wherever a template parameter of a class or of a function template is
// required to be a model of the 'HalfedgeListHDS' concept, in order to ensure
// that no further requirements are placed on the template parameter than are
// stated in the 'HalfedgeListHDS' concept.
//
///Archetype class
///---------------
//..
//  template <>
//  struct hds_traits<HalfedgeListHDSConcept_archetype> {
//      typedef boost::default_constructible_archetype<
//              boost::copy_constructible_archetype<
//              boost::equality_comparable_archetype<
//              boost::assignable_archetype<> > > > halfedge_descriptor;
//      typedef boost::forward_iterator_archetype<halfedge_descriptor> 
//                                                         halfedge_iterator;
//      typedef int                      size_type;
//      enum { supports_vertices = false };
//      enum { supports_facets = false };
//  };
//  class HalfedgeListHDSConcept_archetype : public HDSConcept_archetype {
//      typedef hds_traits<HalfedgeListHDSConcept_archetype
//                                 >::halfedge_iterator halfedge_iterator;
//      typedef hds_traits<HalfedgeListHDSConcept_archetype
//                                 >::halfedge_descriptor halfedge_descriptor;
//    typedef hds_traits<HalfedgeListHDSConcept_archetype>::size_type size_type;
//    public:
//      halfedge_iterator
//      halfedges_begin(HalfedgeListHDSConcept_archetype const& hds) const;
//      halfedge_iterator
//      halfedges_end(HalfedgeListHDSConcept_archetype const& hds) const;
//      size_type
//      num_halfedges(HalfedgeListHDSConcept_archetype const& hds) const;
//  };
//..

#ifndef BOOST_HDSTL_HALFEDGE_LIST_HDS_CONCEPT_ARCHETYPE_HPP
#define BOOST_HDSTL_HALFEDGE_LIST_HDS_CONCEPT_ARCHETYPE_HPP

#include <boost/hdstl/hds_archetypes/hds_concept_archetype.hpp>
#include <boost/hdstl/hds_traits.hpp>
#include <boost/concept_archetype.hpp>

namespace boost {
namespace hdstl {

class HalfedgeListHDSConcept_archetype;  // forward declaration

template <>
struct hds_traits<HalfedgeListHDSConcept_archetype> {
    // This template specialization of 'hds_traits' for the
    // 'HalfedgeListHDSConcept_archetype' provides the 'halfedge_iterator'
    // and 'size_type' types.
    
    // TYPES
    typedef boost::default_constructible_archetype<
            boost::copy_constructible_archetype<
            boost::equality_comparable_archetype<
            boost::assignable_archetype<> > > > halfedge_descriptor;
        // Halfedge descriptor type for the 'HalfedgeListHDSConcept' archetype.

    typedef boost::forward_iterator_archetype<halfedge_descriptor> 
                                                             halfedge_iterator;
        // Halfedge iterator type for the 'HalfedgeListHDSConcept' archetype.

    typedef int                      size_type;
        // Halfedge size type for the 'HalfedgeListHDSConcept' archetype.

    enum { supports_vertices = false };
    enum { supports_facets = false };
};

class HalfedgeListHDSConcept_archetype : public HDSConcept_archetype {
    // This class provides an exact implementation (no more, no less) of the
    // 'HalfedgeListHDS' concept.  It can be used to instantiate class and
    // function templates that require their template arguments to be a model
    // of this concept in order to verify that there are no extra syntactic
    // requirements.

    // PRIVATE TYPES
    typedef hds_traits<HalfedgeListHDSConcept_archetype
                               >::halfedge_iterator halfedge_iterator;
    
    typedef hds_traits<HalfedgeListHDSConcept_archetype
                               >::halfedge_descriptor halfedge_descriptor;
        //halfedge_descriptor from the HalfedgeListHDSConcept used here.

    typedef hds_traits<HalfedgeListHDSConcept_archetype>::size_type size_type;
    
  public:
    // MANIPULATORS
    halfedge_iterator
    halfedges_begin(HalfedgeListHDSConcept_archetype const& hds) const;
        // Returns a 'halfedge_iterator' pointing to the beginning of the 
        // 'halfedge list'.

    halfedge_iterator
    halfedges_end(HalfedgeListHDSConcept_archetype const& hds) const;
        // Returns a 'halfedge_iterator' pointing to the end of the 
        // 'halfedge list'.

    size_type
    num_halfedges(HalfedgeListHDSConcept_archetype const& hds) const;
        // Returns the number of halfedges in the 'hds' data structure.
};

// MANIPULATORS
inline
hds_traits<HalfedgeListHDSConcept_archetype>::halfedge_iterator
halfedges_begin(HalfedgeListHDSConcept_archetype const& hds)
{
    (void)hds;  // eliminate unused variable warning
    return hds_traits<HalfedgeListHDSConcept_archetype>::halfedge_iterator();
}

inline
hds_traits<HalfedgeListHDSConcept_archetype>::halfedge_iterator
halfedges_end(HalfedgeListHDSConcept_archetype const& hds)
{
    (void)hds;  // eliminate unused variable warning
    return hds_traits<HalfedgeListHDSConcept_archetype>::halfedge_iterator();
}

inline
hds_traits<HalfedgeListHDSConcept_archetype>::size_type
num_halfedges(HalfedgeListHDSConcept_archetype const& hds)
{
    (void)hds;  // eliminate unused variable warning
    return 0;
}

} // end namespace hdstl
} // end namespace boost

#endif
