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
//  class MutableFacetHDSConcept_archetype : public HDSConcept_archetype,
//                                          public FacetHDSConcept_archetype {
//      typedef typename hds_traits<FacetHDSConcept_archetype
//                                 >::facet_descriptor facet_descriptor;
//      typedef typename hds_traits<HDSConcept_archetype
//                                 >::halfedge_descriptor halfedge_descriptor;
//      MutableFacetHDSConcept_archetype();
//      MutableFacetHDSConcept_archetype(
//                                    const MutableFacetHDSConcept_archetype&);
//      public:
//          void
//          set_facet(MutableFacetHDSConcept_archetype& hds,
//                      halfedge_descriptor h, facet_descriptor f);
//          void
//          add_facet(MutableFacetHDSConcept_archetype& hds,
//                     facet_descriptor                   f);
//          void
//          remove_facet(MutableFacetHDSConcept_archetype& hds,
//                        facet_descriptor                   f);
//  };
//..

#ifndef BOOST_HDSTL_MUTABLE_FACET_HDS_CONCEPT_ARCHETYPE_HPP
#define BOOST_HDSTL_MUTABLE_FACET_HDS_CONCEPT_ARCHETYPE_HPP

#include <boost/concept_archetype.hpp>

namespace boost {
namespace hdstl {

class MutableFacetHDSConcept_archetype;  // forward declaration

class MutableFacetHDSConcept_archetype : public HDSConcept_archetype,
                                         public FacetHDSConcept_archetype {
    // This archetype class for the 'MutableFacetHDSConcept' class can be used
    // wherever a template parameter of a class or of a function template is
    // required to be a model of the 'MutableFacetHDS' concept, in order to
    // ensure that no further requirements are placed on the template parameter
    // than are stated in the 'MutableFacetHDS' concept.

    // PRIVATE TYPES
    typedef typename hds_traits<FacetHDSConcept_archetype
                               >::facet_descriptor facet_descriptor;
    
    typedef typename hds_traits<HDSConcept_archetype
                               >::halfedge_descriptor halfedge_descriptor;
    
    // NOT IMPLEMENTED
    MutableFacetHDSConcept_archetype();
    MutableFacetHDSConcept_archetype(
                                     const MutableFacetHDSConcept_archetype&);
    public:
        //MANIPULATORS
        void
        set_facet(MutableFacetHDSConcept_archetype& hds,
                    halfedge_descriptor h, facet_descriptor f);
        // sets the source facet descriptor value of 'h' to 'v' for a single
        // halfedge in the 'hds' data structure.
        
        void
        add_facet(MutableFacetHDSConcept_archetype& hds,
                   facet_descriptor                   f);
        // adds a new facet 'v' to the 'hds' data structure. By this operation
        // the facet is added but no connections to the halfedges are set. In
        // order to assign vertces to halfedges 'set_facet(hds,h,v)' operation
        // should be used.

        void
        remove_facet(MutableFacetHDSConcept_archetype& hds,
                      facet_descriptor                   f);
        //  removes the facet 'v' from the 'hds' data structure, by iterating
        //  in clockwise order around the facet and removing the connections
        //  with the halfedges.

};

//MANIPULATORS

void
MutableFacetHDSConcept_archetype::set_facet(
                              MutableFacetHDSConcept_archetype& hds,
                              halfedge_descriptor h, facet_descriptor f)
{}

void
MutableFacetHDSConcept_archetype::add_facet(
                              MutableFacetHDSConcept_archetype& hds,
                              facet_descriptor                    f)
{}

void
MutableFacetHDSConcept_archetype::remove_facet(
                              MutableFacetHDSConcept_archetype& hds,
                              facet_descriptor                    f)
{}

} // end namespace hdstl
} // end namespace boost

#endif
