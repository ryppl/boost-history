//facet_selectors.hpp   -*- C++ -*-
//
//@PURPOSE: Provide selector classes for 'FacetS'.
//
//@CLASSES:
//        'facetS':  defines configuration options for facet storage and access
//  'stored_facet':  defines the storage for a given facet configuration
//     'facet_gen':  defines the stored type and accessors for a facet configuration
//  
//@SEE_ALSO: {hds_concepts.hpp, halfedge_ds.hpp}
//
//@DESCRIPTION: This file provides the 'facetS' selector classes for
// configurations related to facet properties.  These selector types are
// suitable to use for the 'FacetS' template parameter to the 'halfedge_ds'
// class template.  Currently, there are two facet selectors:
//
// - 'noFacetS': means that there should be no facets in the corresponding
//   'hafedge_ds' data structure, and
//
// - 'facetS<ContainerS, HasfacetLink>': (where 'ContainerS' can be any of the
//    types defined by the 'container_selectors' component, such as 'listS',
//    'vecS', 'setS' or 'hashS') means that there should be facets stored in
//    that kind of containers within the 'halfedge_ds' data structure, and
//    whether the facets must have a link to one of the halfedges in their
//    facet cycles or not.
//
// As a consequence, a 'halfedge_ds<..., ..., facetS<...> >' data structure
// (where the actual selectors used in '...' do not matter) will model the
// 'FacetListHDS' concept.
//
///Usage
///-----
// Supposed we want to design a stored facet from a facet selector.  A stored
// facet needs to know:
// - whether it stores a facet link or not.
// - the halfedge descriptor:  in order to store the facet link.  Since it is
//   not needed when the facet stores no facet link, we make it integer in that
//   case.
// - any extra properties that need to get contained by the facet as member. We
//   make this a parameterized base class.
//
// We therefore have the following general implementation.
//..
//  template <typename Base,
//            bool HasVertexLink = false, 
//            typename HalfedgeDescriptor = int>
//  struct stored_facet : public Base {
//      // This 'struct' implements a stored facet, deriving from the
//      // parameterized 'Base', and adding a facet link if 'HasFacetLink' is
//      // true.
//
//      // DATA
//      HalfedgeDescriptor m_facetLink;
//
//      // CREATORS
//      stored_facet(Base const& base, HalfedgeDescriptor facetLink)
//      : Base(base)
//      , m_facetLink(facetLink) {}
//  };
//..
// Since the facetLink is stored or not, we encapsulate this in a specialization.
//..
//  template <typename Base>
//  struct stored_facet : public Base {
//      // This specialization implements a stored facet, deriving from the
//      // parameterized 'Base', but without storing a facet link although its
//      // constructor accepts a null facet link for uniformity of the
//      // interface with the general definition.
//
//      // CREATORS
//      stored_facet(Base const& base, HalfedgeDescriptor = 0)
//      : Base(base) {}
//  };
//..
// Now, we can further define the facet generator: it takes a facet selector
// and a halfedge descriptor, and uses the 'container_selectors' component to
// define a suitable base for the 'halfedge_ds_gen' that implements all the
// functionality of the 'halfedge_ds' related to the facets.
//..
//  template <typename HalfedgeDescriptor, typename FacetBase>
//  struct facet_gen {
//      // This 'struct' implements a facet generator.  The general definition does
//      // nothing.
//  };
//..
// The proper facet generator is defined only when 'FacetS' is of the form
// 'facetS<...>':
//..
//  // SPECIALIZATIONS
//  template <typename FacetS, typename HalfedgeDescriptor = void, typename FacetBase = void>
//  struct facet_gen<facetS<ContainerS, HasFacetLink>, HalfedgeDescriptor, FacetBase> {
//      // This specialization of a facet generator for 'facetS' takes a facet selector
//      // and a halfedge descriptor, and uses the 'container_selectors' component
//      // to define a suitable base for the 'halfedge_ds_gen' that implements all
//      // the functionality of the 'halfedge_ds' related to the facets.
//  
//      // TYPES
//      typedef container_gen<ContainerS>           ContainerGen;
//          // The container generator for this facet generator.
//
//      typedef stored_facet<FacetBase, HasfacetLink, HalfedgeDescriptor>
//                                                  facet_type;
//          // The stored facet type for this facet generator.
//  
//      typedef typename container_gen<ContainerS, facet_type>::type
//                                                  container_type;
//          // The facet container type for this facet generator.
//  
//      typedef typename container_type::descriptor facet_descriptor;
//          // The facet descriptor type for this facet generator.
//
//      typedef typename container_type::iterator   facet_iterator;
//          // The facet iterator type for this facet generator.
//  
//      // DATA
//      container_type m_container;
//          // The actual container for the facets, from which facets can be obtained.
//  };
//..

#ifndef BOOST_HDSTL_FACET_SELECTORS_HPP
#define BOOST_HDSTL_FACET_SELECTORS_HPP 1

