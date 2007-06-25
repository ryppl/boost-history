//bidirectional_hds_concept.hpp    -*- C++ -*-
//
//@PURPOSE: Provide 'BidirectionalHDS' concept definition, and concept-checking
//class.
//
//@DESCRIPTION: This file provides a single class template that,
// when used with the following concept-checking utilities:
//..
//  boost::function_requires<BidirectionalHDSConcept<HDS> >();
//..
// inside a function body, or
//..
//  BOOST_CLASS_REQUIRE(HDS, boost::hdstl,BidirectionalHDSConcept);
//..
// inside a class body, checks whether the type given as template
// argument is a model of the HDS concept described on the page
// [bidirectionalhds].
//
//@CONCEPT: [bidirectionalhds] Bidirectional HDS concept
//
///Definition
///-----------
// This concept refines the 'ForwardHDSConcept' and the 'BackwardHDSConcept', 
// and specifies additional bidirectional traversal category to iterate over 
// the 'HDS' data structure. 'Bidirectional_category' requires both the 
// 'forward_category' and the 'backward_category'
// defined to iterate over the 'HDS' data structure, which means both 
// clockwise & counter clockwise movement around the facet and vertex. 
// Once one of these categories are given all others can be accessed with 
// the help of the 'opposite' of a halfedge (see the section "Invariants" 
// below for the algebraic relations that must hold).
//
///Refinement of
///-------------
//  - 'ForwardHDSConcept'
//  - 'BackwardHDSConcept'
//
///Associated types
///----------------
// In addition to the types of the 'ForwardHDS' and 'BackwardHDS' concepts:
//  - 'hds_traits<HDS>::traversal_category' :  from 'HDSConcept', with
//     the additional requirement that this type must be convertible to
//     the 'hds_traits<HDS>::bidirectional_traversal_tag' type which is
//     defined in '<boost/hdstl.hpp>'.
//
///Definitions
///-----------
// In addition to the definitions of the 'ForwardHDS' and 'BackwardHDS'
// concepts:
//  - 'traversal_category' is a type that contains information about the 
//     direction of the traversal, which is forward, backward or bidirectional.
//     For this 'BidirectionalHDS' concept, the 'traversal_category' has to be 
//     convertible to the 'hdstl::bidirectional_traversal_tag' type defined 
//     in '<boost/hdstl.hpp>'.
//
///Valid Expressions
///-----------------
// None besides the valid expressions of the 'ForwardHDSConcept' and 
// the 'BackwardHDSConcept'.
//
///Expression Semantics
///--------------------
// Same as the expression semantics of the 'ForwardHDSConcept' and 
// the 'BackwardHDSConcept'.
//
///Complexity guarantees
///---------------------
// Same as the complexity guarantees of the 'ForwardHDSConcept' and 
// the 'BackwardHDSConcept'.
//
///Invariants 
///----------
// 'BidirectionalHDSConcept' should validate all the invariants for the 
// 'ForwardHDSConcept' and the 'BackwardHDSConcept'.
//
///Concept-checking class
///----------------------
//..
// template <class HDS> 
// struct BidirectionalHDSConcept {
//   typedef typename hds_traits<HDS>::halfedge_descriptor halfedge_descriptor; 
//   typedef typename hds_traits<HDS>::traversal_category traversal_category;
//   typedef typename hds_traits<HDS>::forward_category forward_category;
//   typedef typename hds_traits<HDS>::backward_category backward_category;
//    void constraints() {
//       using namespace boost;
//       function_requires<ForwardHDSConcept<HDS> >();
//       function_requires<BackwardHDSConcept<HDS> >();
//       function_requires<ConvertibleConcept<traversal_category,
//                                          bidirectional_traversal_tag> >();  
//    }
//    void const_constraints(HDS const& hds) {
//    }
//    BidirectionalHDS hds;
// };
//..

#ifndef BOOST_HDSTL_CONCEPTS_BIDIRECTIONAL_HDS_CONCEPT_HPP
#define BOOST_HDSTL_CONCEPTS_BIDIRECTIONAL_HDS_CONCEPT_HPP 1

#include <boost/concepts.h>

namespace hdstl{
namespace concepts{
   
    template <class HDS> 
    struct BidirectionalHDSConcept {
        // This class provides a concept checking class for the 
        // 'BidirectionalHDSConcept', i.e., using the statement
        //..
        //  boost::function_requires<BidirectionalHDSConcept<HDS>()
        //..
        // inside a function body, or
        //..
        //  BOOST_CLASS_REQUIRE(HDS, boost::hdstl, BidirectionalHDSConcept);
        //..
        // inside a class definition body, should trigger a compile-time error
        // if the type HDS does not model the 'BidirectionalHDSConcept'.

        // TYPES
        typedef typename hds_traits<HDS>::halfedge_descriptor halfedge_descriptor; 
        typedef typename hds_traits<HDS>::traversal_category traversal_category;
        typedef typename hds_traits<HDS>::forward_category forward_category;
        typedef typename hds_traits<HDS>::backward_category backward_category;
            // The specialization of 'hds_traits<HDS>' must have these required
            // types, obeying the types requirements stated in the detailed
            // description of the 'BidirectionalHDS' concept on page 
            // [bidirectionalhdsconcept].

        //MANIPULATORS
        void constraints() 
            // Check that the 'HDS' template parameter satisfies all the
            // constraints of 'BidirectionalHDSConcept' on page 
            // [bidirectionalhdsconcept].
        {
            using namespace boost;
            function_requires<ForwardHDSConcept<HDS> >();
            function_requires<BackwardHDSConcept<HDS> >();
            function_requires<ConvertibleConcept<traversal_category,
                bidirectional_traversal_tag> >();  
        }

        // ACCESSORS
        void const_constraints(HDS const& hds)
            // Check that the non-modifiable 'HDS' template parameters
            // satisfies all the constraints of 'BidirectionalHDSConcept'.
        {
        }

      private:
        //DATA
        BidirectionalHDS hds;  // a halfedge data structure object
   };

}  // close namespace concepts
}  // close namespace hdstl

#endif
