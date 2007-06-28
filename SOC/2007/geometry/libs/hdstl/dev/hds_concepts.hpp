//hds_concepts.hpp   -*- C++ -*-
//
//@PURPOSE: Provides concept checking classes for HDS-related concepts.
//
///HDS Concepts
///------------
// This library defines the following concepts:
//
// - [hdsconcept] is the basic concept, which all the other HDS concepts refine.
//   Requires a halfedge descriptor type and access to the opposite halfedge.
//
// - [forwardhdsconcept] refines the [hdsconcept], and requires access to the next
//   halfedge along a facet boundary, at source, or at target.  All three
//   halfedges are linked by algebraic relations involving opposite, so having
//   one is like having all three.
//
// - [backwardhdsconcept] refines the [hdsconcept], and requires access to the
//   previous halfedge along a facet boundary, at source, or at target.  All
//   three halfedges are linked by algebraic relations involving opposite, so
//   having one is like having all three.
//
// - [bidirectionalhdsconcept] is a combination of the [forwardhdsconcept] and
//   of the [backwardhdSconcept], with no extra requirements.
//
// - [facethdsconcept] requires, in addition to the [hdsconcept], a facet
//   descriptor and access to the facet incident to a given halfedge.
//
// - [vertexhdsconcept] requires, in addition to the [hdsconcept], a vertex
//   descriptor and access to the vertex incident to a given halfedge, either at
//   source or at target.
//
// - [halfedgelisthdsconcept] requires, in addition to [hdsconcept], a halfedge
//   iterator, and methods for accessing the range of all halfedges in a given
//   HDS.  Note that there are no pre-defined order for these halfedges.
//
// - [facetlisthdsconcept] requires, in addition to [facethdsconcept], a
//   halfedge iterator, and methods for accessing the range of all facets in a
//   given HDS.  Note that there are no pre-defined order for these facets.
//
// - [vertexlisthdsconcept] requires, in addition to [vertexhdsconcept], a
//   vertex iterator, and methods for accessing the range of all vertices in a
//   given HDS.  Note that there are no pre-defined order for these vertices.
//
// In addition to these concepts, this library also defines mutable concepts
// mirroring each one of the above concepts:
//
// - [mutablehdsconcept] refines [hdsconcept], and requires the ability to
//   create a new edge (pair of opposite halfedges), and to remove such a pair.
//
// - [mutableforwardhdsconcept] refines the [mutablehdsconcept] and the
//   [forwardhdsconcept], and requires the ability to set the next halfedge
//   along a facet boundary, at source, or at target.
//
// - [mutablebackwardhdsconcept] refines the [mutablehdsconcept] and the
//   [backwardhdsconcept], and requires the ability to set the 
//   previous halfedge along a facet boundary, at source, or at target.
//
// - [mutablebidirectionalhdsconcept] is a combination of the [mutableforwardhdsconcept] and
//   of the [mutablebackwardhdSconcept], with no extra requirements.
//
// - [mutablefacethdsconcept] requires, in addition to the [mutablehdsconcept]
//   and the [facethdsconcept], the ability to set the facet of a halfedge to a
//   given facet descriptor.
//
// - [mutablevertexhdsconcept] requires, in addition to the [mutablehdsconcept]
//   and the [vertexdsconcept], the ability to set the source or target of a
//   halfedge to a given vertex descriptor.
//
// The following concept hierarchy gives a description of the relation ships
// between these concepts.  The arrows '-->'  describe a *isA* relationship
// between these concepts, that is, there is an arrow from *B* to *A* when a
// concept *B* refines another concept *A*.
//..
//  ,---------------------.  ,------------------------.   ,----------------------. 
//  | FacetListHDSConcept |  | HalfedgeListHDSConcept |   | VertexListHDSConcept | 
//  `---------------------'  `------------------------'   `----------------------' 
//             |                        |                           |
//             v                        |                           v
//    ,-----------------.               |                 ,------------------. 
//    | FacetHDSConcept |               |                 | VertexHDSConcept | 
//    `-----------------'               |                 `------------------' 
//         |                            |                           |
//         |                            |                           |
//         |                ,-------------------------.             |
//         |                | BidirectionalHDSConcept |             |
//         |                `-------------------------'             |
//         |                     |      |         |                 |
//         |                     v      |         v                 |
//         |    ,-------------------.   |   ,-------------------.   |
//         |    | BackwardHDSConcept |  |   | ForwardHDSConcept |   |
//         |    `-------------------'   |   `-------------------'   |
//         |                     |      |    |                      |
//         \                     v      v    v                      |
//          \                   ,-------------.                    /
//           `----------------> |  HDSConcept | <-----------------' 
//                              `-------------'                     
//..
// In addition, there is an identical hierarchy among the mutable concepts,
// with horizontal arrows between mutable and non-mutable.  That is, there is
// an arrow from *MutableB* to *MutableA* iff there is an arrow from *B* to
// *A*, and there always is an arrow from *MutableA* to *A*.  As an example,
// the sub-lattice involving the 'HDS', 'ForwardHDS', 'MutableHDS', and
// 'MutableForwardHDS' concepts looks like this:
//..
//             .   ,-------------------.        ,--------------------------.
//             .   | ForwardHDSConcept |<-------| MutableForwardHDSConcept |
//             .   `-------------------'        `--------------------------'
//          .  .     |          .                  |
//          v  v     v        .                    v             .
//      ,-------------. < . .   ,-------------------.          .
// . . >|  HDSConcept | <-------| MutableHDSConcept |< . . . .
//      `-------------'         `-------------------'  
//..

#ifndef HDSTL_HDS_CONCEPTS_HPP
#define HDSTL_HDS_CONCEPTS_HPP

#include <hds_concepts/hds_concept.hpp>
#include <hds_concepts/forward_hds_concept.hpp>
#include <hds_concepts/backward_hds_concept.hpp>
#include <hds_concepts/bidirectional_hds_concept.hpp>
#include <hds_concepts/facet_hds_concept.hpp>
#include <hds_concepts/vertex_hds_concept.hpp>
#include <hds_concepts/halfedge_list_hds_concept.hpp>
#include <hds_concepts/vertex_list_hds_concept.hpp>
#include <hds_concepts/facet_list_hds_concept.hpp>
#include <hds_concepts/mutable_hds_concept.hpp>
#include <hds_concepts/mutable_forward_hds_concept.hpp>
#include <hds_concepts/mutable_backward_hds_concept.hpp>
#include <hds_concepts/mutable_bidirectional_hds_concept.hpp>
#include <hds_concepts/mutable_facet_hds_concept.hpp>
#include <hds_concepts/mutable_vertex_hds_concept.hpp>

#endif
