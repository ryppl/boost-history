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

#include <boost/hdstl/hds_archetypes/mutable_hds_concept_archetype.hpp>
#include <boost/hdstl/hds_archetypes/vertex_hds_concept_archetype.hpp>
#include <boost/hdstl/hds_traits.hpp>
#include <boost/concept_archetype.hpp>

namespace boost {
namespace hdstl {

class MutableVertexHDSConcept_archetype;  // forward declaration

template<>
struct hds_traits<MutableVertexHDSConcept_archetype> {
    // This template specialization of 'hds_traits' for the
    // 'MutableVertexHDSConcept_archetype' provides the 'vertex_descriptor'
    // type.

    // TYPES
    typedef boost::default_constructible_archetype<
            boost::copy_constructible_archetype<
            boost::equality_comparable_archetype<
            boost::assignable_archetype<> > > > halfedge_descriptor;
        // Halfedge descriptor type for the 'MutableVertexHDSConcept' archetype.

    typedef boost::default_constructible_archetype<
            boost::copy_constructible_archetype<
            boost::equality_comparable_archetype<
            boost::assignable_archetype<> > > > vertex_descriptor;
        // Vertex descriptor type for the 'MutableVertexHDSConcept' archetype.

    enum { supports_vertices = true };
    enum { supports_facets = false };
};
class MutableVertexHDSConcept_archetype : public MutableHDSConcept_archetype,
                                          public VertexHDSConcept_archetype {
    // This archetype class for the 'MutableVertexHDSConcept' class can be used
    // wherever a template parameter of a class or of a function template is
    // required to be a model of the 'MutableVertexHDS' concept, in order to
    // ensure that no further requirements are placed on the template parameter
    // than are stated in the 'MutableVertexHDS' concept.

    // PRIVATE TYPES
    typedef hds_traits<MutableVertexHDSConcept_archetype
                               >::vertex_descriptor vertex_descriptor;
    
    typedef hds_traits<MutableVertexHDSConcept_archetype
                               >::halfedge_descriptor halfedge_descriptor;
    
    public:
        //MANIPULATORS
        void
        set_vertex(halfedge_descriptor h, vertex_descriptor v,
                                      MutableVertexHDSConcept_archetype& hds);
        // sets the source vertex descriptor value of 'h' to 'v' for a single
        // halfedge in the 'hds' data structure.
        
        vertex_descriptor
        new_vertex(MutableVertexHDSConcept_archetype& hds);
        // adds a new vertex 'v' to the 'hds' data structure. By this operation
        // the vertex is added but no connections to the halfedges are set. In
        // order to assign vertces to halfedges 'set_vertex(hds,h,v)' operation
        // should be used.

        void
        delete_vertex(vertex_descriptor v,
                                      MutableVertexHDSConcept_archetype& hds);
        //  removes the vertex 'v' from the 'hds' data structure, by iterating
        //  in clockwise order around the vertex and removing the connections
        //  with the halfedges.

};

//MANIPULATORS
inline
void
set_vertex(hds_traits<MutableVertexHDSConcept_archetype>::halfedge_descriptor h,
             hds_traits<MutableVertexHDSConcept_archetype>::vertex_descriptor v,
                              MutableVertexHDSConcept_archetype& hds)
{
    (void)hds;  // eliminate unused variable warning
    (void)h;  // eliminate unused variable warning
    (void)v;  // eliminate unused variable warning
}

inline
hds_traits<MutableVertexHDSConcept_archetype>::vertex_descriptor
new_vertex(MutableVertexHDSConcept_archetype& hds)
{
    (void)hds;  // eliminate unused variable warning
    return hds_traits<MutableVertexHDSConcept_archetype>::vertex_descriptor();
}

inline
void
delete_vertex(hds_traits<MutableVertexHDSConcept_archetype>
                                        ::vertex_descriptor v,
                                        MutableVertexHDSConcept_archetype& hds)
{
    (void)hds;  // eliminate unused variable warning
    (void)v;  // eliminate unused variable warning
}

} // end namespace hdstl
} // end namespace boost

#endif
