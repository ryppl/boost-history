//halfedge_list_hds_concept.hpp    -*- C++ -*- 
//
//@PURPOSE: Provide 'HalfedgeListHDS' concept definition, and concept checking class.
//
//@DESCRIPTION: This file provides a single class template that,
// when used with the following concept-checking utilities:
//..
//  boost::function_requires<HalfedgeListHDSConcept<HDS> >();
//..
// inside a function body, or
//..
//  BOOST_CLASS_REQUIRE(HDS, boost::hdstl,HalfedgeListHDSConcept);
//..
// inside a class body, checks whether the type given as template
// argument is a model of the HDS concept described on the page 
// [halfedgelisthds].
//
//@CONCEPT: [halfedgelisthds] Halfedge List HDS concept
//
///Definition
///----------
// This concept refines the 'HDSConcept', and specifies additional accessors
// to handle halfedges in the 'HDS' data structure. 
//
///Refinement of:
///--------------
//  - 'HDSConcept'
//
///Notation
///--------
//  - 'HDS'  A type that is a model of HalfedgeListHDSConcept
//  - 'hds'  A non-modifiable instance of HDS
//  - 'h'    Halfedge descriptor, of type 'hds_traits<HDS>::halfedge_descriptor'
//  - 'p'    Halfedge iterator pair, of type 
//           'std::pair<halfedge_iterator,halfedge_iterator>'
//  - 'H'    Halfedge size type, of type 'hds_traits<HDS>::halfedge_size_type'
//
///Associated types
///----------------
//  - 'hds_traits<HDS>::halfedge_descriptor': must be 'DefaultConstructible', 
//    'CopyConstructible', 'EqualityComparable', and 'Assignable'.
//  - 'hds_traits<HDS>::halfedge_iterator': must be 'MultiPassInputIterator'.
//  - 'hds_traits<HDS>::halfedge_size_type': halfedge size type.
//
///Definitions
///-----------
//  - 'halfedge_descriptor' is a type that contains information to access 
//     the halfedge.  (See the 'HDSConcept' for a full definition.)
//  - 'halfedge_iterator' is an iterator type for the halfedges.
//  - 'halfedge_size_type' defines the size type.
//
///Valid Expressions
///-----------------
// In addition to the valid expressions of the 'HDS' concept:
//  - 'halfedges(hds)' must return a value assignable to 'p'.  
//  - '*p.first' must return a value assignable to 'h'.
//  - 'num_halfedges(HDS)' must return a value assignable to 'H'.
//
///Expression Semantics
///--------------------
// In addition to the valid expression semantics of the 'HDS' concept:
//  - 'halfedges(hds)' returns a 'halfedge_iterator' pair 'p', consistent 
//    with the STL style iterator ranges, which means all the halfedges
//    can be accessed by increasing the first part of the pair until the second
//    part is reached.
//  - '*p.first' returns a 'halfedge_descriptor'.
//  - 'num_halfedges(HDS)' returns the number of halfedges in the 'HDS' data
//    structure.
//
///Complexity guarantees
///---------------------
//  - 'halfedges(hds)'    : amortized constant time. (for just getting the 
//    'halfedge_iterator' pair, accessing the halfedges takes linear time.)
//  - '*p.first'          : amortized constant time.
//  - 'num_halfedges(HDS)': amortized constant time.
//
///Invariants 
///----------
// 'HalfedgeListHDSConcept' should validate all the invariants for the 
// 'HDSConcept'.
//
///Concept-checking class
///----------------------
//..
// template <class HDS> 
// struct HalfedgeListHDSConcept {
//    typedef typename hds_traits<HDS>::halfedge_iterator  halfedge_iterator;
//    typedef typename hds_traits<HDS>::halfedge_descriptor halfedge_descriptor;
//    typedef typename hds_traits<HDS>::halfedges_size_type halfedges_size_type;
//     void constraints() 
//     {
//         using namespace boost;
//         function_requires<HDSConcept>();
//         function_requires<DefaultConstructibleConcept<halfedge_descriptor> >();
//         function_requires<CopyConstructibleConcept<halfedge_descriptor> >();
//         function_requires<EqualityComparableConcept<halfedge_descriptor> >();
//         function_requires<AssignableConcept<halfedge_descriptor> >();
//         function_requires<hdstl_detail::MultiPassInputIteratorConcept<halfedge_iterator> >();
//         p = halfedges(hds);
//         h = *p.first;
//         H = num_halfedges(hds);
//         const_constraints(hds);
//     }
//     void const_constraints(HDS const& hds) 
//     {
//         p = halfedges(hds);
//         h = *p.first;
//         H = num_halfedges(hds);
//     }

