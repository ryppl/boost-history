//forward_hds_concept.hpp  -*- C++ -*-
//
//@PURPOSE: Provide 'ForwardHDS' concept definition and concept-checking class.
//
//@DESCRIPTION: This file provides a single class template that,
// when used with the following concept-checking utilities:
//..
//  boost::function_requires<ForwardHDSConcept<HDS> >();
//..
// inside a function body, or
//..
//  BOOST_CLASS_REQUIRE(HDS, boost::hdstl, ForwardHDSConcept);
//..
// inside a class body, checks whether the type given as template
// argument is a model of the HDS concept described on the page
// [forwardhdsconcept].
//
//@CONCEPT: [forwardhdsconcept] Forward HDS concept
//
///Definition
///----------
// This concept refines the 'HDSConcept', and specifies additional forward
// category to iterate over the 'HDS' data structure, which means clockwise 
// movement around the facet and counter clockwise movement around the
// vertex. Possible forward categories are 'next_in_facet', 
// 'next_at_source', and 'next_at_target'. Once one of these categories 
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
//  - 'HDS'    A type that is a model of 'ForwardHDSConcept'
//  - 'hds'    A non-modifiable instance of 'HDS'
//  - 'h'      Halfedge descriptors, of type 'hds_traits<HDS>::halfedge_descriptor'
//
///Associated types
///----------------
// In addition to the types of the 'HDS' concept:
//  - 'hds_traits<HDS>::traversal_category':  from 'HDSConcept', with
//     additional requirement that this type must be convertible to
//     'hdstl::forward_traversal_tag'.
//  - 'hds_traits<HDS>::forward_category': defines the pointer type for 
//     forward iteration.
//
///Definitions
///-----------
// In addition to the definitions of the 'HDS' concept:
//  - 'traversal_category' is a type that contains information about the 
//     direction of the traversal, which is forward, backward or bidirectional.
//     For this 'ForwardHDS' concept, the 'traversal_category' has to be 
//     convertible to the 'hdstl::forward_traversal_tag' type defined in 
//     '<boost/hdstl.hpp>'.
//  - 'forward_category' defines the primary accessor of the forward
//     information for use by algorithms which desire the most direct access. 
//
///Valid Expressions
///-----------------
// In addition to the valid expressions of the 'HDS' concept:
//   - 'next_in_facet(h, hds)' must return a value assignable to 'h'.
//   - 'next_at_source(h, hds)' must return a value assignable to 'h'.
//   - 'next_at_target(h, hds)' must return a value assignable to 'h'.
//
///Expression Semantics
///--------------------
// In addition to the expression semantics of the 'HDS' concept:
//   - 'next_in_facet(h, hds)' returns a halfedge descriptor to the halfedge 
//      succeeding 'h' in the adjacent facet cycle, when facet cycles are
//      oriented in counter-clockwise order.
//   - 'next_at_source(h, hds)' returns a halfedge descriptor to the halfedge 
//      succeeding 'h' around the source vertex of 'h', when halfedges are
//      ordered around a given vertex in clockwise order.
//   - 'next_at_target(h, hds)' returns a halfedge descriptor to the halfedge 
//      succeeding 'h' around the target vertex of 'h', when halfedges are
//      ordered around a given vertex in clockwise order.
//
///Complexity guarantees
///---------------------
// In addition to the complexity guarantees of the 'HDS' concept:
//  - 'next_in_facet(h, hds)': amortized constant time.
//  - 'next_at_source(h, hds)': amortized constant time.
//  - 'next_at_target(h, hds)': amortized constant time.
//
///Invariants 
///----------
// The forward halfedge accessors are linked via the algebraic relations:
//   - 'next_in_facet(h, hds)'  == 'next_at_source(opposite(h, hds), hds)' 
//                              == 'opposite(next_at_target(h, hds), hds)'
//   - 'next_at_source(h, hds)' == 'next_in_facet(opposite(h, hds), hds)'
//               == 'opposite(next_at_target(opposite(h, hds), hds), hds)'
//   - 'next_at_target(h, hds)' == 'opposite(next_in_facet(h, hds), hds)' 
//               == 'opposite(next_at_source(opposite(h, hds), hds), hds)'
//
///Concept-checking class
///----------------------
//..
//  template <class HDS> 
//  struct ForwardHDSConcept {
//    typedef typename hds_traits<HDS>::halfedge_descriptor halfedge_descriptor; 
//    typedef typename hds_traits<HDS>::traversal_category traversal_category;
//    typedef typename hds_traits<HDS>::forward_category forward_category;
//     void constraints() {
//        using namespace boost;
//        function_requires<HDSConcept<HDS> >();
//        function_requires<ConvertibleConcept<traversal_category,
//                                         hdstl::forward_traversal_tag> >();  
//        const bool is_valid_storage_tag =
//           is_convertible<forward_category,next_at_source_tag>::value ||
//           is_convertible<forward_category,next_at_target_tag>::value ||
//           is_convertible<forward_category,next_in_facet_tag>::value;
//        BOOST_STATIC_ASSERT( is_valid_storage_tag ); 
//
//        h = next_in_facet(h, hds);
//        h = next_at_source(h, hds);
//        h = next_at_target(h, hds);
//        const_constraints(hds);
//     }
//     void const_constraints(HDS const& hds){
//        h = next_in_facet(h, hds);
//        h = next_at_source(h, hds);
//        h = next_at_target(h, hds);
//     }
//     ForwardHDS hds;
//     halfedge_descriptor h;
//     halfedge_descriptor g;
//  };
//..

