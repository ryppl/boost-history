// mutable_forward_hds_concept_archetype.hpp   -*- C++ -*-
//
//@PURPOSE: Provide a 'MutableForwardHDS' concept archetype 
//
//@DESCRIPTION: This file provides an archetype class for compile-time testing
// of the 'MutableForwardHDSConcept' class.  Namely, this class can be used
// wherever a template parameter of a class or of a function template is
// required to be a model of the 'MutableForwardHDS' concept, in order to
// ensure that no further requirements are placed on the template parameter
// than are stated in the 'MutableForwardHDS' concept.
//
///Archetype class
///---------------
//..
//  template <typename ForwardCategory>
//  struct hds_traits<MutableForwardHDSConcept_archetype<ForwardCategory> > {
//      typedef boost::default_constructible_archetype<
//              boost::copy_constructible_archetype<
//              boost::equality_comparable_archetype<
//              boost::assignable_archetype<> > > > halfedge_descriptor;
//      typedef boost::convertible_to_archetype<forward_traversal_tag>
//                                                  traversal_category;
//      typedef ForwardCategory                     forward_category;
//      enum {supports_vertices = false};
//      enum {supports_facets = false};
//  };
//  template <typename ForwardCategory>
//  class MutableForwardHDSConcept_archetype 
//  : public ForwardHDSConcept_archetype<ForwardCategory>,
//    public MutableHDSConcept_archetype {
//      typedef typename hds_traits<HDSConcept_archetype
//                                 >::halfedge_descriptor halfedge_descriptor;
//      typedef typename hds_traits<ForwardHDSConcept_archetype<ForwardCategory>
//                                 >::traversal_category traversal_category;
//      typedef ForwardCategory                     forward_category;
//    public:
//      void
//      set_next_in_facet(halfedge_descriptor h, halfedge_descriptor g, 
//                                     MutableForwardHDSConcept_archetype& hds);
//      void
//      set_next_at_source(halfedge_descriptor h, halfedge_descriptor g, 
//                                     MutableForwardHDSConcept_archetype& hds);
//      void
//      set_next_at_target(halfedge_descriptor h, halfedge_descriptor g, 
//                                     MutableForwardHDSConcept_archetype& hds);
//  };
//..

#ifndef BOOST_HDSTL_MUTABLE_FORWARD_HDS_CONCEPT_ARCHETYPE_HPP
#define BOOST_HDSTL_MUTABLE_FORWARD_HDS_CONCEPT_ARCHETYPE_HPP

#include <boost/hdstl/hds_archetypes/forward_hds_concept_archetype.hpp>
#include <boost/hdstl/hds_archetypes/mutable_hds_concept_archetype.hpp>
#include <boost/hdstl/hds_traits.hpp>
#include <boost/concept_archetype.hpp>

namespace boost {
namespace hdstl {

template <typename ForwardCategory>
class MutableForwardHDSConcept_archetype;  // forward declaration

template <typename ForwardCategory>
struct hds_traits<MutableForwardHDSConcept_archetype<ForwardCategory> > {
    // This template specialization of 'hds_traits' for the
    // 'MutableForwardHDSConcept_archetype' provides the 'halfedge_descriptor' 
    // and 'traversal_category' types.
    
    // TYPES
    typedef boost::default_constructible_archetype<
            boost::copy_constructible_archetype<
            boost::equality_comparable_archetype<
            boost::assignable_archetype<> > > > halfedge_descriptor;
        // Halfedge descriptor type for the 'MutableForwardHDSConcept'
        // archetype.

    typedef boost::convertible_to_archetype<forward_traversal_tag>
                                                traversal_category;
        // This type, convertible to 'forward_traversal_tag', indicates that
        // the 'MutableForwardHDSConcept' archetype is a model of
        // 'MutableForwardHDSConcept'.

    typedef ForwardCategory                     forward_category;
        // This type, convertible to one or more of 'next_in_facet_tag',
        // 'next_at_source_tag', or 'next_at_target_tag', indicates which is
        // the primary accessor(s) for which the 'set_...' methods are defined.
    
