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
//  template<>
//  struct hds_traits<MutableHDSConcept_archetype> {
//      typedef boost::default_constructible_archetype<
//              boost::copy_constructible_archetype<
//              boost::equality_comparable_archetype<
//              boost::assignable_archetype<> > > > halfedge_descriptor;
//      enum { supports_facets   = false };
//      enum { supports_vertices = false };
//  };
//  class MutableHDSConcept_archetype : public HDSConcept_archetype {
//      typedef hds_traits<MutableHDSConcept_archetype
//                                 >::halfedge_descriptor halfedge_descriptor;
//    public:
//      halfedge_descriptor& new_edge(MutableHDSConcept_archetype&);
//      void delete_edge(halfedge_descriptor h, MutableHDSConcept_archetype&);
//  };
//..

#ifndef BOOST_HDSTL_MUTABLE_HDS_CONCEPT_ARCHETYPE_HPP
#define BOOST_HDSTL_MUTABLE_HDS_CONCEPT_ARCHETYPE_HPP

#include <boost/hdstl/hds_archetypes/hds_concept_archetype.hpp>
#include <boost/hdstl/hds_traits.hpp>
#include <boost/concept_archetype.hpp>

namespace boost {
namespace hdstl {

class MutableHDSConcept_archetype;  // forward declaration

template<>
struct hds_traits<MutableHDSConcept_archetype> {
    // This template specialization of 'hds_traits' for the
    // 'MutableHDSConcept_archetype' provides the 'halfedge_descriptor' type.

    // TYPES
    typedef boost::default_constructible_archetype<
            boost::copy_constructible_archetype<
            boost::equality_comparable_archetype<
            boost::assignable_archetype<> > > > halfedge_descriptor;
        // Halfedge descriptor type for the 'MutableHDSConcept' archetype.

    enum { supports_facets   = false };
    enum { supports_vertices = false };

};

class MutableHDSConcept_archetype : virtual public HDSConcept_archetype {
    // This archetype class for the 'MutableHDSConcept' class can be used
    // wherever a template parameter of a class or of a function template is
    // required to be a model of the 'MutableHDS' concept, in order to ensure
    // that no further requirements are placed on the template parameter than
    // are stated in the 'MutableHDS' concept.

    // PRIVATE TYPES
    typedef hds_traits<MutableHDSConcept_archetype
                               >::halfedge_descriptor halfedge_descriptor;

  public:
    // MANIPULATORS
    halfedge_descriptor& new_edge(MutableHDSConcept_archetype&);
       // add two halfedges to the data structure.
    
    void delete_edge(halfedge_descriptor h, MutableHDSConcept_archetype&);
      // delete two halfedges from the data structure.
};

// MANIPULATORS
inline
hds_traits<MutableHDSConcept_archetype>::halfedge_descriptor
new_edge(MutableHDSConcept_archetype&)
{
    return hds_traits<MutableHDSConcept_archetype>::halfedge_descriptor();
}

inline
void 
delete_edge(hds_traits<MutableHDSConcept_archetype>::halfedge_descriptor&, 
                                                   MutableHDSConcept_archetype&)
{
}

} // end namespace hdstl
} // end namespace boost

#endif
