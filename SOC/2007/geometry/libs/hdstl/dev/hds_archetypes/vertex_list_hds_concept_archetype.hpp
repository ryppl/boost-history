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
//      typedef hdstl_detail::MultiPassInputIteratorConcept<> vertex_iterator;
//      typedef size_type                      size_type;
//  };
//  class VertexListHDSConcept_archetype : public HDSConcept_archetype, 
//                                        public VertexHDSConcept_archetype {
//      typedef typename hds_traits<VertexListHDSConcept_archetype
//                                 >::vertex_iterator vertex_iterator;
//      typedef typename hds_traits<VertexHDSConcept_archetype
//                                 >::vertex_descriptor vertex_descriptor;
//      typedef typename hds_traits<VertexListHDSConcept_archetype
//                                 >::size_type size_type;
//      VertexListHDSConcept_archetype();
//      VertexListHDSConcept_archetype(const VertexListHDSConcept_archetype&);
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

#include <boost/hds_archetype/hds_concept_archetypes.hpp>

namespace boost {
namespace hdstl {

class VertexListHDSConcept_archetype;  // forward declaration

template <>
struct hds_traits<VertexListHDSConcept_archetype> {
    // This template specialization of 'hds_traits' for the
    // 'VertexListHDSConcept_archetype' provides the 'vertex_iterator'
    // and 'size_type' types.
    
    // TYPES
    typedef hdstl_detail::MultiPassInputIteratorConcept<> vertex_iterator;
        // Vertex iterator type for the 'VertexListHDSConcept' archetype.

    typedef size_type                      size_type;
        // Vertex size type for the 'VertexListHDSConcept' archetype.
};


class VertexListHDSConcept_archetype : public HDSConcept_archetype, 
                                      public VertexHDSConcept_archetype {
    // This class provides an exact implementation (no more, no less) of the
    // 'VertexListHDS' concept.  It can be used to instantiate class and
    // function templates that require their template arguments to be a model
    // of this concept in order to verify that there are no extra syntactic
    // requirements.

    // PRIVATE TYPES
    typedef typename hds_traits<VertexListHDSConcept_archetype
                               >::vertex_iterator vertex_iterator;
    
    typedef typename hds_traits<VertexHDSConcept_archetype
                               >::vertex_descriptor vertex_descriptor;
        //vertex_descriptor from the VertexHDSConcept used here.

    typedef typename hds_traits<VertexListHDSConcept_archetype
                               >::size_type size_type;
    
    // NOT IMPLEMENTED
    VertexListHDSConcept_archetype();
    VertexListHDSConcept_archetype(const VertexListHDSConcept_archetype&);

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
typename hds_traits<VertexListHDSConcept_archetype>::vertex_iterator
VertexListHDSConcept_archetype<>::vertices_begin(
                               VertexListHDSConcept_archetype const& hds) const
{
    return vertex_iterator();
}

typename hds_traits<VertexListHDSConcept_archetype>::vertex_iterator
VertexListHDSConcept_archetype<>::vertices_end(
                               VertexListHDSConcept_archetype const& hds) const
{
    return vertex_iterator();
}

typename hds_traits<VertexListHDSConcept_archetype>::size_type
VertexListHDSConcept_archetype<>::num_vertices(
                               VertexListHDSConcept_archetype const& hds) const
{
    return size_type();
}

} // end namespace hdstl
} // end namespace boost

#endif