    enum {supports_vertices = false};
    enum {supports_facets = false};
};

template <typename ForwardCategory>
class MutableForwardHDSConcept_archetype 
: virtual public ForwardHDSConcept_archetype<ForwardCategory>,
  virtual public MutableHDSConcept_archetype {
    // This class provides an exact implementation (no more, no less) of the
    // 'MutableForwardHDS' concept.  It can be used to instantiate class and
    // function templates that require their template arguments to be a model
    // of this concept in order to verify that there are no extra syntactic
    // requirements.

    // PRIVATE TYPES
    typedef typename hds_traits<MutableForwardHDSConcept_archetype<
                                       ForwardCategory> >::halfedge_descriptor 
                                                           halfedge_descriptor;
        //Halfedge descriptor type from 'HDSConcept' 

    typedef typename hds_traits<MutableForwardHDSConcept_archetype<
                                         ForwardCategory> >::traversal_category
                                                            traversal_category;
        //traversal category type from from 'ForwardHDSConcept', 
        //has to be convertible to 'forward_traversal_tag'.
    
    typedef ForwardCategory                     forward_category;
        // This type, convertible to one or more of 'next_in_facet_tag',
        // 'next_at_source_tag', or 'next_at_target_tag', indicates which is
        // the primary accessor(s) for which the 'set_...' methods are defined.
                               
  public:
    // MANIPULATORS
    void
    set_next_in_facet(halfedge_descriptor h, halfedge_descriptor g, 
                      MutableForwardHDSConcept_archetype<ForwardCategory>& hds);
        // set 'g' as the halfedge following 'h' in the (counter-clockwise)
        // facet cycle of 'h' in 'hds'.

    void
    set_next_at_source(halfedge_descriptor h, halfedge_descriptor g, 
                      MutableForwardHDSConcept_archetype<ForwardCategory>& hds);
        // set 'g' as the halfedge following 'h' in the (clockwise) vertex
        // cycle of the source of 'h' in 'hds'.

    void
    set_next_at_target(halfedge_descriptor h, halfedge_descriptor g, 
                      MutableForwardHDSConcept_archetype<ForwardCategory>& hds);
        // set 'g' as the halfedge following 'h' in the (clockwise) vertex
        // cycle of the target of 'h' in 'hds'.
};

// MANIPULATORS
template <typename ForwardCategory>
inline
void
set_next_in_facet(typename hds_traits<MutableForwardHDSConcept_archetype<
                                     ForwardCategory> >::halfedge_descriptor h, 
                  typename hds_traits<MutableForwardHDSConcept_archetype<
                                     ForwardCategory> >::halfedge_descriptor g, 
                   MutableForwardHDSConcept_archetype<ForwardCategory>& hds)
{
    (void)hds;  // eliminate unused variable warning
    (void)h;  // eliminate unused variable warning
    (void)g;  // eliminate unused variable warning
}

template <typename ForwardCategory>
inline
void
set_next_at_source(typename hds_traits<MutableForwardHDSConcept_archetype<
                                     ForwardCategory> >::halfedge_descriptor h, 
                  typename hds_traits<MutableForwardHDSConcept_archetype<
                                     ForwardCategory> >::halfedge_descriptor g, 
                   MutableForwardHDSConcept_archetype<ForwardCategory>& hds)
{
    (void)hds;  // eliminate unused variable warning
    (void)h;  // eliminate unused variable warning
    (void)g;  // eliminate unused variable warning
}

template <typename ForwardCategory>
inline
void
set_next_at_target(typename hds_traits<MutableForwardHDSConcept_archetype<
                                     ForwardCategory> >::halfedge_descriptor h, 
                  typename hds_traits<MutableForwardHDSConcept_archetype<
                                     ForwardCategory> >::halfedge_descriptor g, 
                   MutableForwardHDSConcept_archetype<ForwardCategory>& hds)
{
    (void)hds;  // eliminate unused variable warning
    (void)h;  // eliminate unused variable warning
    (void)g;  // eliminate unused variable warning
}

} // end namespace hdstl
} // end namespace boost

#endif