//     private:
//     HalfedgeListHDS hds;
//     halfedge_descriptor h;
//     std::pair<halfedge_iterator,halfedge_iterator> p;
//     halfedge_size_type H;
// };
//..

#ifndef BOOST_HDSTL_CONCEPTS_HALFEDGE_LIST_HDS_CONCEPT_HPP
#define BOOST_HDSTL_CONCEPTS_HALFEDGE_LIST_HDS_CONCEPT_HPP 1

#include <boost/concepts.h>

namespace hdstl{
namespace concepts {

   template <class HDS> 
   struct HalfedgeListHDSConcept {
       // This class provides a concept checking class for the 
       // 'HalfedgeListHDSConcept', i.e., using the statement
       //..
       //  boost::function_requires<HalfedgeListHDSConcept<HDS>()
       //..
       // inside a function body, or
       //..
       //  BOOST_CLASS_REQUIRE(HDS, boost::hdstl, HalfedgeListHDSConcept);
       //..
       // inside a class definition body, should trigger a compile-time error
       // if the type HDS does not model the 'HalfedgeListHDSConcept'.

       // TYPES
       typedef typename hds_traits<HDS>::halfedge_iterator  halfedge_iterator;
       typedef typename hds_traits<HDS>::halfedge_descriptor halfedge_descriptor;
       typedef typename hds_traits<HDS>::halfedges_size_type halfedges_size_type;
       // The specialization of 'hds_traits<HDS>' must have these required
       // types, obeying the types requirements stated in the detailed
       // description of the 'HalfedgeListHDS' concept on page 
       // [halfedgelisthdsconcept].

       //MANIPULATORS
       void constraints() 
           // Check that the 'HDS' template parameters satisfies all the
           // constraints of 'HalfedgeListHDSConcept' on page 
           // [halfedgelisthdsconcept].
       {
           using namespace boost;
           function_requires<HDSConcept>();
           function_requires<DefaultConstructibleConcept<halfedge_descriptor> >();
           function_requires<CopyConstructibleConcept<halfedge_descriptor> >();
           function_requires<EqualityComparableConcept<halfedge_descriptor> >();
           function_requires<AssignableConcept<halfedge_descriptor> >();
           function_requires<hdstl_detail::MultiPassInputIteratorConcept<halfedge_iterator> >();
           p = halfedges(hds);
           h = *p.first;
           H = num_halfedges(hds);
           const_constraints(hds);
       }

       // ACCESSORS
       void const_constraints(HDS const& hds) 
           // Check that the non-modifiable 'HDS' template parameters
           // satisfies all the constraints of 'HalfedgeListHDSConcept'.
       {
           p = halfedges(hds);
           h = *p.first;
           H = num_halfedges(hds);
       }

       private:
       //DATA
       HalfedgeListHDS hds;         // a halfedge data structure object
       halfedge_descriptor h;   // a halfedge descriptor
       std::pair<halfedge_iterator,halfedge_iterator> p; // a halfedge iterator pair
       halfedge_size_type H; // halfedge size type

   };

}  // close namespace concepts
}  // close namespace hdstl

#endif
