// mutable_facet_hds_concept_archetype.hpp   -*- C++ -*-
//
//@PURPOSE: Provide a 'MutableFacetHDSConcept' archetype 
//
//@DESCRIPTION: This file provides an archetype class for compile-time testing
// of the 'MutableFacetHDSConcept' class.  Namely, this class can be used
// wherever a template parameter of a class or of a function template is
// required to be a model of the 'MutableFacetHDS' concept, in order to ensure
// that no further requirements are placed on the template parameter than are
// stated in the 'MutableFacetHDS' concept.
//
///Archetype class
///---------------
//..
//  template<>
//  struct hds_traits<MutableFacetHDSConcept_archetype> {
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
//  class MutableFacetHDSConcept_archetype 
//      : public MutableHDSConcept_archetype,
//        public FacetHDSConcept_archetype {
//      // PRIVATE TYPES
//      typedef hds_traits<MutableFacetHDSConcept_archetype
//                                 >::facet_descriptor facet_descriptor;
//      typedef hds_traits<MutableFacetHDSConcept_archetype
//                                 >::halfedge_descriptor halfedge_descriptor;
//      public:
//       void
//       set_facet(halfedge_descriptor h, facet_descriptor f,
//                                       MutableFacetHDSConcept_archetype& hds);
//       facet_descriptor
//       new_facet(MutableFacetHDSConcept_archetype& hds);
//       void
//      delete_facet(facet_descriptor f, MutableFacetHDSConcept_archetype& hds);
//  };
//..

#ifndef BOOST_HDSTL_MUTABLE_FACET_HDS_CONCEPT_ARCHETYPE_HPP
#define BOOST_HDSTL_MUTABLE_FACET_HDS_CONCEPT_ARCHETYPE_HPP

#include <boost/hdstl/hds_archetypes/mutable_hds_concept_archetype.hpp>
#include <boost/hdstl/hds_archetypes/facet_hds_concept_archetype.hpp>
#include <boost/hdstl/hds_traits.hpp>
#include <boost/concept_archetype.hpp>

namespace boost {
namespace hdstl {

class MutableFacetHDSConcept_archetype;  // forward declaration

template<>
struct hds_traits<MutableFacetHDSConcept_archetype> {
    // This template specialization of 'hds_traits' for the
    // 'MutableFacetHDSConcept_archetype' provides the 'facet_descriptor' type.

    // TYPES
    typedef boost::default_constructible_archetype<
            boost::copy_constructible_archetype<
            boost::equality_comparable_archetype<
            boost::assignable_archetype<> > > > halfedge_descriptor;
        // Halfedge descriptor type for the 'MutableFacetHDSConcept' archetype.

    typedef boost::default_constructible_archetype<
            boost::copy_constructible_archetype<
            boost::equality_comparable_archetype<
            boost::assignable_archetype<> > > > facet_descriptor;
        // Facet descriptor type for the 'MutableFacetHDSConcept' archetype.

    enum {supports_vertices = false};
    enum {supports_facets = true};
};

class MutableFacetHDSConcept_archetype 
    : public MutableHDSConcept_archetype,
      public FacetHDSConcept_archetype {
    // This archetype class for the 'MutableFacetHDSConcept' class can be used
    // wherever a template parameter of a class or of a function template is
    // required to be a model of the 'MutableFacetHDS' concept, in order to
    // ensure that no further requirements are placed on the template parameter
    // than are stated in the 'MutableFacetHDS' concept.

    // PRIVATE TYPES
    typedef hds_traits<MutableFacetHDSConcept_archetype
                               >::facet_descriptor facet_descriptor;
    
    typedef hds_traits<MutableFacetHDSConcept_archetype
                               >::halfedge_descriptor halfedge_descriptor;
    
    public:
        //MANIPULATORS
        void
        set_facet(halfedge_descriptor h, facet_descriptor f,
                                        MutableFacetHDSConcept_archetype& hds);
        // sets the source facet descriptor value of 'h' to 'v' for a single
        // halfedge in the 'hds' data structure.
        
        facet_descriptor
        new_facet(MutableFacetHDSConcept_archetype& hds);
        // adds a new facet 'v' to the 'hds' data structure. By this operation
        // the facet is added but no connections to the halfedges are set. In
        // order to assign vertces to halfedges 'set_facet(hds,h,v)' operation
        // should be used.

        void
        delete_facet(facet_descriptor f, MutableFacetHDSConcept_archetype& hds);
        //  removes the facet 'v' from the 'hds' data structure, by iterating
        //  in clockwise order around the facet and removing the connections
        //  with the halfedges.

};

//MANIPULATORS

inline
void
set_facet(hds_traits<MutableFacetHDSConcept_archetype>::halfedge_descriptor h,
             hds_traits<MutableFacetHDSConcept_archetype>::facet_descriptor f,
                              MutableFacetHDSConcept_archetype& hds)
{
    (void)hds;  // eliminate unused variable warning
    (void)h;  // eliminate unused variable warning
    (void)f;  // eliminate unused variable warning
}

inline
hds_traits<MutableFacetHDSConcept_archetype>::facet_descriptor
new_facet(MutableFacetHDSConcept_archetype& hds)
{
    (void)hds;  // eliminate unused variable warning
    return hds_traits<MutableFacetHDSConcept_archetype>::facet_descriptor();
}

inline
void
delete_facet(hds_traits<MutableFacetHDSConcept_archetype>::facet_descriptor f,
                                         MutableFacetHDSConcept_archetype& hds)
{
    (void)hds;  // eliminate unused variable warning
    (void)f;  // eliminate unused variable warning
}

} // end namespace hdstl
} // end namespace boost

#endif
