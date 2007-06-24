//facet_hds_concept.hpp    -*- C++ -*- 
//
//@PURPOSE: Provide 'FacetHDS' concept definition, and concept checking class.
//
//@DESCRIPTION: This file provides a single class template that,
// when used with the following concept-checking utilities:
//..
//  boost::function_requires<FacetHDSConcept<HDS> >();
//..
// inside a function body, or
//..
//  BOOST_CLASS_REQUIRE(HDS, boost::hdstl,FacetHDSConcept);
//..
// inside a class body, checks whether the type given as template
// argument is a model of the HDS concept described on the page [facethds].
//
//@CONCEPT: [facethds] Facet HDS concept
//
///Definition
///----------
// This concept refines the 'HDSConcept', and requires an additional facet
// descriptor to handle facets in the 'HDS' data structure. 
//
///Refinement of:
///--------------
//  - 'HDSConcept'
//
///Notation
///--------
//  - 'HDS'   A type that is a model of FacetHDSConcept
//  - 'hds'   A non-modifiable instance of HDS
//  - 'h'     Halfedge descriptor, of type 'hds_traits<HDS>::halfedge_descriptor'
//  - 'f'     Facet descriptor, of type 'hds_traits<HDS>::facet_descriptor'
//
///Associated types
///----------------
// In addition to the types required by the 'HDSConcept':
//  - 'hds_traits<HDS>::facet_descriptor': must be 'DefaultConstructible', 
//    'CopyConstructible', 'EqualityComparable', and 'Assignable'.
//
///Definitions
///-----------
//  - 'halfedge_descriptor' is a type that contains information to access 
//     the halfedge.  (See the 'HDSConcept' for a full definition.)
//  - 'facet_descriptor' is a type that contains information to access the
//     facet to the left of the halfedge, if forward facet cycles are
//     ordered in counter clockwise rotation around the facet.
//
///Valid Expressions
///-----------------
// In addition to the valid expressions of the 'HDSConcept':
//  - 'facet(hds,h)' must return a value assignable to 'f'.
//
///Expression Semantics
///--------------------
// In addition to the expressions semantics of the 'HDSConcept':
//  - 'hds_traits<HDS>::supports_facets': must compare equal to 'true'.
//  - 'facet(hds,h)' returns the facet descriptor of the facet to the
//     left of 'h', when forward facet cycles are oriented in
//     counter-clockwise order.
//
///Complexity guarantees
///---------------------
// In addition to the complexity guarantees of the 'HDSConcept':
//  - 'facet(hds,h)': amortized constant time.
//
///Invariants 
///----------
// 'FacetHDSConcept' should validate all the invariants for the 'HDSConcept'.
//
///Concept-checking class
///----------------------
//..
// template <class HDS> 
// struct FacetHDSConcept {
//    typedef typename hds_traits<HDS>::facet_descriptor facet_descriptor; 
//    void constraints() {
//       using namespace boost;
//       function_requires<HDSConcept>();
//       function_requires<DefaultConstructibleConcept<facet_descriptor> >();
//       function_requires<CopyConstructibleConcept<facet_descriptor> >();
//       function_requires<EqualityComparableConcept<facet_descriptor> >();
//       function_requires<AssignableConcept<facet_descriptor> >();
//       BOOST_STATIC_ASSERT(hds_traits<HDS>::supports_facets);
//       const_constraints(hds);
//    }
//    void const_constraints(HDS const& hds) {
//       f = facet(hds, h);
//    }
//    HDS hds;
//    facet_descriptor f;
// };
//..

#ifndef BOOST_HDSTL_CONCEPTS_FACET_HDS_CONCEPT_HPP
#define BOOST_HDSTL_CONCEPTS_FACET_HDS_CONCEPT_HPP 1

#include <boost/concepts.h>

namespace hdstl{
namespace concepts {

   template <class HDS> 
   struct FacetHDSConcept {
       // This class provides a concept checking class for the 
       // 'FacetHDSConcept', i.e., using the statement
       //..
       //  boost::function_requires<FacetHDSConcept<HDS>()
       //..
       // inside a function body, or
       //..
       //  BOOST_CLASS_REQUIRE(HDS, boost::hdstl, FacetHDSConcept);
       //..
       // inside a class definition body, should trigger a compile-time error
       // if the type HDS does not model the 'FacetHDSConcept'.

       // TYPES
       typedef typename hds_traits<HDS>::facet_descriptor facet_descriptor; 
       // The specialization of 'hds_traits<HDS>' must have these required
       // types, obeying the types requirements stated in the detailed
       // description of the 'FacetHDS' concept on page [facethdsconcept].

       //MANIPULATORS
       void constraints() 
           // Check that the 'HDS' template parameters satisfies all the
           // constraints of 'FacetHDSConcept' on page [facethdsconcept].
       {
           using namespace boost;
           function_requires<HDSConcept>();
           function_requires<DefaultConstructibleConcept<facet_descriptor> >();
           function_requires<CopyConstructibleConcept<facet_descriptor> >();
           function_requires<EqualityComparableConcept<facet_descriptor> >();
           function_requires<AssignableConcept<facet_descriptor> >();

	   BOOST_STATIC_ASSERT(hds_traits<HDS>::supports_facets);
           const_constraints(hds);
       }

       // ACCESSORS
       void const_constraints(HDS const& hds) 
           // Check that the non-modifiable 'HDS' template parameters
           // satisfies all the constraints of 'FacetHDSConcept'.
       {
	   f = facet(hds,h);
       }

     private:
       //DATA
       FacetHDS hds;         // a halfedge data structure object
       facet_descriptor f;   // a facet descriptor
   };

}  // close namespace concepts
}  // close namespace hdstl

#endif
