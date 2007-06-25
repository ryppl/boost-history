//vertex_hds_concept.hpp    -*- C++ -*- 
//
//@PURPOSE: Provide 'VertexHDS' concept definition, and concept-checking class.
//
//@DESCRIPTION: This file provides a single class template that,
// when used with the following concept-checking utilities:
//..
//  boost::function_requires<VertexHDSConcept<HDS> >();
//..
// inside a function body, or
//..
//  BOOST_CLASS_REQUIRE(HDS, boost::hdstl,VertexHDSConcept);
//..
// inside a class body, checks whether the type given as template
// argument is a model of the HDS concept described on the page [vertexhds].
//
//@CONCEPT: [vertexhds] Vertex HDS concept
//
///Definition
///----------
// This concept refines the 'HDSConcept', and requires an additional vertex
// descriptor to handle vertices in the 'HDS' data structure. 
//
///Refinement of:
///--------------
//  - 'HDSConcept'
//
///Notation
///--------
//  - 'HDS'  A type that is a model of 'VertexHDSConcept'
//  - 'hds'  A non-modifiable instance of 'HDS'
//  - 'h'    Halfedge descriptor, of type 'hds_traits<HDS>::halfedge_descriptor'
//  - 'v'    Vertex descriptor, of type 'hds_traits<HDS>::vertex_descriptor'
//
///Associated types
///----------------
// In addition to the types in the 'HDS' concept:
//  - 'hds_traits<HDS>::vertex_descriptor': must be 'DefaultConstructible', 
//    'CopyConstructible', 'EqualityComparable', and 'Assignable'.
//
///Definitions
///-----------
// In addition to the definitions in the 'HDS' concept:
//  - 'halfedge_descriptor' is a type that contains information to access 
//     the halfedge.  (See the 'HDSConcept' for a full definition.)
//  - 'vertex_descriptor' is a type that contains information to access the
//     vertex at the source or the target of the halfedge.
//
///Valid Expressions
///-----------------
// In addition to the valid expressions of the 'HDS' concept:
//  - 'vertex(hds,h)' must return a value assignable to 'v'.
//
///Expression Semantics
///--------------------
// In addition to the expressions semantics of the 'HDS' concept:
//  - 'hds_traits<HDS>::supports_vertices': must compare equal to 'true'.
//  - 'vertex(hds,h)' returns the vertex descriptor of the vertex to the
//    source or target of 'h', depending on the 'source_access_tag' and
//    'target_access_tag'.
//
///Complexity guarantees
///---------------------
// In addition to the complexity guarantees of the 'HDS' concept:
//  - 'vertex(hds,h)': amortized constant time.
//
///Invariants 
///----------
// 'VertexHDSConcept' should validate all the invariants for the 'HDSConcept'.
//
///Concept-checking class
///----------------------
//..
// template <class HDS> 
// struct VertexHDSConcept {
//    typedef typename hds_traits<HDS>::vertex_descriptor vertex_descriptor; 
//    void constraints() {
//       using namespace boost;
//       function_requires<HDSConcept>();
//       function_requires<DefaultConstructibleConcept<vertex_descriptor> >();
//       function_requires<CopyConstructibleConcept<vertex_descriptor> >();
//       function_requires<EqualityComparableConcept<vertex_descriptor> >();
//       function_requires<AssignableConcept<vertex_descriptor> >();
//       BOOST_STATIC_ASSERT(hds_traits<HDS>::supports_vertices);
//       v = vertex(hds,h);
//       const_constraints(hds);
//    }
//    void const_constraints(HDS const& hds) {
//       v = vertex(hds,h);
//    }
//    HDS hds;
//    vertex_descriptor v;
// };
//..

#ifndef BOOST_HDSTL_CONCEPTS_VERTEX_HDS_CONCEPT_HPP
#define BOOST_HDSTL_CONCEPTS_VERTEX_HDS_CONCEPT_HPP 1

#include <boost/concepts.h>

namespace hdstl{
namespace concepts {

    template <class HDS> 
    struct VertexHDSConcept {
        // This class provides a concept checking class for the 
        // 'VertexHDSConcept', i.e., using the statement
        //..
        //  boost::function_requires<VertexHDSConcept<HDS>()
        //..
        // inside a function body, or
        //..
        //  BOOST_CLASS_REQUIRE(HDS, boost::hdstl, VertexHDSConcept);
        //..
        // inside a class definition body, should trigger a compile-time error
        // if the type HDS does not model the 'VertexHDSConcept'.

        // TYPES
        typedef typename hds_traits<HDS>::vertex_descriptor vertex_descriptor; 
            // The specialization of 'hds_traits<HDS>' must have these required
            // types, obeying the types requirements stated in the detailed
            // description of the 'VertexHDS' concept on page [vertexhdsconcept].

        //MANIPULATORS
        void constraints() 
            // Check that the 'HDS' template parameters satisfies all the
            // constraints of 'VertexHDSConcept' on page [vertexhdsconcept].
        {
            using namespace boost;
            function_requires<HDSConcept>();
            function_requires<DefaultConstructibleConcept<vertex_descriptor> >();
            function_requires<CopyConstructibleConcept<vertex_descriptor> >();
            function_requires<EqualityComparableConcept<vertex_descriptor> >();
            function_requires<AssignableConcept<vertex_descriptor> >();
            BOOST_STATIC_ASSERT(hds_traits<HDS>::supports_vertices);

            v = vertex(hds,h);
            const_constraints(hds);
        }

        // ACCESSORS
        void const_constraints(HDS const& hds) 
            // Check that the non-modifiable 'HDS' template parameters
            // satisfies all the constraints of 'VertexHDSConcept'.
        {
            v = vertex(hds,h);
        }

      private:
        //DATA
        VertexHDS hds;         // a halfedge data structure object
        vertex_descriptor f;   // a vertex descriptor
   };

}  // close namespace concepts
}  // close namespace hdstl

#endif
