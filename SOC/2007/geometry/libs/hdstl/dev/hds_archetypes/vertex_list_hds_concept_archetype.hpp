// vertex_list_hds_concept_archetype.hpp   -*- C++ -*-
//
//@PURPOSE: Provide a 'VertexListHDS' concept archetype 
//
//@DESCRIPTION: This file provides an archetype class for compile-time testing
// of the 'VertexListHDSConcept' class.  Namely, this class can be used wherever
// a template parameter of a class or of a function template is required to be
// a model of the 'VertexListHDS' concept, in order to ensure that no further
// requirements are placed on the template parameter than are stated in the
// 'VertexListHDS' concept.
//
///Archetype class
///---------------
//..
//  template <>
//  struct hds_traits<VertexListHDSConcept_archetype> {
//      typedef boost::default_constructible_archetype<
//              boost::copy_constructible_archetype<
//              boost::equality_comparable_archetype<
//              boost::assignable_archetype<> > > > halfedge_descriptor;
//      typedef boost::default_constructible_archetype<
//              boost::copy_constructible_archetype<
//              boost::equality_comparable_archetype<
//              boost::assignable_archetype<> > > > vertex_descriptor;
// typedef boost::forward_iterator_archetype<vertex_descriptor> vertex_iterator;
//      typedef int size_type;
//      enum { supports_vertices = true };
//      enum { supports_facets = false };
//  };
//  class VertexListHDSConcept_archetype : public VertexHDSConcept_archetype {
//      typedef hds_traits<VertexListHDSConcept_archetype
//                                 >::vertex_iterator vertex_iterator;
//      typedef hds_traits<VertexHDSConcept_archetype
//                                 >::halfedge_descriptor halfedge_descriptor;
//      typedef hds_traits<VertexHDSConcept_archetype
//                                 >::vertex_descriptor vertex_descriptor;
//      typedef hds_traits<VertexListHDSConcept_archetype>::size_type size_type;
//    public:
//      vertex_iterator
//      vertices_begin(VertexListHDSConcept_archetype const& hds) const;
//      vertex_iterator
//      vertices_end(VertexListHDSConcept_archetype const& hds) const;
//      size_type
//      num_vertices(VertexListHDSConcept_archetype const& hds) const;
//  };
//..

#ifndef BOOST_HDSTL_VERTEX_LIST_HDS_CONCEPT_ARCHETYPE_HPP
#define BOOST_HDSTL_VERTEX_LIST_HDS_CONCEPT_ARCHETYPE_HPP

#include <boost/hdstl/hds_archetypes/vertex_hds_concept_archetype.hpp>
#include <boost/hdstl/hds_traits.hpp>
#include <boost/concept_archetype.hpp>

namespace boost {
namespace hdstl {

class VertexListHDSConcept_archetype;  // forward declaration

template <>
struct hds_traits<VertexListHDSConcept_archetype> {
    // This template specialization of 'hds_traits' for the
    // 'VertexListHDSConcept_archetype' provides the 'vertex_iterator'
    // and 'size_type' types.
    
    // TYPES
    typedef boost::default_constructible_archetype<
            boost::copy_constructible_archetype<
            boost::equality_comparable_archetype<
            boost::assignable_archetype<> > > > halfedge_descriptor;
        // Halfedge descriptor type for the 'VertexListHDSConcept' archetype.
    
    typedef boost::default_constructible_archetype<
            boost::copy_constructible_archetype<
            boost::equality_comparable_archetype<
            boost::assignable_archetype<> > > > vertex_descriptor;
        // Vertex descriptor type for the 'VertexHDSConcept' archetype.

    typedef boost::forward_iterator_archetype<vertex_descriptor> vertex_iterator;
        // Vertex iterator type for the 'VertexListHDSConcept' archetype.

    typedef int size_type;
        // Vertex size type for the 'VertexListHDSConcept' archetype.
    
    enum { supports_vertices = true };
    enum { supports_facets = false };
};

class VertexListHDSConcept_archetype : public VertexHDSConcept_archetype {
    // This class provides an exact implementation (no more, no less) of the
    // 'VertexListHDS' concept.  It can be used to instantiate class and
    // function templates that require their template arguments to be a model
    // of this concept in order to verify that there are no extra syntactic
    // requirements.

    // PRIVATE TYPES
    typedef hds_traits<VertexListHDSConcept_archetype
                               >::vertex_iterator vertex_iterator;
    
    typedef hds_traits<VertexHDSConcept_archetype
                               >::halfedge_descriptor halfedge_descriptor;

    typedef hds_traits<VertexHDSConcept_archetype
                               >::vertex_descriptor vertex_descriptor;
        //vertex_descriptor from the VertexHDSConcept used here.

    typedef hds_traits<VertexListHDSConcept_archetype>::size_type size_type;
    
  public:
    // MANIPULATORS
    vertex_iterator
    vertices_begin(VertexListHDSConcept_archetype const& hds) const;
        // Returns a 'vertex_iterator' pointing to the beginning of the 
        // 'vertex list'.

    vertex_iterator
    vertices_end(VertexListHDSConcept_archetype const& hds) const;
        // Returns a 'vertex_iterator' pointing to the end of the 
        // 'vertex list'.
    
    size_type
    num_vertices(VertexListHDSConcept_archetype const& hds) const;
        // Returns the number of vertices in the 'hds' data structure.
};

// MANIPULATORS
inline
hds_traits<VertexListHDSConcept_archetype>::vertex_iterator
vertices_begin(VertexListHDSConcept_archetype const& hds)
{
    (void)hds;  // eliminate unused variable warning
    return hds_traits<VertexListHDSConcept_archetype>::vertex_iterator();
}

inline
hds_traits<VertexListHDSConcept_archetype>::vertex_iterator
vertices_end(VertexListHDSConcept_archetype const& hds)
{
    (void)hds;  // eliminate unused variable warning
    return hds_traits<VertexListHDSConcept_archetype>::vertex_iterator();
}

inline
hds_traits<VertexListHDSConcept_archetype>::size_type
num_vertices(VertexListHDSConcept_archetype const& hds)
{
    (void)hds;  // eliminate unused variable warning
    return 0;
}

} // end namespace hdstl
} // end namespace boost

#endif
