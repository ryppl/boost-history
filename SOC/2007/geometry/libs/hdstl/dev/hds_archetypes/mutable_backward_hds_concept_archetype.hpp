// mutable_backward_hds_concept_archetype.hpp   -*- C++ -*-
//
//@PURPOSE: Provide a 'MutableBackwardHDS' concept archetype 
//
//@DESCRIPTION: This file provides an archetype class for compile-time testing
// of the 'MutableBackwardHDSConcept' class.  Namely, this class can be used
// wherever a template parameter of a class or of a function template is
// required to be a model of the 'MutableBackwardHDS' concept, in order to
// ensure that no further requirements are placed on the template parameter
// than are stated in the 'MutableBackwardHDS' concept.
//
///Archetype class
///---------------
//..
//  template <typename BackwardCategory>
//  struct hds_traits<MutableBackwardHDSConcept_archetype<BackwardCategory> > {
//      typedef boost::default_constructible_archetype<
//              boost::copy_constructible_archetype<
//              boost::equality_comparable_archetype<
//              boost::assignable_archetype<> > > > halfedge_descriptor;
//      typedef boost::convertible_to_archetype<backward_traversal_tag>
//                                                  traversal_category;
//      typedef BackwardCategory                     backward_category;
//      enum {supports_vertices = false};
//      enum {supports_facets = false};
//  };
//  template <typename BackwardCategory>
//  class MutableBackwardHDSConcept_archetype 
//  : public BackwardHDSConcept_archetype<BackwardCategory>,
//    public MutableHDSConcept_archetype {
//      typedef typename hds_traits<MutableBackwardHDSConcept_archetype<
//                               BackwardCategory> >::halfedge_descriptor
//                                                          halfedge_descriptor;
//      typedef typename hds_traits<MutableBackwardHDSConcept_archetype<
//                                  BackwardCategory> >::traversal_category 
//                                                          traversal_category;
//      typedef BackwardCategory                     backward_category;
//    public:
//      void
//      set_prev_in_facet(halfedge_descriptor h, halfedge_descriptor g, 
//                                    MutableBackwardHDSConcept_archetype& hds);
//      void
//      set_prev_at_source(halfedge_descriptor h, halfedge_descriptor g, 
//                                    MutableBackwardHDSConcept_archetype& hds);
//      void
//      set_prev_at_target(halfedge_descriptor h, halfedge_descriptor g, 
//                                    MutableBackwardHDSConcept_archetype& hds);
//  };
//..

#ifndef BOOST_HDSTL_MUTABLE_BACKWARD_HDS_CONCEPT_ARCHETYPE_HPP
#define BOOST_HDSTL_MUTABLE_BACKWARD_HDS_CONCEPT_ARCHETYPE_HPP

#include <boost/hdstl/hds_archetypes/backward_hds_concept_archetype.hpp>
#include <boost/hdstl/hds_archetypes/mutable_hds_concept_archetype.hpp>
#include <boost/hdstl/hds_traits.hpp>
#include <boost/concept_archetype.hpp>

namespace boost {
namespace hdstl {

template <typename BackwardCategory>
class MutableBackwardHDSConcept_archetype;  // forward declaration

template <typename BackwardCategory>
struct hds_traits<MutableBackwardHDSConcept_archetype<BackwardCategory> > {
    // This template specialization of 'hds_traits' for the
    // 'MutableBackwardHDSConcept_archetype' provides the 'halfedge_descriptor' 
    // and 'traversal_category' types.
    
    // TYPES
    typedef boost::default_constructible_archetype<
            boost::copy_constructible_archetype<
            boost::equality_comparable_archetype<
            boost::assignable_archetype<> > > > halfedge_descriptor;
        // Halfedge descriptor type for the 'MutableBackwardHDSConcept'
        // archetype.

    typedef boost::convertible_to_archetype<backward_traversal_tag>
                                                traversal_category;
        // This type, convertible to 'backward_traversal_tag', indicates that
        // the 'MutableBackwardHDSConcept' archetype is a model of
        // 'MutableBackwardHDSConcept'.

    typedef BackwardCategory                     backward_category;
        // This type, convertible to one or more of 'prev_in_facet_tag',
        // 'prev_at_source_tag', or 'prev_at_target_tag', indicates which is
        // the primary accessor(s) for which the 'set_...' methods are defined.
    
