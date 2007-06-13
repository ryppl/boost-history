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
//  - 'hds'    A non-modifiable instance of 'HDS'
//  - 'h','g'  Halfedge descriptors, of type 
//    'hds_traits<HDS>::halfedge_descriptor'
//
///Associated types
///----------------
//  - 'hds_traits<HDS>::halfedge_descriptor': from 'HDSConcept'.
//  - 'hds_traits<HDS>::traversal_category':  from 'BackwardHDSConcept'.
//  - 'hds_traits<HDS>::backward_category':   from 'BackwardHDSConcept'.
//
///Definitions
///-----------
//  - 'halfedge_descriptor' is a type that contains information to access 
//     the halfedge.  (See the 'HDS' concept for a full definition.)
//  - 'traversal_category' is a type that contains information about the 
//     direction of the traversal, which is forward, backward or bidirectional.
//     (See the 'HDS' concept for a full definition.)
//  - 'backward_category' defines the primary accessor of the backward
//     information for use by algorithms which desire the most direct access. 
//     (See the 'HDS' concept for a full definition.)
//
///Valid Expressions
///-----------------
// In addition to the valid expressions of the 'HDS' concept, and the 
// 'BackwardHDS' concept:
//   - 'set_prev_in_facet(hds,h,g)' sets 'g' as the prev_in_facet value of 'h'.
//   - 'set_prev_at_source(hds,h,g)' sets 'g' as the prev_at_source value of 
//     'h'.
//   - 'set_prev_at_target(hds,h,g)' sets 'g' as the prev_at_target value of
//     'h'.
//
///Expression Semantics
///--------------------
// In addition to the expression semantics of the 'HDS' concept, and the
// 'BackwardHDS' concept:
//   - 'set_prev_in_facet(hds,h,g)' set 'g' as the halfedge 
//      preceding 'h' in the adjacent facet cycle, when facet cycles are
//      oriented in counter-clockwise order.
//   - 'set_prev_at_source(hds,h,g)' sets 'g' as the halfedge 
//      preceding 'h' around the source vertex of 'h', when halfedges are
//      ordered around a given vertex in clockwise order.
//   - 'set_prev_at_target(hds,h,g)' sets 'g' as the halfedge  
//      preceding 'h' around the target vertex of 'h', when halfedges are
//      ordered around a given vertex in clockwise order.
//
///Complexity guarantees
///---------------------
//  - 'set_prev_in_facet(hds,h,g)': amortized constant time.
//  - 'set_prev_at_source(hds,h,g)': amortized constant time.
//  - 'set_prev_at_target(hds,h,g)': amortized constant time.
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
//          set_prev_in_facet(hds,h,g);
//          set_prev_at_source(hds,h,g);
//          set_prev_at_target(hds,h,g);
//          const_constraints(hds);
//      }
//      void const_constraints(HDS const& hds)
//      {
//      }
//    private:
//      MutableBackwardHDS hds;
//      halfedge_descriptor h;
//      halfedge_descriptor g;
//  };
//..

#ifdef BOOST_HDSTL_CONCEPTS_MUTABLEBACKWARDHDSCONCEPT_HPP
#define BOOST_HDSTL_CONCEPTS_MUTABLEBACKWARDHDSCONCEPT_HPP 1

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

            set_prev_in_facet(hds,h,g);
            set_prev_at_source(hds,h,g);
            set_prev_at_target(hds,h,g);

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
        halfedge_descriptor h;  // a halfedge descriptor
        halfedge_descriptor g;  // another halfedge descriptor
    };

}  // close namespace concepts
}  // close namespace hdstl

#endif
