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
// to iterate over vertices in the 'HDS' data structure. 
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
//  - 'p'    Vertex iterator pair, of type 'hds_traits<HDS>::vertex_iterator' 
//  - 'n'    Vertex size type, of type 'hds_traits<HDS>::size_type'
//
///Associated types
///----------------
//  - 'hds_traits<HDS>::vertex_descriptor': must be 'DefaultConstructible', 
//    'CopyConstructible', 'EqualityComparable', and 'Assignable'.
//  - 'hds_traits<HDS>::vertex_iterator': must be 'MultiPassInputIterator'.
//  - 'hds_traits<HDS>::size_type': vertex size type.
//
///Definitions
///-----------
//  - 'vertex_descriptor' is a type that contains information to access 
//     the vertex.  (See the 'HDSConcept' for a full definition.)
//  - 'vertex_iterator' is an iterator type for the vertices.
//  - 'size_type' defines the size type.
//
///Valid Expressions
///-----------------
// In addition to the valid expressions of the 'HDS' concept:
//  - 'vertices_begin(hds)' must return a value assignable to 'p'.  
//  - 'vertices_end(hds)' must return a value assignable to 'p'.  
//  - 'num_vertices(hds)' must return a value assignable to 'n'.
//
///Expression Semantics
///--------------------
// In addition to the valid expression semantics of the 'HDS' concept:
//  - 'vertices_begin(hds)' returns a 'vertex_iterator' 'p' pointing to the 
//    beginning of the 'vertex list'.
//  - 'vertices_end(hds)' returns a 'vertex_iterator' 'p' pointing to the 
//    end of the 'vertex list'.
//  - 'num_vertices(hds)' returns the number of vertices in the 'HDS' data
//    structure.
//
///Complexity guarantees
///---------------------
//  - 'vertices_begin(hds)': amortized constant time.  
//  - 'vertices_end(hds)'  : amortized constant time.  
//  - 'num_vertices(hds)'  : amortized constant time.
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
//         b = vertices_begin(hds);
//         e = vertices_end(hds);
//         n = num_vertices(hds);
//         const_constraints(hds);
//     }
//     void const_constraints(HDS const& hds) 
//     {
//         b = vertices_begin(hds);
//         e = vertices_end(hds);
//         n = num_vertices(hds);
//     }
//     private:
//     VertexListHDS hds; 
//     vertex_descriptor v;
//     vertex_iterator b;
//     vertex_iterator e;
//     size_type n;  
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
           b = vertices_begin(hds);
           e = vertices_end(hds);
           n = num_vertices(hds);
           const_constraints(hds);
       }

       // ACCESSORS
       void const_constraints(HDS const& hds) 
           // Check that the non-modifiable 'HDS' template parameters
           // satisfies all the constraints of 'VertexListHDSConcept'.
       {
           b = vertices_begin(hds);
           e = vertices_end(hds);
           n = num_vertices(hds);
       }

       private:
       //DATA
       VertexListHDS hds;     // a halfedge data structure object
       vertex_descriptor v;   // a vertex descriptor
       vertex_iterator b;     // a vertex iterator
       vertex_iterator e;     // a vertex iterator
       size_type n;    // vertex size type

   };

}  // close namespace concepts
}  // close namespace hdstl

#endif