#include <boost/hdstl/halfedge_ds/container_selectors.hpp>

namespace boost {
namespace hdstl {

                 // ===========================
                 // classes noFacetS and facetS
                 // ===========================

struct noFacetS {
    // A selector that tells the 'halfedge_ds' that it should not store facets.
};

template<typename ContainerS, bool HasFacetLink = false>
struct facetS {
    // A selector that tells the 'halfedge_ds' that it should store facets in a
    // container selected by the specified 'Selector', and that those facets
    // should have a link to one of the halfedges in their facet cycle or not,
    // depending on the specified 'HasFacetLink' boolean template parameter.

    // TYPES
    typedef ContainerS   container_selector;
        // A selector (see the component 'container_selector') for the type of
        // container that
        // will be used to store the halfedge.

    enum { has_facet_links = HasFacetLink };
        // A boolean enumeration that tells whether the stored halfedge should
        // contain a link to one of the halfedges in its facet cycle or not.
};

                 // ==================
                 // class stored_facet
                 // ==================

template <typename Base,
          bool HasFacetLink = false, 
          typename HalfedgeDescriptor = int>
struct stored_facet {
    // This 'struct' implements a stored facet, from the parameterized 'Base'
    // (if not void), and adding a facet link if 'HasFacetLink' is true.

    // DATA
    HalfedgeDescriptor m_facetLink;
    Base m_base;

    // CREATORS
    stored_facet() {}

    stored_facet(Base const& base, HalfedgeDescriptor facetLink = 0)
    : m_facetLink(facetLink)
    , m_base(base) {}

    // MANIPULATORS
    Base& base() { return m_base; }

    // ACCESSORS
    const Base& base() const { return m_base; }
};

template <typename Base, typename HalfedgeDescriptor>
struct stored_facet<Base, false, HalfedgeDescriptor> {
    // This partial specialization implements a stored facet, deriving from the
    // parameterized 'Base', but without storing a facet link although its
    // constructor accepts a null facet link for uniformity of the
    // interface with the general definition.

    // DATA
    Base m_base;

    // CREATORS
    stored_facet() {}

    stored_facet(Base const& base, HalfedgeDescriptor = 0)
    : m_base(base) {}

    // MANIPULATORS
    Base& base() { return m_base; }

    // ACCESSORS
    const Base& base() const { return m_base; }
};

template <bool HasFacetLink,
          typename HalfedgeDescriptor>
struct stored_facet<void, HasFacetLink, HalfedgeDescriptor> {
    // This 'struct' implements a stored facet, with a 'void' base, adding a
    // facet link if 'HasFacetLink' is true.

    // DATA
    HalfedgeDescriptor m_facetLink;

    // CREATORS
    stored_facet() {}

    stored_facet(HalfedgeDescriptor facetLink)
    : m_facetLink(facetLink) {}
};

template <typename HalfedgeDescriptor>
struct stored_facet<void, false, HalfedgeDescriptor> {
    // This partial specialization implements a stored facet, with a 'void' 
    // base, but without storing a facet link although its
    // constructor accepts a null facet link for uniformity of the
    // interface with the general definition.

    // CREATORS
    stored_facet(HalfedgeDescriptor = 0) {}
};

                 // ===============
                 // class facet_gen
                 // ===============

template <typename FacetS, typename HalfedgeDescriptor, typename FacetBase>
struct facet_gen {
    // This 'struct' implements a facet generator.  The general definition does
    // nothing.
};

// SPECIALIZATIONS
template <typename ContainerS, bool HasFacetLink, 
          typename HalfedgeDescriptor, typename FacetBase>
struct facet_gen<facetS<ContainerS,HasFacetLink>, HalfedgeDescriptor, FacetBase> {
    // This specialization of a facet generator for 'facetS' takes a facet selector
    // and a halfedge descriptor, and uses the 'container_selectors' component
    // to define a suitable base for the 'halfedge_ds_gen' that implements all
    // the functionality of the 'halfedge_ds' related to the facets.

    // TYPES
    typedef facetS<ContainerS,HasFacetLink>       facet_selector;
        // The facet selector.

    typedef stored_facet<FacetBase, HasFacetLink, HalfedgeDescriptor>
                                                facet_type;
        // The stored facet type for this facet generator.
    
    typedef container_gen<ContainerS, facet_type> ContainerGen;
        // The container generator for this facet generator.

    typedef typename ContainerGen::type           container_type;
        // The facet container type for this facet generator.

    typedef typename ContainerGen::size_type      size_type;
        // The facet container size type for this facet generator.

    typedef typename ContainerGen::descriptor     facet_descriptor;
        // The facet descriptor type for this facet generator.

    typedef typename ContainerGen::iterator       facet_iterator;
        // The facet iterator type for this facet generator.

    // DATA
    container_type m_container;
        // The actual container for the facets, from which facets can be obtained.
};


} // namespace hdstl
} // namespace boost

#endif
