//hds_concept.hpp   -*- C++ -*-
//
//@PURPOSE: Provide 'HDS' concept definition and concept-checking class.
//
//@DESCRIPTION: This file provides a single class template that,
// when used with the following concept-checking utilities:
//..
//  boost::function_requires<HDSConcept<HDS> >();
//..
// inside a function body, or
//..
//  BOOST_CLASS_REQUIRE(HDS, boost::hdstl, HDSConcept);
//..
// inside a class body, checks whether the type given as template
// argument is a model of the HDS concept described on the page
// [hdsconcept].
//
//@CONCEPT: [hdsconcept] HDS concept definition
//
///Definition
///----------
// This concept is the basic 'HDSConcept', which all the other 'HDS'
// concepts refine.  It contains a few requirements which are common to
// all the graph concepts.  One should note that a model of 'HDSConcept' is not
// required to be a model of 'Assignable', so algorithms should pass 'HDS'
// objects by reference.
//
///Refinement of:
///--------------
// This concept is not a refinement of another concept, in   particular a
// model 'HDS' of 'HDSConcept' does *not* need to be 'DefaultConstructible',
// 'CopyConstructible', or 'Assignable'.
//
///Notation
///--------
//  - 'HDS'    A type that is a model of 'HDSConcept'
//  - 'hds'    A non-modifiable instance of 'HDS'
//  - 'h','g'  Halfedge descriptors, of type 'hds_traits<HDS>::halfedge_descriptor'
//
///Associated types
///----------------
//  - 'hds_traits<HDS>::halfedge_descriptor': must be 'DefaultConstructible',
//    'CopyConstructible', 'EqualityComparable', and 'Assignable'.
//
///Definitions
///-----------
// 'halfedge_descriptor' is a type that contains information to access
// the halfedge.  Note that the halfedge descriptor type (accessible as a
// member of the 'hds_traits<HDS>' type) is an opaque type.  All that can be
// done to it is passing it to the function opposite and letting the hds use
// the information it contains to find the opposite halfedge.  In
// particular, there is no guarantee that the descriptor be a pointer type.
//
///Valid Expressions
///-----------------
// The following expression must be valid and obey the syntactic requirement:
//   - 'opposite(h, hds)' must return a value assignable to 'h'.
//   - 'hds_traits<HDS>::supports_facets' must be usable as a Boolean.
//   - 'hds_traits<HDS>::supports_vertices' must be usable as a Boolean.
//
///Expression Semantics
///--------------------
// The expressions semantics are as follows:
//   - 'hds_traits<HDS>::supports_facets' is true if and only if 'HDS'
//     is a model of the 'FacetHDS' concept (see the page [facethdsconcept]).
//   - 'hds_traits<HDS>::supports_vertices' is true if and only if 'HDS'
//     is a model of the 'VertexHDS' concept (see the page [vertexhdsconcept]).
//   - 'opposite(h, hds)' returns a descriptor to the halfedge opposite 'h' in
//     the data structure 'hds'. Does not modify the state of either 'hds'
//     or 'h'.
//
///Complexity guarantees
///---------------------
//   - 'opposite(h, hds)': Amortized constant time.
//
///Invariants
///----------
// Every valid halfedge descriptor 'h' must have a valid opposite
// 'g = opposite(h, hds)', *distinct* from h, such that
// 'opposite(g, hds)' returns 'h'.  (Hence, 'opposite' is an involution.)
//
///Concept-checking class
///----------------------
//..
//  template <class HDS> 
//  struct HDSConcept {
//    typedef typename hds_traits<HDS>::halfedge_descriptor halfedge_descriptor; 
//    void constraints() {
//      using namespace boost;
//      function_requires<DefaultConstructibleConcept<halfedge_descriptor> >();
//      function_requires<CopyConstructibleConcept<halfedge_descriptor> >();
//      function_requires<EqualityComparableConcept<halfedge_descriptor> >();
//      function_requires<AssignableConcept<halfedge_descriptor> >();
//
//      h = opposite(h, hds);
//      const_constraints(hds);
//    }
//    void const_constraints(HDS const& hds) {
//      if (hds_traits<HDS>::supports_vertices) {}
//      if (hds_traits<HDS>::supports_facets) {}
//      h = opposite(h, hds);
//    }
//    HDS hds;
//    halfedge_descriptor h;
//  }
//..

#ifndef BOOST_HDSTL_CONCEPTS_HDS_CONCEPT_HPP
#define BOOST_HDSTL_CONCEPTS_HDS_CONCEPT_HPP 1

#include <boost/concept_check.hpp>
#include <boost/hdstl/hds_traits.hpp>

namespace boost {
namespace hdstl {

    template <class HDS> 
    struct HDSConcept {
        // This class provides a concept checking class for the
        // 'HDS' concept defined in [hdsconcept], i.e., using the statement
        //..
        //  boost::function_requires<HDSConcept<HDS> >();
        //..
        // inside a function body, or
        //..
        //  BOOST_CLASS_REQUIRE(HDS, boost::hdstl, HDSConcept);
        //..
        // inside a class definition body, should trigger a compile-time
        // error if the type 'HDS' does not model the 'HDS' concept.

        // TYPES
        typedef typename hds_traits<HDS>::halfedge_descriptor halfedge_descriptor; 
            // The specialization of 'hds_traits<HDS>' must have these required
            // types, obeying the types requirements stated in the detailed
            // description of the 'HDS' concept on page [hdsconcept].

        // MANIPULATORS
        void constraints()
            // Check that the 'HDS' template parameters satisfies all the
            // constraints of 'HDSConcept' on page [hdsconcept].
        {
            using namespace boost;
            function_requires<DefaultConstructibleConcept<halfedge_descriptor> >();
            function_requires<CopyConstructibleConcept<halfedge_descriptor> >();
            function_requires<EqualityComparableConcept<halfedge_descriptor> >();
            function_requires<AssignableConcept<halfedge_descriptor> >();

            h = opposite(h, hds);
            const_constraints(hds);
        }

        // ACCESSORS
        void const_constraints(const HDS& hds)
            // Check that the non-modifiable 'HDS' template parameters
            // satisfies all the constraints of 'HDSConcept'.
        {
            if (hds_traits<HDS>::supports_vertices) {}
            if (hds_traits<HDS>::supports_facets) {}
            h = opposite(h, hds);
        }

      private:
	// DATA
        HDS hds;                 // a halfedge data structure object
        halfedge_descriptor h;   // a halfedge descriptor
    };

}  // close namespace hdstl
}  // close namespace boost

#endif
