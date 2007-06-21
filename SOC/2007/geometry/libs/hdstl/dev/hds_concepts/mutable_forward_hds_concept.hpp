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
//  - 'hds'    A non-modifiable instance of 'HDS'
//  - 'h','g'  Halfedge descriptors, of type 
//    'hds_traits<HDS>::halfedge_descriptor'
//
///Associated types
///----------------
//  - 'hds_traits<HDS>::halfedge_descriptor': from 'HDSConcept'.
//  - 'hds_traits<HDS>::traversal_category':  from 'ForwardHDSConcept'.
//  - 'hds_traits<HDS>::forward_category':   from 'ForwardHDSConcept'.
//
///Definitions
///-----------
//  - 'halfedge_descriptor' is a type that contains information to access 
//     the halfedge.  (See the 'HDS' concept for a full definition.)
//  - 'traversal_category' is a type that contains information about the 
//     direction of the traversal, which is forward, backward or bidirectional.
//     (See the 'HDS' concept for a full definition.)
//  - 'forward_category' defines the primary accessor of the forward
//     information for use by algorithms which desire the most direct access. 
//     (See the 'HDS' concept for a full definition.)
//
///Valid Expressions
///-----------------
// In addition to the valid expressions of the 'HDS' concept, and the 
// 'ForwardHDS' concept:
//   - 'set_next_in_facet(hds,h,g)' sets 'g' as the 'next_in_facet' value of 
//     'h'.
//   - 'set_next_at_source(hds,h,g)' sets 'g' as the 'next_at_source' value of 
//     'h'.
//   - 'set_next_at_target(hds,h,g)' sets 'g' as the 'next_at_target' value of
//     'h'.
//
///Expression Semantics
///--------------------
// In addition to the expression semantics of the 'HDS' concept, and the
// 'ForwardHDS' concept:
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
//      halfedge_descriptor h;
//      halfedge_descriptor g;
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
        halfedge_descriptor h;  // a halfedge descriptor
        halfedge_descriptor g;  // another halfedge descriptor
    };

}  // close namespace concepts
}  // close namespace hdstl

#endif
