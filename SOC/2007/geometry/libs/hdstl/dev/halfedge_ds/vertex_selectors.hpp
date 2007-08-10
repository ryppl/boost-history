//vertex_selectors.hpp   -*- C++ -*-
//
//@PURPOSE: Provide selector classes for 'VertexS'.
//
//@CLASSES:
//        'vertexS': defines configuration options for vertex storage and access
//  'stored_vertex': defines the storage for a given vertex configuration
//     'vertex_gen': defines the stored type and accessors for a vertex configuration
//
//@SEE_ALSO: {hds_concepts.hpp, halfedge_ds.hpp}
//
//@DESCRIPTION: This file provides the 'vertexS' selector class for
//configurations related to vertex properties. These selector types are
//suitable to use for the 'VertexS' template parameter to the 'halfedge_ds' 
//class template. Currently there are two vertex selectors:
//
//  - 'noVertexS': means that there should be no vertices in the corresponding
//    'hafedge_ds' data structure.
//
//  - 'vertexS<ContainerS, HasVertexLink, VertexType>': (where 'ContainerS' can
//    be any of the types defined by the 'container_selectors' component, such 
//    as 'listS', 'vecS', 'setS' or 'hashS') means that there should be
//    vertices stored in that kind of containers within the 'halfedge_ds' data
//    structure, and whether the vertices must have a link to one of the
//    halfedges in their vertex cycles defined by 'VertexType' or not.
//
//  - 'sourceS':  default vertex access method for halfedges is set to source
// 
//  - 'targetS':  default vertex access method for halfedges is set to target
//  
//  - 'base_vertex': base class to select the method of access between source 
//    and target
//
// As a consequence, a 'halfedge_ds<..., ..., vertexS<...> >' data structure
// (where the actual selectors used in '...' do not matter) will model the
// 'VertexListHDS' concept.
//
///Usage
///-----
// Supposed we want to design a stored vertex from a vertex selector.  A stored
// vertex needs to know:
// - whether halfedge_ds should store a link to source or target vertex.
// - whether it stores a vertex link or not.
// - the halfedge descriptor: in order to store the vertex link.  Since it is
//   not needed when the vertex stores no vertex link, we make it integer in 
//   that case.
// - any extra properties that need to get contained by the vertex as member. We
//   make this a parameterized base class.
//
// We therefore have the following general implementation.
//..
//  template <bool HasVertexLink = false, typename HalfedgeDescriptor = int>
//  struct stored_vertex {
//      // This struct implements a stored vertex with a vertex link if
//      // 'HasVertexLink' is true

//      //DATA
//      HalfedgeDescriptor m_vertexLink;