#ifndef BOOST_HDSTL_CONCEPTS_FORWARD_HDS_CONCEPT_HPP
#define BOOST_HDSTL_CONCEPTS_FORWARD_HDS_CONCEPT_HPP 1

namespace boost {
namespace hdstl{
namespace concepts{

    template <class HDS> 
    struct ForwardHDSConcept {
        // This class provides a concept checking class for the 
        // 'ForwardHDSConcept', i.e., using the statement
        //..
        //  boost::function_requires<ForwardHDSConcept<HDS>()
        //..
        // inside a function body, or
        //..
        //  BOOST_CLASS_REQUIRE(HDS, boost::hdstl, ForwardHDSConcept);
        //..
        // inside a class definition body, should trigger a compile-time error
        // if the type HDS does not model the 'ForwardHDSConcept'.

        // OPAQUE TYPES
        typedef typename hds_traits<HDS>::halfedge_descriptor halfedge_descriptor; 
        typedef typename hds_traits<HDS>::traversal_category traversal_category;
        typedef typename hds_traits<HDS>::forward_category forward_category;
            // The specialization of 'hds_traits<HDS>' must have these required
            // types, obeying the types requirements stated in the detailed
            // description of the 'ForwardHDS' concept on page 
            // [forwardhdsconcept].

        //  OPAQUE MANIPULATORS
        void constraints()
            // Check that the 'HDS' template parameter satisfies all the
            // constraints of 'ForwardHDSConcept' on page 
            // [forwardhdsconcept].
       	{
            using namespace boost;
            function_requires<HDSConcept<HDS> >();
            function_requires<ConvertibleConcept<traversal_category,
                                                 forward_traversal_tag> >();  

            const bool is_valid_storage_tag =
                   is_convertible<forward_category,next_at_source_tag>::value ||
                   is_convertible<forward_category,next_at_target_tag>::value ||
                   is_convertible<forward_category,next_in_facet_tag>::value;
            BOOST_STATIC_ASSERT( is_valid_storage_tag ); 

            h = next_in_facet(h, hds);
            h = next_at_source(h, hds);
            h = next_at_target(h, hds);
            const_constraints(hds);
        }

        // OPAQUE ACCESSORS
        void const_constraints(HDS const& hds)
            // Check that the non-modifiable 'HDS' template parameters
            // satisfies all the constraints of 'ForwardHDSConcept'.
	{
            h = next_in_facet(h, hds);
            h = next_at_source(h, hds);
            h = next_at_target(h, hds);
        }

      private:
        // DATA
        HDS hds;                // a halfedge data structure object
        halfedge_descriptor h;  // a halfedge descriptor
        halfedge_descriptor g;  // another halfedge descriptor
    };

}  // close namespace concepts
}  // close namespace hdstl
}  // close namespace boost

#endif
