// vertex_hds_concept_archetype.hpp   -*- C++ -*-
//
//@PURPOSE: Provide a 'VertexHDSConcept' archetype 
//
//@DESCRIPTION: This file provides an archetype class for compile-time testing
// of the 'VertexHDSConcept' class.  Namely, this class can be used wherever a
// template parameter of a class or of a function template is required to be a
// model of the 'VertexHDS' concept, in order to ensure that no further
// requirements are placed on the template parameter than are stated in the
// 'VertexHDS' concept.
//
///Archetype class
///---------------
//..
//  struct hds_traits<VertexHDSConcept_archetype> {
//      typedef boost::default_constructible_archetype<
//              boost::copy_constructible_archetype<
//              boost::equality_comparable_archetype<
//              boost::assignable_archetype<> > > > halfedge_descriptor;
//      typedef boost::default_constructible_archetype<
//              boost::copy_constructible_archetype<
//              boost::equality_comparable_archetype<
//              boost::assignable_archetype<> > > > vertex_descriptor;
//      enum { supports_facets   = false };
//      enum { supports_vertices = true };
//  };
//  class VertexHDSConcept_archetype : virtual public HDSConcept_archetype {
//      typedef hds_traits<VertexHDSConcept_archetype
//                                 >::vertex_descriptor vertex_descriptor;
//      typedef hds_traits<VertexHDSConcept_archetype
//                                 >::halfedge_descriptor halfedge_descriptor;
//    public:
//      vertex_descriptor
//     source(halfedge_descriptor, VertexHDSConcept_archetype const& hds) const;
//      vertex_descriptor
//     target(halfedge_descriptor, VertexHDSConcept_archetype const& hds) const;
//  };
//..

#ifndef BOOST_HDSTL_VERTEX_HDS_CONCEPT_ARCHETYPE_HPP
#define BOOST_HDSTL_VERTEX_HDS_CONCEPT_ARCHETYPE_HPP

#include <boost/hdstl/hds_archetypes/hds_concept_archetype.hpp>
#include <boost/hdstl/hds_traits.hpp>
#include <boost/concept_archetype.hpp>

namespace boost {
namespace hdstl {

class VertexHDSConcept_archetype;  // forward declaration

template<>
struct hds_traits<VertexHDSConcept_archetype> {
    // This template specialization of 'hds_traits' for the
    // 'VertexHDSConcept_archetype' provides the 'vertex_descriptor' type.

    // TYPES
    typedef boost::default_constructible_archetype<
            boost::copy_constructible_archetype<
            boost::equality_comparable_archetype<
            boost::assignable_archetype<> > > > halfedge_descriptor;
        // Halfedge descriptor type for the 'HDSConcept' archetype.

    typedef boost::default_constructible_archetype<
            boost::copy_constructible_archetype<
            boost::equality_comparable_archetype<
            boost::assignable_archetype<> > > > vertex_descriptor;
        // Vertex descriptor type for the 'VertexHDSConcept' archetype.
    
    enum { supports_facets   = false };
    enum { supports_vertices = true };
};

class VertexHDSConcept_archetype : virtual public HDSConcept_archetype {
    // This archetype class for the 'VertexHDSConcept' class can be used
    // wherever a template parameter of a class or of a function template is
    // required to be a model of the 'VertexHDS' concept, in order to ensure
    // that no further requirements are placed on the template parameter than
    // are stated in the 'VertexHDS' concept.

    // PRIVATE TYPES
    typedef hds_traits<VertexHDSConcept_archetype
                               >::vertex_descriptor vertex_descriptor;
    typedef hds_traits<VertexHDSConcept_archetype
                               >::halfedge_descriptor halfedge_descriptor;

  public:
    // MANIPULATORS
    vertex_descriptor
    source(halfedge_descriptor, VertexHDSConcept_archetype const& hds) const;
        // returns the source vertex descriptor of 'h' in 'hds'
    
    vertex_descriptor
    target(halfedge_descriptor, VertexHDSConcept_archetype const& hds) const;
        // returns the target vertex descriptor of 'h' in 'hds'
};

//MANIPULATORS
inline
hds_traits<VertexHDSConcept_archetype>::vertex_descriptor
source(hds_traits<VertexHDSConcept_archetype>::halfedge_descriptor, 
                                   VertexHDSConcept_archetype const& hds)
{
    (void)hds;  // eliminate unused variable warning
    return hds_traits<VertexHDSConcept_archetype>::vertex_descriptor();
}

inline
hds_traits<VertexHDSConcept_archetype>::vertex_descriptor
target(hds_traits<VertexHDSConcept_archetype>::halfedge_descriptor, 
                                   VertexHDSConcept_archetype const& hds) 
{
    (void)hds;  // eliminate unused variable warning
    return hds_traits<VertexHDSConcept_archetype>::vertex_descriptor();
}

} // end namespace hdstl
} // end namespace boost

#endif