//      //CREATORS
//      stored_vertex(HalfedgeDescriptor vertexLink) : m_vertexLink(vertexLink){
//      }
//  };
//..
// Since the vertexLink is stored or not, we encapsulate this in a 
// specialization.
//..
//  template <typename HalfedgeDescriptor>
//  struct stored_vertex<false, HalfedgeDescriptor> {
//     // This partial specialization implements a stored vertex without storing
//      // a vertex link although its constuctoe accepts a null vertex link for 
//      // uniformity of the interface with the general definition.
//
//      // CREATORS
//      stored_vertex(HalfedgeDescriptor = 0) {
//      }
//  };
//..
// We use 'base_vertexS' to select the vertex access method. Base class defines
// nothing.
//..
//  template <typename VertexType>
//  struct base_vertexS {
//  // A selector that specializes whether source or target vertices are stored.
//  };
//..
//  We specialize with sourceS to tell the halfedge_ds that it should keep 
//  links to source vertices.
//..
//  template<>
//  struct base_vertexS<sourceS>{
//  // A specialized selector that tells the 'halfedge_ds' that source vertices
//      // should be stored.
//
//      // TYPES
//      enum { type = true };
//      enum { is_source = true };
//      enum { is_target = false };
//  };
//..
//  or we specialize with targetS to tell the halfedge_ds that it should keep 
//  links to target vertices.
//..
//
//  template<>
//  struct base_vertexS<targetS>{
//  // A specialized selector that tells the 'halfedge_ds' that target vertices
//      // should be stored.
//
//      // TYPES
//      enum { type = false };
//      enum { is_source = false };
//      enum { is_target = true };
//  };
//..
// Now, we can further define the vertex generator: it takes a vertex selector
// and a halfedge descriptor, and uses the 'container_selectors' component to
// define a suitable base for the 'halfedge_ds_gen' that implements all the
// functionality of the 'halfedge_ds' related to the vertices.
//..
//  template <typename VertexS, typename HalfedgeDescriptor>
//  struct vertex_gen {
//      // This struct implements a vertex generator. The generel definition does
//      // nothing.
//  };
//..
// The proper vertex generator is defined only when 'VertexS' is of the form
// 'vertexS<...>':
//..
//
//  //SPECIALIZATIONS
//  template <typename ContainerS, bool HasVertexLink, typename VertexType, 
//            typename HalfedgeDescriptor>
//  struct vertex_gen<vertexS<ContainerS, HasVertexLink, VertexType>, 
//                                                       HalfedgeDescriptor> {
//      // This specialization is a vertex generator for 'vertexS' that takes a 
//      // vertex selector, vertex type selector , halfedge descriptor and uses 
//      // the container_selectors component to define a suitable base for the
//      // 'halfedge_ds_gen' that implements all the functionality of the
//      // 'halfedge_ds' related to vertices.
//      
//      // TYPES
//      typedef stored_vertex<HasVertexLink, HalfedgeDescriptor> vertex_type;
//          // The stored vertex type for this vertex generator.
//
//      typedef container_gen<ContainerS, vertex_type>  ContainerGen;
//          // The container generator for this vertex generator.
//
//      typedef typename ContainerGen::type             container_type;
//          // The vertex container type for this vertex generator.
//
//      typedef typename ContainerGen::descriptor       vertex_descriptor;
//          // The vertex descriptor type for this vertex generator.
//
//      typedef typename ContainerGen::iterator         vertex_iterator;
//          // The vertex iterator type for this vertex generator.
//
//      // DATA
//      container_type m_container;
//          // The actual container for the vertices from which vertices can be 
//          // obtained.
//  };
//..

#ifndef BOOST_HDSTL_VERTEX_SELECTORS_HPP
#define BOOST_HDSTL_VERTEX_SELECTORS_HPP 1

#include <boost/hdstl/halfedge_ds/container_selectors.hpp>

namespace boost {
namespace hdstl {

                   // =======================
                   // vertex selector classes
                   // =======================

struct sourceS {
    // A selector that tells the 'halfedge_ds' that each halfedge should keep 
    // a pointer to its source vertex.
};

struct targetS { 
    // A selector that tells the 'halfedge_ds' that each halfedge should keep 
    // a pointer to its target vertex.
};

struct noVertexS { 
   // A selector that tells the 'halfedge_ds' that it should not store vertices.
};

template <typename VertexType>
struct base_vertexS {
    // A selector that specializes whether source or target vertices are stored.
    typedef noVertexS t_type;
};

template<>
struct base_vertexS<sourceS>{
    // A specialized selector that tells the 'halfedge_ds' that source vertices
    // should be stored.

    // TYPES
    enum { type = true };
    enum { is_source = true };
    enum { is_target = !is_source };
    typedef sourceS t_type;
};

template<>
struct base_vertexS<targetS>{
    // A specialized selector that tells the 'halfedge_ds' that target vertices
    // should be stored.

    // TYPES
    enum { type = false };
    enum { is_target = true };
    enum { is_source = !is_target };
    typedef targetS t_target;
};

template <typename ContainerS, bool HasVertexLink=false, 
                                                  typename VertexType=sourceS>
struct vertexS 
: base_vertexS<VertexType> {
    // A selector that tells 'halfedge_ds' that it should store vertices
    // specified by the 'base_vertexS', which is either source or target,
    // and if 'HasVertexLink' is true, a link should be kept to one of the
    // halfedges in the 'source' or 'target' cycle.

    //TYPES
    typedef ContainerS  container_selector;
        // A selector (see the compenent 'container_selector') for the type of
        // container that will be used to store the vertices.

    enum { has_vertex_link = HasVertexLink };
};

