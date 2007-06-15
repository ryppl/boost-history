//vertex_list_hds_concept.hpp    -*- C++ -*- 
//
//@PURPOSE: Provide 'VertexListHDS' concept definition, and concept checking class.
//
//@DESCRIPTION: This file provides a single class template that,
// when used with the following concept-checking utilities:
//..
//  boost::function_requires<VertexListHDSConcept<HDS> >();
//..
// inside a function body, or
//..
//  BOOST_CLASS_REQUIRE(HDS, boost::hdstl,VertexListHDSConcept);
//..
// inside a class body, checks whether the type given as template
// argument is a model of the HDS concept described on the page 
// [vertexlisthds].
//
//@CONCEPT: [vertexlisthds] Vertex List HDS concept
//
///Definition
///----------
// This concept refines the 'HDSConcept', and specifies additional accessors
// to handle vertices in the 'HDS' data structure. 
//
///Refinement of:
///--------------
//  - 'HDSConcept'
//
///Notation
///--------
//  - 'HDS'  A type that is a model of VertexListHDSConcept
//  - 'hds'  A non-modifiable instance of HDS
//  - 'v'    Vertex descriptor, of type 'hds_traits<HDS>::vertex_descriptor'
//  - 'p'    Vertex iterator pair, of type 
//           'std::pair<vertex_iterator,vertex_iterator>'
//  - 'V'    Vertex size type, of type 'hds_traits<HDS>::vertex_size_type'
//
///Associated types
///----------------
//  - 'hds_traits<HDS>::vertex_descriptor': must be 'DefaultConstructible', 
//    'CopyConstructible', 'EqualityComparable', and 'Assignable'.
//  - 'hds_traits<HDS>::vertex_iterator': must be 'MultiPassInputIterator'.
//  - 'hds_traits<HDS>::vertex_size_type': vertex size type.
//
///Definitions
///-----------
//  - 'vertex_descriptor' is a type that contains information to access 
//     the vertex.  (See the 'HDSConcept' for a full definition.)
//  - 'vertex_iterator' is an iterator type for the vertices.
//  - 'vertex_size_type' defines the size type.
//
///Valid Expressions
///-----------------
// In addition to the valid expressions of the 'HDS' concept:
//  - 'vertices(hds)' must return a value assignable to 'p'.  
//  - '*p.first' must return a value assignable to 'v'.
//  - 'num_vertices(HDS)' must return a value assignable to 'V'.
//
///Expression Semantics
///--------------------
// In addition to the valid expression semantics of the 'HDS' concept:
//  - 'vertices(hds)' returns a 'vertex_iterator' pair 'p', consistent 
//    with the STL style iterator ranges, which means all the vertices
//    can be accessed by increasing the first part of the pair until the second
//    part is reached.
//  - '*p.first' returns a 'vertex_descriptor'.
//  - 'num_vertices(HDS)' returns the number of vertices in the 'HDS' data
//    structure.
//
///Complexity guarantees
///---------------------
//  - 'vertices(hds)'    : amortized constant time. (for just getting the 
//    'vertex_iterator' pair, accessing the vertices takes linear time.)
//  - '*p.first'         : amortized constant time.
//  - 'num_vertices(HDS)': amortized constant time.
//
///Invariants 
///----------
// 'VertexListHDSConcept' should validate all the invariants for the 
// 'HDSConcept'.
//
///Concept-checking class
///----------------------
//..
// template <class HDS> 
// struct VertexListHDSConcept {
//     typedef typename hds_traits<HDS>::vertex_iterator  vertex_iterator;
//     typedef typename hds_traits<HDS>::vertex_descriptor vertex_descriptor;
//     typedef typename hds_traits<HDS>::vertices_size_type vertices_size_type;
//     void constraints() 
//     {
//         using namespace boost;
//         function_requires<HDSConcept>();
//         function_requires<DefaultConstructibleConcept<vertex_descriptor> >();
//         function_requires<CopyConstructibleConcept<vertex_descriptor> >();
//         function_requires<EqualityComparableConcept<vertex_descriptor> >();
//         function_requires<AssignableConcept<vertex_descriptor> >();
//         function_requires<hdstl_detail::MultiPassInputIteratorConcept<vertex_iterator> >();
//         p = vertices(hds);
//         v = *p.first;
//         V = num_vertices(hds);
//         const_constraints(hds);
//     }
//     void const_constraints(HDS const& hds) 
//     {
//         p = vertices(hds);
//         v = *p.first;
//         V = num_vertices(hds);
//     }
//     private:
//     VertexListHDS hds; 
//     vertex_descriptor v;
//     std::pair<vertex_iterator,vertex_iterator> p;
//     vertex_size_type V;  
// };
//..

#ifndef BOOST_HDSTL_CONCEPTS_VERTEX_LIST_HDS_CONCEPT_HPP
#define BOOST_HDSTL_CONCEPTS_VERTEX_LIST_HDS_CONCEPT_HPP 1

#include <boost/concepts.h>

namespace hdstl{
namespace concepts {

   template <class HDS> 
   struct VertexListHDSConcept {
       // This class provides a concept checking class for the 
       // 'VertexListHDSConcept', i.e., using the statement
       //..
       //  boost::function_requires<VertexListHDSConcept<HDS>()
       //..
       // inside a function body, or
       //..
       //  BOOST_CLASS_REQUIRE(HDS, boost::hdstl, VertexListHDSConcept);
       //..
       // inside a class definition body, should trigger a compile-time error
       // if the type HDS does not model the 'VertexListHDSConcept'.

       // TYPES
       typedef typename hds_traits<HDS>::vertex_iterator  vertex_iterator;
       typedef typename hds_traits<HDS>::vertex_descriptor vertex_descriptor;
       typedef typename hds_traits<HDS>::vertices_size_type vertices_size_type;
       // The specialization of 'hds_traits<HDS>' must have these required
       // types, obeying the types requirements stated in the detailed
       // description of the 'VertexListHDS' concept on page 
       // [vertexlisthdsconcept].

       //MANIPULATORS
       void constraints() 
           // Check that the 'HDS' template parameters satisfies all the
           // constraints of 'VertexListHDSConcept' on page 
           // [vertexlisthdsconcept].
       {
           using namespace boost;
           function_requires<HDSConcept>();
           function_requires<DefaultConstructibleConcept<vertex_descriptor> >();
           function_requires<CopyConstructibleConcept<vertex_descriptor> >();
           function_requires<EqualityComparableConcept<vertex_descriptor> >();
           function_requires<AssignableConcept<vertex_descriptor> >();
           function_requires<hdstl_detail::MultiPassInputIteratorConcept<vertex_iterator> >();
           p = vertices(hds);
           v = *p.first;
           V = num_vertices(hds);
           const_constraints(hds);
       }

       // ACCESSORS
       void const_constraints(HDS const& hds) 
           // Check that the non-modifiable 'HDS' template parameters
           // satisfies all the constraints of 'VertexListHDSConcept'.
       {
           p = vertices(hds);
           v = *p.first;
           V = num_vertices(hds);
       }

       private:
       //DATA
       VertexListHDS hds;     // a halfedge data structure object
       vertex_descriptor v;   // a vertex descriptor
       std::pair<vertex_iterator,vertex_iterator> p; // a vertex iterator pair
       vertex_size_type V;    // vertex size type

   };

}  // close namespace concepts
}  // close namespace hdstl

#endif
