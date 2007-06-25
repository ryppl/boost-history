//mutable_forward_hds_concept.hpp  -*- C++ -*-
//
//@PURPOSE: Provide 'MutableForwardHDS' concept definition and concept-checking class.
//
//@DESCRIPTION: This file provides a single class template that,
// when used with the following concept-checking utilities:
//..
//  boost::function_requires<MutableForwardHDSConcept<HDS> >();
//..
// inside a function body, or
//..
//  BOOST_CLASS_REQUIRE(HDS, boost::hdstl, MutableForwardHDSConcept);
//..
// inside a class body, checks whether the type given as template
// argument is a model of the HDS concept described on the page
// [mutableforwardhds].
//
//@CONCEPT: [mutableforwardhds] Mutable Forward HDS concept
//
///Definition
///----------
// This concept refines the 'ForwardHDSConcept' and the 'MutableHDSConcept' 
// with additional mutable operations for the halfedge data structure.
//
///Refinement of
///-------------
//  - 'MutableHDSConcept'
//  - 'ForwardHDSConcept'
//
///Notation
///--------
//  - 'HDS'    A type that is a model of 'MutableForwardHDSConcept'
//  - 'hds'    A modifiable instance of 'HDS'
//  - 'h','g'  Halfedge descriptors, of type 
//    'hds_traits<HDS>::halfedge_descriptor'
//
///Associated types
///----------------
// None besides the types in the 'MutableHDS' and the 'ForwardHDS' concepts.
//
///Definitions
///-----------
// Same as the definitions in the 'MutableHDS' and the 'ForwardHDS' concepts.
//
///Valid Expressions
///-----------------
// In addition to the valid expressions of the 'MutableHDS' and the 
// 'ForwardHDS' concepts:
//   - 'set_next_in_facet(hds,h,g)' sets 'g' as the 'next_in_facet' value of 
//     'h'.
//   - 'set_next_at_source(hds,h,g)' sets 'g' as the 'next_at_source' value of 
//     'h'.
//   - 'set_next_at_target(hds,h,g)' sets 'g' as the 'next_at_target' value of
//     'h'.
//
///Expression Semantics
///--------------------
// In addition to the expression semantics of the 'MutableHDS' and the
// 'ForwardHDS' concepts:
//   - 'set_next_in_facet(hds,h,g)' sets 'g' as the halfedge 
//      succeeding 'h' in the adjacent facet cycle, when facet cycles are
//      oriented in counter-clockwise order.
//   - 'set_next_at_source(hds,h,g)' sets 'g' as the halfedge 
//      succeeding 'h' around the source vertex of 'h', when halfedges are
//      ordered around a given vertex in clockwise order.
//   - 'set_next_at_target(hds,h,g)' sets 'g' as the halfedge  
//      succeeding 'h' around the target vertex of 'h', when halfedges are
//      ordered around a given vertex in clockwise order.
//
///Complexity guarantees
///---------------------
// In addition to the complexity guarantees of the 'MutableHDS' and the
// 'ForwardHDS' concepts:
//  - 'set_next_in_facet(hds,h,g)': amortized constant time.
//  - 'set_next_at_source(hds,h,g)': amortized constant time.
//  - 'set_next_at_target(hds,h,g)': amortized constant time.
//
///Invariants 
///----------
// All the 'ForwardHDS' invariants should hold after the mutable 
// operations. (See the 'ForwardHDS' concept invariants for a full definition.)
//
///Concept-checking class
///----------------------
//..
//  template <class HDS> 
//  struct MutableForwardHDSConcept {
//   typedef typename hds_traits<HDS>::halfedge_descriptor halfedge_descriptor; 
//      typedef typename hds_traits<HDS>::traversal_category traversal_category;
//      typedef typename hds_traits<HDS>::forward_category forward_category;
//      void constraints() 
//      {
//          using namespace boost;
//          function_requires<ForwardHDSConcept<HDS> >();
//          function_requires<MutableHDSConcept<HDS> >();
//          set_next_in_facet(hds,h,g);
//          set_next_at_source(hds,h,g);
//          set_next_at_target(hds,h,g);
//          const_constraints(hds);
//      }
//      void const_constraints(HDS const& hds)
//      {
//      }
//    private:
//      MutableForwardHDS hds;
//      halfedge_descriptor h,g;
//  };
//..

#ifndef BOOST_HDSTL_CONCEPTS_MUTABLE_FORWARD_HDS_CONCEPT_HPP
#define BOOST_HDSTL_CONCEPTS_MUTABLE_FORWARD_HDS_CONCEPT_HPP 1

#include <boost/concepts.h>

namespace hdstl{
namespace concepts{

    template <class HDS> 
    struct MutableForwardHDSConcept {
        // This class provides a concept checking class for the 
        // MutableForwardHDSConcept, i.e., using the statement
        //..
        //  boost::function_requires<MutableForwardHDSConcept<HDS>()
        //..
        // inside a function body, or
        //..
        //  BOOST_CLASS_REQUIRE(HDS, boost::hdstl, MutableForwardHDSConcept);
        //..
        // inside a class definition body, should trigger a compile-time error
        // if the type HDS does not model the 'MutableForwardHDSConcept'.

        // TYPES
        typedef typename hds_traits<HDS>::halfedge_descriptor halfedge_descriptor; 
        typedef typename hds_traits<HDS>::traversal_category traversal_category;
        typedef typename hds_traits<HDS>::forward_category forward_category;
            // The specialization of 'hds_traits<HDS>' must have these
            // required types, obeying the types requirements stated in the
            // detailed description of the 'MutableForwardHDS' concept on page
            // [mutableforwardhdsconcept].

        // MANIPULATORS
        void constraints() 
            // Check that the 'HDS' template parameters satisfies all the
            // constraints of 'MutableForwardHDSConcept' on page 
            // [mutableforwardhdsconcept].
        {
            using namespace boost;

            function_requires<ForwardHDSConcept<HDS> >();
            function_requires<MutableHDSConcept<HDS> >();

            set_next_in_facet(hds,h,g);
            set_next_at_source(hds,h,g);
            set_next_at_target(hds,h,g);

            const_constraints(hds);
        }

        // ACCESSORS
        void const_constraints(HDS const& hds)
            // Check that the non-modifiable 'HDS' template parameters
            // satisfies all the constraints of 'MutableForwardHDSConcept'.
        {
        }

      private:
        // DATA
        MutableForwardHDS hds; // a halfedge data structure object
        halfedge_descriptor h,g;  // halfedge descriptors
    };

}  // close namespace concepts
}  // close namespace hdstl

#endif
