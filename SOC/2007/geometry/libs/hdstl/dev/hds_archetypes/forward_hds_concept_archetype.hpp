// forward_hds_concept_archetype.hpp   -*- C++ -*-
//
//@PURPOSE: Provide a 'ForwardHDS' concept archetype 
//
//@DESCRIPTION: This file provides an archetype class for compile-time testing
// of the 'ForwardHDSConcept' class.  Namely, this class can be used wherever a
// template parameter of a class or of a function template is required to be a
// model of the 'ForwardHDS' concept, in order to ensure that no further
// requirements are placed on the template parameter than are stated in the
// 'ForwardHDS' concept.
//
///Archetype class
///---------------
//..
// class ForwardHDSConcept_archetype
//    :HDSConcept_archetype {
//    public:
//       halfedge_descriptor& next_in_faccet(ForwardHDSConcept_archetype,
//                                               halfedge_descriptor&) const{};
//       halfedge_descriptor& next_at_source(ForwardHDSConcept_archetype,
//                                               halfedge_descriptor&) const{};
//       halfedge_descriptor& next_at_target(ForwardHDSConcept_archetype,
//                                               halfedge_descriptor&) const{};
//
// };
// template<>
// class hds_traits<ForwardHDSConcept_archetype> {
//    struct halfedge_descriptor 
//    :public boost::default_constructible_archetype,
//            boost::copy_constructible_archetype,
//            boost::equality_comparable_archetype,
//            boost::assignable_archetype {};
//
//    struct traversal_category 
//    :public boost::convertible_to_archetype<forward_traversal_tag>
//       {};
// };
//..

#ifndef BOOST_HDSTL_FORWARD_HDS_CONCEPT_ARCHETYPE_HPP
#define BOOST_HDSTL_FORWARD_HDS_CONCEPT_ARCHETYPE_HPP

#include <boost/hds_archetype/hds_concept_archetypes.hpp>

namespace boost {
namespace hdstl {

template <typename ForwardCategory>
class ForwardHDSConcept_archetype;  // forward declaration

template <typename ForwardCategory>
struct hds_traits<ForwardHDSConcept_archetype<ForwardCategory> > {
    // This template specialization of 'hds_traits' for the
    // 'ForwardHDSConcept_archetype' provides the 'halfedge_descriptor' and
    // 'traversal_category' types.
    
    // TYPES
    typedef boost::default_constructible_archetype<
            boost::copy_constructible_archetype<
            boost::equality_comparable_archetype<
            boost::assignable_archetype<> > > > halfedge_descriptor;
        // Halfedge descriptor type for the 'ForwardHDSConcept' archetype.

    typedef boost::convertible_to_archetype<forward_traversal_tag>
                                                traversal_category;
        // This type, convertible to 'forward_traversal_tag', indicates that
        // the 'ForwardHDSConcept' archetype is a model of 'ForwardHDSConcept'.

    typedef ForwardCategory                     forward_category;
        // This type, convertible to one or more of 'next_in_facet_tag',
        // 'next_at_source_tag', or 'next_at_target_tag', indicates which is
        // the primary accessor(s) for which the 'set_...' methods are defined.
};


template <typename ForwardCategory>
class ForwardHDSConcept_archetype : public HDSConcept_archetype {
    // This class provides an exact implementation (no more, no less) of the
    // 'ForwardHDS' concept.  It can be used to instantiate class and function
    // templates that require their template arguments to be a model of this
    // concept in order to verify that there are no extra syntactic
    // requirements.

    // PRIVATE TYPES
    typedef typename hds_traits<ForwardHDSConcept_archetype
                               >::halfedge_descriptor halfedge_descriptor;

    // NOT IMPLEMENTED
    ForwardHDSConcept_archetype();
    ForwardHDSConcept_archetype(const ForwardHDSConcept_archetype&);

  public:
    // MANIPULATORS
    halfedge_descriptor
    next_in_facet(ForwardHDSConcept_archetype const& hds,
                  halfedge_descriptor                h) const;
        // Return the halfedge following 'h' in the (counter-clockwise) facet
        // cycle of 'h' in 'hds'.

    halfedge_descriptor
    next_at_source(ForwardHDSConcept_archetype const& hds,
                   halfedge_descriptor                h) const;
        // Returns the halfedge following 'h' in the (clockwise) vertex
        // cycle of the source of 'h' in 'hds'.

     halfedge_descriptor
     next_at_target(ForwardHDSConcept_archetype const& hds,
                    halfedge_descriptor                h) const;
        // Returns the halfedge following 'h' in the (clockwise) vertex
        // cycle of the target of 'h' in 'hds'.
};

// MANIPULATORS
template <typename ForwardCategory>
typename hds_traits<ForwardHDSConcept_archetype>::halfedge_descriptor
ForwardHDSConcept_archetype<ForwardCategory>::next_in_facet(
                                        ForwardHDSConcept_archetype const& hds,
                                        halfedge_descriptor                h)
{
    return halfedge_descriptor();
}

template <typename ForwardCategory>
typename hds_traits<ForwardHDSConcept_archetype>::halfedge_descriptor
ForwardHDSConcept_archetype<ForwardCategory>::next_at_source(
                                        ForwardHDSConcept_archetype const& hds,
                                        halfedge_descriptor                h)
{
    return halfedge_descriptor();
}

template <typename ForwardCategory>
typename hds_traits<ForwardHDSConcept_archetype>::halfedge_descriptor
ForwardHDSConcept_archetype<ForwardCategory>::next_at_target(
                                        ForwardHDSConcept_archetype const& hds,
                                        halfedge_descriptor                h)
{
    return halfedge_descriptor();
}

#endif
