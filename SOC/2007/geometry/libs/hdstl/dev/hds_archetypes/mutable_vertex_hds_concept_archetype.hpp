// mutable_vertex_hds_concept_archetype.hpp   -*- C++ -*-
//
//@PURPOSE: Provide a 'MutableVertexHDSConcept' archetype 
//
//@DESCRIPTION: This file provides an archetype class for compile-time testing
// of the 'MutableVertexHDSConcept' class.  Namely, this class can be used
// wherever a template parameter of a class or of a function template is
// required to be a model of the 'MutableVertexHDS' concept, in order to ensure
// that no further requirements are placed on the template parameter than are
// stated in the 'MutableVertexHDS' concept.
//
///Archetype class
///---------------
//..
//  class MutableVertexHDSConcept_archetype : public HDSConcept_archetype,
//                                          public VertexHDSConcept_archetype {
//      typedef typename hds_traits<VertexHDSConcept_archetype
//                                 >::vertex_descriptor vertex_descriptor;
//      typedef typename hds_traits<HDSConcept_archetype
//                                 >::halfedge_descriptor halfedge_descriptor;
//      MutableVertexHDSConcept_archetype();
//      MutableVertexHDSConcept_archetype(
//                                    const MutableVertexHDSConcept_archetype&);
//      public:
//          void
//          set_vertex(MutableVertexHDSConcept_archetype& hds,
//                      halfedge_descriptor h, vertex_descriptor v);
//          void
//          add_vertex(MutableVertexHDSConcept_archetype& hds,
//                     vertex_descriptor                   v);
//          void
//          remove_vertex(MutableVertexHDSConcept_archetype& hds,
//                        vertex_descriptor                   v);
//  };
//..

#ifndef BOOST_HDSTL_MUTABLE_VERTEX_HDS_CONCEPT_ARCHETYPE_HPP
#define BOOST_HDSTL_MUTABLE_VERTEX_HDS_CONCEPT_ARCHETYPE_HPP

#include <boost/concept_archetype.hpp>

class MutableVertexHDSConcept_archetype;  // forward declaration

class MutableVertexHDSConcept_archetype : public HDSConcept_archetype,
                                          public VertexHDSConcept_archetype {
    // This archetype class for the 'MutableVertexHDSConcept' class can be used
    // wherever a template parameter of a class or of a function template is
    // required to be a model of the 'MutableVertexHDS' concept, in order to
    // ensure that no further requirements are placed on the template parameter
    // than are stated in the 'MutableVertexHDS' concept.

    // PRIVATE TYPES
    typedef typename hds_traits<VertexHDSConcept_archetype
                               >::vertex_descriptor vertex_descriptor;
    
    typedef typename hds_traits<HDSConcept_archetype
                               >::halfedge_descriptor halfedge_descriptor;
    
    // NOT IMPLEMENTED
    MutableVertexHDSConcept_archetype();
    MutableVertexHDSConcept_archetype(
                                     const MutableVertexHDSConcept_archetype&);
    public:
        //MANIPULATORS
        void
        set_vertex(MutableVertexHDSConcept_archetype& hds,
                    halfedge_descriptor h, vertex_descriptor v);
        // sets the source vertex descriptor value of 'h' to 'v' for a single
        // halfedge in the 'hds' data structure.
        
        void
        add_vertex(MutableVertexHDSConcept_archetype& hds,
                   vertex_descriptor                   v);
        // adds a new vertex 'v' to the 'hds' data structure. By this operation
        // the vertex is added but no connections to the halfedges are set. In
        // order to assign vertces to halfedges 'set_vertex(hds,h,v)' operation
        // should be used.

        void
        remove_vertex(MutableVertexHDSConcept_archetype& hds,
                      vertex_descriptor                   v);
        //  removes the vertex 'v' from the 'hds' data structure, by iterating
        //  in clockwise order around the vertex and removing the connections
        //  with the halfedges.

};

//MANIPULATORS

void
MutableVertexHDSConcept_archetype::set_vertex(
                              MutableVertexHDSConcept_archetype& hds,
                              halfedge_descriptor h, vertex_descriptor v)
{}

void
MutableVertexHDSConcept_archetype::add_vertex(
                              MutableVertexHDSConcept_archetype& hds,
                              vertex_descriptor                    v)
{}

void
MutableVertexHDSConcept_archetype::remove_vertex(
                              MutableVertexHDSConcept_archetype& hds,
                              vertex_descriptor                    v)
{}

#endif
