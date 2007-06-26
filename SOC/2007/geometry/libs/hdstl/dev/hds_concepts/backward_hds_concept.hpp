//backward_hds_concept.hpp  -*- C++ -*-
//
//@PURPOSE: Provide 'BackwardHDS' concept definition and concept-checking class.
//
//@DESCRIPTION: This file provides a single class template that,
// when used with the following concept-checking utilities:
//..
//  boost::function_requires<BackwardHDSConcept<HDS> >();
//..
// inside a function body, or
//..
//  BOOST_CLASS_REQUIRE(HDS, boost::hdstl, BackwardHDSConcept);
//..
// inside a class body, checks whether the type given as template
// argument is a model of the HDS concept described on the page
// [backwardhds].
//
//@CONCEPT: [backwardhds] Backward HDS concept
//
///Definition
///----------
// This concept refines the 'HDSConcept', and specifies additional backward
// category to iterate over the 'HDS' data structure, which means clockwise 
// movement around the facet and counter clockwise movement around the
// vertex. Possible backward categories are 'prev_in_facet', 
// 'prev_at_source', and 'prev_at_target'. Once one of these categories 
// are given all others can be accessed with the help of the 'opposite' of a 
// halfedge (see the section "Invariants" below for the algebraic relations
// that must hold).
//
///Refinement of
///-------------
//  - 'HDSConcept'
//
///Notation
///--------
//  - 'HDS'    A type that is a model of 'BackwardHDSConcept'
//  - 'hds'    A non-modifiable instance of 'HDS'
//  - 'h','g'  Halfedge descriptors, of type 'hds_traits<HDS>::halfedge_descriptor'
//
///Associated types
///----------------
// In addition to the types of the 'HDS' concept:
//  - 'hds_traits<HDS>::traversal_category':  from 'HDSConcept', with
//     additional requirement that this type must be convertible to
//     'hdstl::backward_traversal_tag'.
//  - 'hds_traits<HDS>::backward_category':   defines the pointer type for 
//     backward iteration.
//
///Definitions
///-----------
// In addition to the definitions of the 'HDS' concept:
//  - 'traversal_category' is a type that contains information about the 
//     direction of the traversal, which is forward, backward or bidirectional.
//     For this 'BackwardHDS' concept, the 'traversal_category' has to be 
//     convertible to the 'hdstl::backward_traversal_tag' type defined in 
//     '<boost/hdstl.hpp>'.
//  - 'backward_category' defines the primary accessor of the backward
//     information for use by algorithms which desire the most direct access. 
//
///Valid Expressions
///-----------------
// In addition to the valid expressions of the 'HDS' concept:
//   - 'prev_in_facet(h, hds)' must return a value assignable to 'h'.
//   - 'prev_at_source(h, hds)' must return a value assignable to 'h'.
//   - 'prev_at_target(h, hds)' must return a value assignable to 'h'.
//
///Expression Semantics
///--------------------
// In addition to the expression semantics of the 'HDS' concept:
//   - 'prev_in_facet(h, hds)' returns a halfedge descriptor to the halfedge 
//      preceding 'h' in the adjacent facet cycle, when facet cycles are
//      oriented in counter-clockwise order.
//   - 'prev_at_source(h, hds)' returns a halfedge descriptor to the halfedge 
//      preceding 'h' around the source vertex of 'h', when halfedges are
//      ordered around a given vertex in clockwise order.
//   - 'prev_at_target(h, hds)' returns a halfedge descriptor to the halfedge 
//      preceding 'h' around the target vertex of 'h', when halfedges are
//      ordered around a given vertex in clockwise order.
//
///Complexity guarantees
///---------------------
// In addition to the complexity guarantees of the 'HDS' concept:
//  - 'prev_in_facet(h, hds)': amortized constant time.
//  - 'prev_at_source(h, hds)': amortized constant time.
//  - 'prev_at_target(h, hds)': amortized constant time.
//
///Invariants 
///----------
// The backward halfedge accessors are linked via the algebraic relations:
//   - 'prev_in_facet(h, hds)'  == 'prev_at_target(opposite(h, hds), hds)' 
//                                  == 'opposite(prev_at_source(h, hds), hds)'
//   - 'prev_at_source(h, hds)' == 'opposite(prev_in_facet(h, hds), hds)' 
//                             == 'opposite(prev_at_target(opposite(h, hds)))'
//   - 'prev_at_target(h, hds)' == 'prev_in_facet(opposite(h, hds), hds)'
//                   == 'opposite(prev_at_source(opposite(h, hds), hds), hds)'
//
///Concept-checking class
///----------------------
//..
// template <class HDS> 
// struct BackwardHDSConcept {
//   typedef typename hds_traits<HDS>::halfedge_descriptor halfedge_descriptor; 
//   typedef typename hds_traits<HDS>::traversal_category traversal_category;
//   typedef typename hds_traits<HDS>::backward_category backward_category;
//    void constraints() {
//       using namespace boost;
//       function_requires<HDSConcept<HDS> >();
//       function_requires<ConvertibleConcept<traversal_category,
//                                        hdstl::backward_traversal_tag> >();  
//       const bool is_valid_storage_tag =
//          is_convertible<backward_category,prev_at_source_tag>::value ||
//          is_convertible<backward_category,prev_at_target_tag>::value ||
//          is_convertible<backward_category,prev_in_facet_tag>::value;
//       BOOST_STATIC_ASSERT( is_valid_storage_tag ); 
//
//       const_constraints(hds);
//    }
//    void const_constraints(HDS const& hds){
//       h = prev_in_facet(h, hds);
//       h = prev_at_source(h, hds);
//       h = prev_at_target(h, hds);
//    }
//    BackwardHDS hds;
//    halfedge_descriptor h;
//    halfedge_descriptor g;
// };
//..

