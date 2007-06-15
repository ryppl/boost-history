// mutable_hds_concept_archetype.hpp   -*- C++ -*-
//
//@PURPOSE: Provide a 'MutableHDSConcept' archetype 
//
//@DESCRIPTION: This file provides an archetype class for compile-time testing
// of the 'MutableHDSConcept' class.  Namely, this class can be used wherever a
// template parameter of a class or of a function template is required to be a
// model of the 'MutableHDS' concept, in order to ensure that no further
// requirements are placed on the template parameter than are stated in the
// 'MutableHDS' concept.
//
///Archetype class
///---------------
//..
//  class MutableHDSConcept_archetype : public HDSConcept_archetype {
//      typedef typename hds_traits<HDSConcept_archetype
//                                 >::halfedge_descriptor halfedge_descriptor;
//    public:
//      void set_opposite(MutableHDSConcept_archetype&,
//                        halfedge_descriptor&, halfedge_descriptor&);
//      halfedge_descriptor& new_edge(MutableHDSConcept_archetype&, 
//                                 halfedge_descriptor&, halfedge_descriptor&);
//      void delete_edge(MutableHDSConcept_archetype&,
//                       halfedge_descriptor&, halfedge_descriptor&);
//  };
//..

#ifndef BOOST_HDSTL_MUTABLE_HDS_CONCEPT_ARCHETYPE_HPP
#define BOOST_HDSTL_MUTABLE_HDS_CONCEPT_ARCHETYPE_HPP

#include <boost/concept_archetype.hpp>

class MutableHDSConcept_archetype;  // forward declaration

class MutableHDSConcept_archetype : public HDSConcept_archetype {
    // This archetype class for the 'MutableHDSConcept' class can be used
    // wherever a template parameter of a class or of a function template is
    // required to be a model of the 'MutableHDS' concept, in order to ensure
    // that no further requirements are placed on the template parameter than
    // are stated in the 'MutableHDS' concept.

    // PRIVATE TYPES
    typedef typename hds_traits<HDSConcept_archetype
                               >::halfedge_descriptor halfedge_descriptor;

  public:
    void set_opposite(MutableHDSConcept_archetype&,
                      halfedge_descriptor&, halfedge_descriptor&);
        // Set the second halfedge descriptor as the opposite of the first
        // halfedge descriptor in 'hds'.

    halfedge_descriptor& new_edge(MutableHDSConcept_archetype&, 
                                  halfedge_descriptor&, halfedge_descriptor&);
       // add two halfedges to the data structure.
    
    void delete_edge(MutableHDSConcept_archetype&,
                     halfedge_descriptor&, halfedge_descriptor&);
      // delete two halfedges from the data structure.
};

// MANIPULATORS
void 
MutableHDSConcept_archetype::set_opposite(MutableHDSConcept_archetype&, 
                                   halfedge_descriptor&, halfedge_descriptor&)
{}

typename hds_traits<MutableHDSConcept_archetype>::halfedge_descriptor
MutableHDSConcept_archetype::new_edge(MutableHDSConcept_archetype&, 
                                   halfedge_descriptor&, halfedge_descriptor&)
{
    return halfedge_descriptor();
}

void 
MutableHDSConcept_archetype::delete_edge(MutableHDSConcept_archetype&, 
                                   halfedge_descriptor&, halfedge_descriptor&)
{}

#endif
