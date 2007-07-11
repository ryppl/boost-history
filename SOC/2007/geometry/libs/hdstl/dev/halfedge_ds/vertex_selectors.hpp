//vertex_selectors.hpp   -*- C++ -*-
//
//@PURPOSE: Provide selector classes for 'VertexS'.
//
//@CLASSES:
//  'vertexS':  defines configuration options for vertex access
//  'noVertexS' defines that vertices are not supported by the 'HDS'
//  'vertexLinkS' reverse links from vertices to a halfedge is defined
//  'noVertexLinkS' reverse links from vertices to halfedges not defined
//  'sourceS'  default vertex access method for halfedges is set to source
//  'targetS'  default vertex access method for halfedges is set to target
//  'base_vertex' base class to select the method of access between source and
//   target
//  
//@SEE_ALSO: {hds_concepts.hpp, halfedge_ds.hpp}
//
//@DESCRIPTION: This file provides the vertexS selector class for
//configurations related to vertex properties. Uses the 'noVertexS',
//'vertexLinkS', 'noVertexLinkS', 'sourceS', 'targetS', and 'base_vertex'
//selector classes to configure options based on the vertex access type.

#ifndef BOOST_HDSTL_VERTEX_SELECTORS_HPP
#define BOOST_HDSTL_VERTEX_SELECTORS_HPP 1

namespace boost;
namespace hdstl;

struct noVertexS { };

struct vertexLinkS { };

struct noVertexLinkS { };

struct sourceS { };

struct targetS { };

template <typename VertexType>
struct base_vertexS {};

template<>
struct base_vertexS<sourceS>{
    typedef true_type type;
    typedef true_type is_source;
    typedef false_type is_target;
}

template<>
struct base_vertexS<targetS>{
    typedef false_type type;
    typedef false_type is_source;
    typedef true_type is_target;
}

template <typename Selector, VertexLinkType, VertexType>
struct vertexS 
: base_vertexS<VertexType> 
{ 
};

template<typename Selector, VertexType>
struct vertexS<Selector, vertexLinkS, VertexType> 
: base_vertexS<VertexType> {
    typedef true_type has_vertex_links;
};

template<typename Selector, VertexType>
struct vertexS<Selector, noVertexLinkS, VertexType> 
: base_vertexS<VertexType> {
    typedef false_type has_vertex_links;
};

} // namespace hdstl
} // namespace boost

#endif