#ifndef BOOST_HDSTL_CONCEPTS_BACKWARD_HDS_CONCEPT_HPP
#define BOOST_HDSTL_CONCEPTS_BACKWARD_HDS_CONCEPT_HPP 1

namespace hdstl{
namespace concepts{

    template <class HDS> 
    struct BackwardHDSConcept {
        // This class provides a concept checking class for the BackwardHDSConcept,
        // i.e., using the statement
        //..
        //  boost::function_requires<BackwardHDSConcept<HDS>()
        //..
        // inside a function body, or
        //..
        //  BOOST_CLASS_REQUIRE(HDS, boost::hdstl, BackwardHDSConcept);
        //..
        // inside a class definition body, should trigger a compile-time error
        // if the type HDS does not model the 'BackwardHDSConcept'.

        // TYPES
        typedef typename hds_traits<HDS>::halfedge_descriptor halfedge_descriptor; 
        typedef typename hds_traits<HDS>::traversal_category traversal_category;
        typedef typename hds_traits<HDS>::backward_category backward_category;

        // MANIPULATORS
        void constraints()
            // Check that the 'HDS' template parameter satisfies all the
            // constraints of 'BackwardHDSConcept' on page 
            // [backwardhdsconcept].
	{
            using namespace boost;
            function_requires<HDSConcept<HDS> >();
            function_requires<ConvertibleConcept<traversal_category,
                                                 backward_traversal_tag> >();  
            const bool is_valid_storage_tag =
                  is_convertible<backward_category,prev_at_source_tag>::value ||
                  is_convertible<backward_category,prev_at_target_tag>::value ||
                  is_convertible<backward_category,prev_in_facet_tag>::value;
            BOOST_STATIC_ASSERT( is_valid_storage_tag ); 

            h = prev_in_facet(h, hds);
            h = prev_at_source(h, hds);
            h = prev_at_target(h, hds);
            const_constraints(hds);
        }

        // ACCESSORS
        void const_constraints(HDS const& hds)
            // Check that the non-modifiable 'HDS' template parameters
            // satisfies all the constraints of 'BackwardHDSConcept'.
	{
            h = prev_in_facet(h, hds);
            h = prev_at_source(h, hds);
            h = prev_at_target(h, hds);
        }

      private:
        // DATA
        BackwardHDS hds;        // a halfedge data structure object
        halfedge_descriptor h;  // a halfedge descriptor
        halfedge_descriptor g;  // another halfedge descriptor
    };

}  // close namespace concepts
}  // close namespace hdstl

#endif
