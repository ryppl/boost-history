//mutable_backward_hds_concept.hpp  -*- C++ -*-
//
//@PURPOSE: Provide 'MutableBackwardHDS' concept definition and concept-checking class.
//
//@DESCRIPTION: This file provides a single class template that,
// when used with the following concept-checking utilities:
//..
//  boost::function_requires<MutableBackwardHDSConcept<HDS> >();
//..
// inside a function body, or
//..
//  BOOST_CLASS_REQUIRE(HDS, boost::hdstl, MutableBackwardHDSConcept);
//..
// inside a class body, checks whether the type given as template
// argument is a model of the HDS concept described on the page
// [mutablebackwardhds].
//
//@CONCEPT: [mutablebackwardhds] Mutable Backward HDS concept
//
///Definition
///----------
// This concept refines the 'BackwardHDSConcept' and the 'MutableHDSConcept' 
// with additional mutable operations for the halfedge data structure.
//
///Refinement of
///-------------
//  - 'MutableHDSConcept'
//  - 'BackwardHDSConcept'
//
///Notation
///--------
//  - 'HDS'    A type that is a model of 'MutableBackwardHDSConcept'
//  - 'hds'    A modifiable instance of 'HDS'
//  - 'h','g'  Halfedge descriptors, of type 
//    'hds_traits<HDS>::halfedge_descriptor'
//
///Associated types
///----------------
// None besides the types in the 'MutableHDS' and 'BackwardHDS' concepts.
//
///Valid Expressions
///-----------------
// In addition to the valid expressions of the 'MutableHDS' and the 
// 'BackwardHDS' concepts:
//   - 'set_prev_in_facet(h, g, hds)' sets 'g' as the 'prev_in_facet' value of 
//     'h'.
//   - 'set_prev_at_source(h, g, hds)' sets 'g' as the 'prev_at_source' value of 
//     'h'.
//   - 'set_prev_at_target(h, g, hds)' sets 'g' as the 'prev_at_target' value of
//     'h'.
//
///Expression Semantics
///--------------------
// In addition to the expression semantics of the 'Mutable HDS' and the
// 'BackwardHDS' concepts:
//   - 'set_prev_in_facet(h, g, hds)' sets 'g' as the halfedge 
//      preceding 'h' in the adjacent facet cycle, when facet cycles are
//      oriented in counter-clockwise order.
//   - 'set_prev_at_source(h, g, hds)' sets 'g' as the halfedge 
//      preceding 'h' around the source vertex of 'h', when halfedges are
//      ordered around a given vertex in clockwise order.
//   - 'set_prev_at_target(h, g, hds)' sets 'g' as the halfedge  
//      preceding 'h' around the target vertex of 'h', when halfedges are
//      ordered around a given vertex in clockwise order.
//
///Complexity guarantees
///---------------------
// In addition to the complexity guarantees of the 'Mutable HDS' and the
// 'BackwardHDS' concepts:
//  - 'set_prev_in_facet(h, g, hds)': amortized constant time.
//  - 'set_prev_at_source(h, g, hds)': amortized constant time.
//  - 'set_prev_at_target(h, g, hds)': amortized constant time.
//
///Invariants 
///----------
// All the 'BackwardHDS' invariants should hold after the mutable 
// operations. (See the 'BackwardHDS' concept invariants for a full definition.)
//
///Concept-checking class
///----------------------
//..
//  template <class HDS> 
//  struct MutableBackwardHDSConcept {
//   typedef typename hds_traits<HDS>::halfedge_descriptor halfedge_descriptor; 
//      typedef typename hds_traits<HDS>::traversal_category traversal_category;
//      typedef typename hds_traits<HDS>::backward_category backward_category;
//      void constraints() 
//      {
//          using namespace boost;
//          function_requires<BackwardHDSConcept<HDS> >();
//          function_requires<MutableHDSConcept<HDS> >();
//
//          set_prev_in_facet(h, g, hds);
//          set_prev_at_source(h, g, hds);
//          set_prev_at_target(h, g, hds);
//          const_constraints(hds);
//      }
//      void const_constraints(HDS const& hds)
//      {
//      }
//    private:
//      MutableBackwardHDS hds;
//      halfedge_descriptor h,g;
//  };
//..

#ifndef BOOST_HDSTL_CONCEPTS_MUTABLE_BACKWARD_HDS_CONCEPT_HPP
#define BOOST_HDSTL_CONCEPTS_MUTABLE_BACKWARD_HDS_CONCEPT_HPP 1

#include <boost/concepts.h>

namespace hdstl{
namespace concepts{

    template <class HDS> 
    struct MutableBackwardHDSConcept {
        // This class provides a concept checking class for the 
        // MutableBackwardHDSConcept, i.e., using the statement
        //..
        //  boost::function_requires<MutableBackwardHDSConcept<HDS>()
        //..
        // inside a function body, or
        //..
        //  BOOST_CLASS_REQUIRE(HDS, boost::hdstl, MutableBackwardHDSConcept);
        //..
        // inside a class definition body, should trigger a compile-time error
        // if the type HDS does not model the 'MutableBackwardHDSConcept'.

        // TYPES
        typedef typename hds_traits<HDS>::halfedge_descriptor halfedge_descriptor; 
        typedef typename hds_traits<HDS>::traversal_category traversal_category;
        typedef typename hds_traits<HDS>::backward_category backward_category;
            // The specialization of 'hds_traits<HDS>' must have these
            // required types, obeying the types requirements stated in the
            // detailed description of the 'MutableBackwardHDS' concept on page
            // [mutablebackwardhdsconcept].

        // MANIPULATORS
        void constraints() 
            // Check that the 'HDS' template parameters satisfies all the
            // constraints of 'MutableBackwardHDSConcept' on page 
            // [mutablebackwardhdsconcept].
        {
            using namespace boost;

            function_requires<BackwardHDSConcept<HDS> >();
            function_requires<MutableHDSConcept<HDS> >();

            set_prev_in_facet(h, g, hds);
            set_prev_at_source(h, g, hds);
            set_prev_at_target(h, g, hds);
            const_constraints(hds);
        }

        // ACCESSORS
        void const_constraints(HDS const& hds)
            // Check that the non-modifiable 'HDS' template parameters
            // satisfies all the constraints of 'MutableBackwardHDSConcept'.
        {
        }

      private:
        // DATA
        MutableBackwardHDS hds; // a halfedge data structure object
        halfedge_descriptor h,g;  // halfedge descriptors
    };

}  // close namespace concepts
}  // close namespace hdstl

#endif