                 // ===================
                 // class stored_vertex
                 // ===================

template <typename Base, 
          bool HasVertexLink = false,                                   
          typename HalfedgeDescriptor = int>
struct stored_vertex {
    // This struct implements a stored vertex from the parameterized 'Base'
    // (if not void), and adds a vertex link if 'HasVertexLink' is true

    //DATA
    HalfedgeDescriptor m_vertexLink;
    Base m_base;

    //CREATORS
    stored_vertex() {}

    stored_vertex(Base const& base, HalfedgeDescriptor vertexLink) 
    : m_vertexLink(vertexLink)
    , m_base(base){}

    //MANIPULATORS
    Base& base() { return m_base; }

    //ACCESSORS
    const Base& base() const { return m_base; }
};

template <typename Base, typename HalfedgeDescriptor>
struct stored_vertex<Base, false, HalfedgeDescriptor> {
    // This partial specialization implements a stored vertex, deriving from
    // the parameterized 'Base', but without storing a vertex link although its
    // constuctor accepts a null vertex link for uniformity of the interface
    // with the general definition.
    
    //DATA
    Base m_base;

    // CREATORS
    stored_vertex() {}

    stored_vertex(Base const& base, HalfedgeDescriptor = 0)
    : m_base(base) {}

    //MANIPULATORS
    Base& base() { return m_base; }

    //ACCESSORS
    const Base& base() const { return m_base; }
};

template <bool HasVertexLink,
          typename HalfedgeDescriptor>
struct stored_vertex<void, HasVertexLink, HalfedgeDescriptor> {
    // This 'struct' implements a stored vertex, with a 'void' base, adding a
    // vertex link if 'HasVertexLink' is true.

    // DATA
    HalfedgeDescriptor m_vertexLink;

    // CREATORS
    stored_vertex() {}

    stored_vertex(HalfedgeDescriptor vertexLink)
    : m_vertexLink(vertexLink) {}
};

template <typename HalfedgeDescriptor>
struct stored_vertex<void, false, HalfedgeDescriptor> {
    // This partial specialization implements a stored vertex, with a 'void' 
    // base, but without storing a vertex link although its
    // constructor accepts a null vertex link for uniformity of the
    // interface with the general definition.

    // CREATORS
    stored_vertex(HalfedgeDescriptor = 0) {}
};


                 // ================
                 // class vertex_gen
                 // ================

template <typename VertexS, typename HalfedgeDescriptor, typename VertexBase>
struct vertex_gen {
    // This struct implements a vertex generator. The generel definition does
    // nothing.
};

//SPECIALIZATIONS
template <typename ContainerS, bool HasVertexLink, typename VertexType, 
          typename HalfedgeDescriptor, typename VertexBase>
struct vertex_gen<vertexS<ContainerS, HasVertexLink, VertexType>, 
                                              HalfedgeDescriptor, VertexBase> {
    // This specialization is a vertex generator for 'vertexS' that takes a 
    // vertex selector, vertex type selector , halfedge descriptor and uses 
    // the container_selectors compenent to define a suitable base for the
    // 'halfedge_ds_gen' that implements all the functionality of the
    // 'halfedge_ds' related to vertices.
    
    // TYPES
    typedef vertexS<ContainerS,HasVertexLink>       vertex_selector;
        // The vertex selector.
    
    typedef stored_vertex<VertexBase,HasVertexLink, HalfedgeDescriptor> 
                                                           vertex_type;
        // The stored vertex type for this vertex generator.

    typedef container_gen<ContainerS, vertex_type>  ContainerGen;
        // The container generator for this vertex generator.

    typedef typename ContainerGen::type             container_type;
        // The vertex container type for this vertex generator.
    
    typedef typename ContainerGen::size_type        size_type;
        // The vertex container size type for this vertex generator.

    typedef typename ContainerGen::descriptor       vertex_descriptor;
        // The vertex descriptor type for this vertex generator.

    typedef typename ContainerGen::iterator         vertex_iterator;
        // The vertex iterator type for this vertex generator.

    // DATA
    container_type m_container;
        // The actual container for the vertices from which vertices can be 
        // obtained.
};
                   

} // namespace hdstl
} // namespace boost

#endif
