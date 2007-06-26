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
//  - 'VertexHDSConcept'
//
///Notation
///--------
//  - 'HDS'  A type that is a model of VertexListHDSConcept
//  - 'hds'  A non-modifiable instance of HDS
//  - 'v'    Vertex descriptor, of type 'hds_traits<HDS>::vertex_descriptor'
//  - 'vit'  Vertex iterator, of type 'hds_traits<HDS>::vertex_iterator' 
//  - 'n'    Vertex list size, of type 'hds_traits<HDS>::size_type'
//
///Associated types
///----------------
// In addition to the types of the 'VertexHDS' concept:
//  - 'hds_traits<HDS>::vertex_iterator': must be a model of the 
//    'ForwardIterator' concept. The value type of the vertex iterator must
//    be the same as the 'vertex_descriptor'.
//  - 'hds_traits<HDS>::size_type': integral size type.
//
///Definitions
///-----------
// In addition to the definitions of the 'VertexHDS' concept:
//  - 'vertex_iterator' is an iterator type for the vertices.
//  - 'size_type' defines the size type.
//
///Valid Expressions
///-----------------
// In addition to the valid expressions of the 'VertexHDS' concept:
//  - 'vertices_begin(hds)' must return a value assignable to 'vit'.  
//  - 'vertices_end(hds)' must return a value assignable to 'vit'.  
//  - '*vit' must return a value assignable to 'v'.  
//  - 'num_vertices(hds)' must return a value assignable to 'n'.
//
///Expression Semantics
///--------------------
// In addition to the valid expression semantics of the 'VertexHDS' concept:
//  - 'vertices_begin(hds)' returns a 'vertex_iterator' 'p' pointing to the 
//    beginning of the "vertex list".
//  - 'vertices_end(hds)' returns a 'vertex_iterator' 'p' pointing to the 
//    end of the "vertex list".
//  - '*vit' returns a descriptor to the vertex 'v' pointed to by 'vit'.
//  - 'num_vertices(hds)' returns the number of vertices in the 'HDS' data
//    structure.
//
///Complexity guarantees
///---------------------
// In addition to the complexity guarantees of the 'VertexHDS' concept:
//  - 'vertices_begin(hds)': amortized constant time.  
//  - 'vertices_end(hds)': amortized constant time.  
//  - '*vit': worst-case constant time.
//  - 'num_vertices(hds)': amortized constant time.
//
///Invariants 
///----------
//  - 'std::distance(vertices_begin(hds), vertices_end(hds)) 
//                                     ==  num_vertices(hds)'
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
//         function_requires<VertexHDSConcept<HDS> >();
//         function_requires<ForwardIteratorConcept<vertex_iterator> >();
//         function_requires<ConvertibleConcept<size_type,int> >();

//         b = vertices_begin(hds);
//         e = vertices_end(hds);
//         v = *b;
//         n = num_vertices(hds);
//         const_constraints(hds);
//     }
//     void const_constraints(HDS const& hds) 
//     {
//         b = vertices_begin(hds);
//         e = vertices_end(hds);
//         v = *b;
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
       typedef typename hds_traits<HDS>::size_type size_type;
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
           function_requires<VertexHDSConcept<HDS> >();
           function_requires<ForwardIteratorConcept<vertex_iterator> >();
           function_requires<ConvertibleConcept<size_type,int> >();

           b = vertices_begin(hds);
           e = vertices_end(hds);
           v = *b;
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
           v = *b;
           n = num_vertices(hds);
       }

     private:
       //DATA
       HDS hds;               // a halfedge data structure object
       vertex_descriptor v;   // a vertex descriptor
       vertex_iterator b,e;   // a vertex iterators
       size_type n;           // vertex size type

   };

}  // close namespace concepts
}  // close namespace hdstl

#endif
