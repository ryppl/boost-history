//mutable_vertex_hds_concept.hpp    -*- C++ -*- 
//
//@PURPOSE: Provide 'MutableVertexHDS' concept definition, and concept-checking class.
//
//@DESCRIPTION: This file provides a single class template that,
// when used with the following concept-checking utilities:
//..
//  boost::function_requires<MutableVertexHDSConcept<HDS> >();
//..
// inside a function body, or
//..
//  BOOST_CLASS_REQUIRE(HDS, boost::hdstl,MutableVertexHDSConcept);
//..
// inside a class body, checks whether the type given as template
// argument is a model of the HDS concept described on the page 
// [mutablevertexhds].
//
//@CONCEPT: [mutablevertexhds] Mutable Vertex HDS concept
//
///Definition
///----------
// This concept refines the 'VertexHDSConcept' and the 'MutableHDS' concept,
// and specifies additional vertex descriptor to handle vertices in the 
// 'HDS' data structure. 
//
///Refinement of:
///--------------
//  - 'VertexHDSConcept'
//  - 'MutableHDSConcept'
//
///Notation
///--------
//  - 'HDS'  A type that is a model of 'MutableVertexHDSConcept'
//  - 'hds'  A modifiable instance of 'HDS'
//  - 'v'    Vertex descriptor, of type 'hds_traits<HDS>::vertex_descriptor'
//
///Associated types
///----------------
// None besides the types in the 'VertexHDS' and the 'MutableHDS' concepts.
//
///Valid Expressions
///-----------------
// In addition to the valid expressions of the 'VertexHDS' and the
// 'MutableHDS' concepts:
//  - 'set_vertex(h, v, hds)' sets the source vertex descriptor value of 'h' to 
//    'v'.
//  - 'new_vertex(hds)' adds a new vertex 'v' to the 'hds' data structure
//    and returns the vertex descriptor to this new vertex.
//  - 'remove_vertex(hds,v)' removes the vertex 'v' from the 'hds' data 
//    structure.
//
///Expression Semantics
///--------------------
// In addition to the expression semantics of the 'VertexHDS' and the
// 'MutableHDS' concepts:
//  - 'set_vertex(h, v, hds)'  sets the source vertex descriptor value of 'h' to 
//    'v' for a single halfedge in the 'hds' data structure.
//  - 'new_vertex(hds)' adds a new vertex 'v' to the 'hds' data structure.
//    By this operation the vertex is added but no connections to the halfedges
//    are set. In order to assign vertces to halfedges 'set_vertex(h, v, hds)' 
//    operation should be used.
//  - 'remove_vertex(hds,v)' removes the vertex 'v' from the 'hds' data 
//    structure. All 'vertex_descriptor's related to this vertex will be 
//    invalidated after this call.
//
///Complexity guarantees
///---------------------
// In addition to the complexity guarantees of the 'VertexHDS' and the
// 'MutableHDS' concepts:
//  - 'set_vertex(h, v, hds)': amortized constant time.
//  - 'new_vertex(hds)': amortized constant time.
//  - 'remove_vertex(hds,v)': amortized constant time.
//
///Invariants 
///----------
// 'MutableVertexHDSConcept' should verify all the invariants for the 
// 'VertexHDSConcept' and the 'MutableHDSConcept'.
//
///Concept-checking class
///----------------------
//..
//  template <class HDS> 
//  struct MutableVertexHDSConcept {
//     typedef typename hds_traits<HDS>::vertex_descriptor vertex_descriptor; 
//     void constraints() {
//        using namespace boost;
//        function_requires<HDSConcept>();
//        function_requires<VertexHDSConcept>();
//        function_requires<DefaultConstructibleConcept<vertex_descriptor> >();
//        function_requires<CopyConstructibleConcept<vertex_descriptor> >();
//        function_requires<EqualityComparableConcept<vertex_descriptor> >();
//        function_requires<AssignableConcept<vertex_descriptor> >();
//
//        set_vertex(h, v, hds);
//        v = new_vertex(hds);
//        remove_vertex(h, v, hds);
//        const_constraints(hds);
//     }
//     void const_constraints(HDS const& hds) {
//     }
//     HDS hds;
//     halfedge_descriptor h;
//     vertex_descriptor v;
//  };
//..

#ifndef BOOST_HDSTL_CONCEPTS_MUTABLE_VERTEX_HDS_CONCEPT_HPP
#define BOOST_HDSTL_CONCEPTS_MUTABLE_VERTEX_HDS_CONCEPT_HPP 1

#include <boost/concepts.h>

namespace hdstl{
namespace concepts {

    template <class HDS> 
    struct MutableVertexHDSConcept {
        // This class provides a concept checking class for the 
        // 'MutableVertexHDSConcept', i.e., using the statement
        //..
        //  boost::function_requires<MutableVertexHDSConcept<HDS>()
        //..
        // inside a function body, or
        //..
        //  BOOST_CLASS_REQUIRE(HDS, boost::hdstl, MutableVertexHDSConcept);
        //..
        // inside a class definition body, should trigger a compile-time error
        // if the type HDS does not model the 'MutableVertexHDSConcept'.

        // TYPES
        typedef typename hds_traits<HDS>::vertex_descriptor vertex_descriptor; 
            // The specialization of 'hds_traits<HDS>' must have these required
            // types, obeying the types requirements stated in the detailed
            // description of the 'MutableVertexHDS' concept on page 
            // [mutablevertexhdsconcept].

        //MANIPULATORS
        void constraints() 
            // Check that the 'HDS' template parameters satisfies all the
            // constraints of 'MutableVertexHDSConcept' on page 
            // [mutablevertexhdsconcept].
        {
            using namespace boost;
            function_requires<HDSConcept>();
            function_requires<VertexHDSConcept>();
            function_requires<DefaultConstructibleConcept<vertex_descriptor> >();
            function_requires<CopyConstructibleConcept<vertex_descriptor> >();
            function_requires<EqualityComparableConcept<vertex_descriptor> >();
            function_requires<AssignableConcept<vertex_descriptor> >();

            set_vertex(h, v, hds);
            v = new_vertex(hds);
            remove_vertex(hds,v);

            const_constraints(hds);
        }

        // ACCESSORS
        void const_constraints(HDS const& hds) 
            // Check that the non-modifiable 'HDS' template parameters
            // satisfies all the constraints of 'MutableVertexHDSConcept'.
        {
        }

      private:
        //DATA
        MutableVertexHDS hds;  // a halfedge data structure object
        halfedge_descriptor h; // a vertex descriptor
        vertex_descriptor v;   // a vertex descriptor
   };

}  // close namespace concepts
}  // close namespace hdstl

#endif