    enum {supports_vertices = false};
    enum {supports_facets = false};
};

template <typename BackwardCategory>
class MutableBackwardHDSConcept_archetype 
: virtual public BackwardHDSConcept_archetype<BackwardCategory>,
  virtual public MutableHDSConcept_archetype {
    // This class provides an exact implementation (no more, no less) of the
    // 'MutableBackwardHDS' concept.  It can be used to instantiate class and
    // function templates that require their template arguments to be a model
    // of this concept in order to verify that there are no extra syntactic
    // requirements.

    // PRIVATE TYPES
    typedef typename hds_traits<MutableBackwardHDSConcept_archetype<
                                BackwardCategory> >::halfedge_descriptor
                                                            halfedge_descriptor;
        //Halfedge descriptor type from 'MutableBackwardHDSConcept' 

    typedef typename hds_traits<MutableBackwardHDSConcept_archetype<
                                BackwardCategory> >::traversal_category 
                                                            traversal_category;
        //traversal category type from from 'MutableBackwardHDSConcept', 
        //has to be convertible to 'backward_traversal_tag'.
    
    typedef BackwardCategory                     backward_category;
        // This type, convertible to one or more of 'prev_in_facet_tag',
        // 'prev_at_source_tag', or 'prev_at_target_tag', indicates which is
        // the primary accessor(s) for which the 'set_...' methods are defined.
                               
  public:
    // MANIPULATORS
    void
    set_prev_in_facet(halfedge_descriptor h, halfedge_descriptor g, 
                    MutableBackwardHDSConcept_archetype<BackwardCategory>& hds);
        // set 'g' as the halfedge preceding 'h' in the (counter-clockwise)
        // facet cycle of 'h' in 'hds'.

    void
    set_prev_at_source(halfedge_descriptor h, halfedge_descriptor g, 
                    MutableBackwardHDSConcept_archetype<BackwardCategory>& hds);
        // set 'g' as the halfedge preceding 'h' in the (clockwise) vertex
        // cycle of the source of 'h' in 'hds'.

    void
    set_prev_at_target(halfedge_descriptor h, halfedge_descriptor g, 
                    MutableBackwardHDSConcept_archetype<BackwardCategory>& hds);
        // set 'g' as the halfedge preceding 'h' in the (clockwise) vertex
        // cycle of the target of 'h' in 'hds'.
};

// MANIPULATORS
template <typename BackwardCategory>
inline
void
set_prev_in_facet(typename hds_traits<MutableBackwardHDSConcept_archetype<
                                    BackwardCategory> >::halfedge_descriptor h, 
                  typename hds_traits<MutableBackwardHDSConcept_archetype<
                                    BackwardCategory> >::halfedge_descriptor g, 
                  MutableBackwardHDSConcept_archetype<BackwardCategory>& hds)
{
    (void)hds;  // eliminate unused variable warning
    (void)h;  // eliminate unused variable warning
    (void)g;  // eliminate unused variable warning
}

template <typename BackwardCategory>
inline
void
set_prev_at_source(typename hds_traits<MutableBackwardHDSConcept_archetype<
                                    BackwardCategory> >::halfedge_descriptor h, 
                  typename hds_traits<MutableBackwardHDSConcept_archetype<
                                    BackwardCategory> >::halfedge_descriptor g, 
                  MutableBackwardHDSConcept_archetype<BackwardCategory>& hds)
{
    (void)hds;  // eliminate unused variable warning
    (void)h;  // eliminate unused variable warning
    (void)g;  // eliminate unused variable warning
}

template <typename BackwardCategory>
inline
void
set_prev_at_target(typename hds_traits<MutableBackwardHDSConcept_archetype<
                                    BackwardCategory> >::halfedge_descriptor h, 
                  typename hds_traits<MutableBackwardHDSConcept_archetype<
                                    BackwardCategory> >::halfedge_descriptor g, 
                  MutableBackwardHDSConcept_archetype<BackwardCategory>& hds)
{
    (void)hds;  // eliminate unused variable warning
    (void)h;  // eliminate unused variable warning
    (void)g;  // eliminate unused variable warning
}

} // end namespace hdstl
} // end namespace boost

#endif
