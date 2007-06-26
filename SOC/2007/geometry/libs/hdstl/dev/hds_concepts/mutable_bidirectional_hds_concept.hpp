//mutable_bidirectional_hds_concept.hpp    -*- C++ -*-
//
//@PURPOSE: Provide 'MutableBidirectionalHDS' concept definition, and 
//concept-checking class.
//
//@DESCRIPTION: This file provides a single class template that,
// when used with the following concept-checking utilities:
//..
//  boost::function_requires<MutableBidirectionalHDSConcept<HDS> >();
//..
// inside a function body, or
//..
//  BOOST_CLASS_REQUIRE(HDS, boost::hdstl,MutableBidirectionalHDSConcept);
//..
// inside a class body, checks whether the type given as template
// argument is a model of the HDS concept described on the page
// [mutablebidirectionalhds].
//
//@CONCEPT: [mutablebidirectionalhds] Mutable Bidirectional HDS concept
//
///Definition
///-----------
// This concept refines the 'BidirectionalHDSConcept', 
// 'MutableForwardHDSConcept' and the 'MutableBackwardHDSConcept', 
// and specifies additional mutable operations to manipulate 
// the 'HDS' data structure. 
//
///Refinement of
///-------------
//  - 'BidirectionalHDSConcept'
//  - 'MutableForwardHDSConcept'
//  - 'MutableBackwardHDSConcept'
//
///Notation
///--------
//  - 'HDS'     A type that is a model of 'MutableBidirectionalHDSConcept'
//  - 'hds'     A modifiable instance of 'HDS'
//  - 'h','g'   Halfedge descriptors, of type 
//              'hds_traits<HDS>::halfedge_descriptor'
//
///Associated types
///----------------
//  None besides the types in the 'BidirectionalHDS', 'MutableForwardHDS',
//  and 'MutableBackwardHDS' concepts
//
///Valid Expressions
///-----------------
//  Same as the valid expressions in the 'BidirectionalHDS', 
//  'MutableForwardHDS', and 'MutableBackwardHDS' concepts
//
///Expression Semantics
///--------------------
//  Same as the expression semantics in the 'BidirectionalHDS', 
//  'MutableForwardHDS', and 'MutableBackwardHDS' concepts
//
///Complexity guarantees
///---------------------
//  Same as the complexity guarantees in the 'BidirectionalHDS', 
//  'MutableForwardHDS', and 'MutableBackwardHDS' concepts
//
///Invariants 
///----------
// 'MutableBidirectionalHDSConcept' should validate all the invariants for the 
// 'MutableForwardHDSConcept', and the 'MutableBackwardHDSConcept'.
//
///Concept-checking class
///----------------------
//..
//  template <class HDS> 
//  struct MutableBidirectionalHDSConcept {
//    typedef typename hds_traits<HDS>::halfedge_descriptor halfedge_descriptor; 
//    typedef typename hds_traits<HDS>::traversal_category traversal_category;
//    typedef typename hds_traits<HDS>::forward_category forward_category;
//    typedef typename hds_traits<HDS>::backward_category backward_category;
//     void constraints() {
//        using namespace boost;
//        function_requires<BidirectionalHDSConcept<HDS> >();
//        function_requires<MutableForwardHDSConcept<HDS> >();
//        function_requires<MutableBackwardHDSConcept<HDS> >();
//        function_requires<ConvertibleConcept<traversal_category,
//                                           bidirectional_traversal_tag> >();  
//     }
//     void const_constraints(HDS const& hds) {
//     }
//     MutableBidirectionalHDS hds;
//  };
//..

#ifndef BOOST_HDSTL_CONCEPTS_MUTABLE_BIDIRECTIONAL_HDS_CONCEPT_HPP
#define BOOST_HDSTL_CONCEPTS_MUTABLE_BIDIRECTIONAL_HDS_CONCEPT_HPP 1

#include <boost/concepts.h>

namespace hdstl{
namespace concepts{
   
    template <class HDS> 
    struct MutableBidirectionalHDSConcept {
        // This class provides a concept checking class for the 
        // 'MutableBidirectionalHDSConcept', i.e., using the statement
        //..
        //  boost::function_requires<MutableBidirectionalHDSConcept<HDS>()
        //..
        // inside a function body, or
        //..
        //  BOOST_CLASS_REQUIRE(HDS, boost::hdstl, MutableBidirectionalHDSConcept);
        //..
        // inside a class definition body, should trigger a compile-time error
        // if the type HDS does not model the 'MutableBidirectionalHDSConcept'.

        // TYPES
        typedef typename hds_traits<HDS>::halfedge_descriptor halfedge_descriptor; 
        typedef typename hds_traits<HDS>::traversal_category traversal_category;
        typedef typename hds_traits<HDS>::forward_category forward_category;
        typedef typename hds_traits<HDS>::backward_category backward_category;
            // The specialization of 'hds_traits<HDS>' must have these required
            // types, obeying the types requirements stated in the detailed
            // description of the 'MutableBidirectionalHDS' concept on page 
            // [mutablebidirectionalhdsconcept].

        //MANIPULATORS
        void constraints() 
            // Check that the 'HDS' template parameters satisfies all the
            // constraints of 'MutableBidirectionalHDSConcept' on page 
            // [mutablebidirectionalhdsconcept].
        {
            using namespace boost;
            function_requires<BidirectionalHDSConcept<HDS> >();
            function_requires<MutableForwardHDSConcept<HDS> >();
            function_requires<MutableBackwardHDSConcept<HDS> >();
            function_requires<ConvertibleConcept<traversal_category,
                                                 bidirectional_traversal_tag> >();  
        }

        // ACCESSORS
        void const_constraints(HDS const& hds)
            // Check that the non-modifiable 'HDS' template parameters
            // satisfies all the constraints of 
            // 'MutableBidirectionalHDSConcept'.
        {
        }

      private:
        //DATA
        MutableBidirectionalHDS hds;     // a halfedge data structure object
   };

}  // close namespace concepts
}  // close namespace hdstl

#endif
