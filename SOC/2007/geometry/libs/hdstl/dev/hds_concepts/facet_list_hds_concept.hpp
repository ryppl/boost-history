//facet_list_hds_concept.hpp    -*- C++ -*- 
//
//@PURPOSE: Provide 'FacetListHDS' concept definition, and concept checking class.
//
//@DESCRIPTION: This file provides a single class template that,
// when used with the following concept-checking utilities:
//..
//  boost::function_requires<FacetListHDSConcept<HDS> >();
//..
// inside a function body, or
//..
//  BOOST_CLASS_REQUIRE(HDS, boost::hdstl,FacetListHDSConcept);
//..
// inside a class body, checks whether the type given as template
// argument is a model of the HDS concept described on the page 
// [facetlisthds].
//
//@CONCEPT: [facetlisthds] Facet List HDS concept
//
///Definition
///----------
// This concept refines the 'HDSConcept', and specifies additional accessors
// to handle facets in the 'HDS' data structure. 
//
///Refinement of:
///--------------
//  - 'HDSConcept'
//
///Notation
///--------
//  - 'HDS'  A type that is a model of FacetListHDSConcept
//  - 'hds'  A non-modifiable instance of HDS
//  - 'f'    Facet descriptor, of type 'hds_traits<HDS>::facet_descriptor'
//  - 'p'    Facet iterator pair, of type 
//           'std::pair<facet_iterator,facet_iterator>'
//  - 'F'    Facet size type, of type 'hds_traits<HDS>::facet_size_type'
//
///Associated types
///----------------
//  - 'hds_traits<HDS>::facet_descriptor': must be 'DefaultConstructible', 
//    'CopyConstructible', 'EqualityComparable', and 'Assignable'.
//  - 'hds_traits<HDS>::facet_iterator': must be 'MultiPassInputIterator'.
//  - 'hds_traits<HDS>::facet_size_type': facet size type.
//
///Definitions
///-----------
//  - 'facet_descriptor' is a type that contains information to access 
//     the facet.  (See the 'HDSConcept' for a full definition.)
//  - 'facet_iterator' is an iterator type for the facets.
//  - 'facet_size_type' defines the size type.
//
///Valid Expressions
///-----------------
// In addition to the valid expressions of the 'HDS' concept:
//  - 'facets(hds)' must return a value assignable to 'p'.  
//  - '*p.first' must return a value assignable to 'f'.
//  - 'num_facets(HDS)' must return a value assignable to 'F'.
//
///Expression Semantics
///--------------------
// In addition to the valid expression semantics of the 'HDS' concept:
//  - 'facets(hds)' returns a 'facet_iterator' pair 'p', consistent 
//    with the STL style iterator ranges, which means all the facets
//    can be accessed by increasing the first part of the pair until the second
//    part is reached.
//  - '*p.first' returns a 'facet_descriptor'.
//  - 'num_facets(HDS)' returns the number of facets in the 'HDS' data
//    structure.
//
///Complexity guarantees
///---------------------
//  - 'facets(hds)'    : amortized constant time. (for just getting the 
//    'facet_iterator' pair, accessing the facets takes linear time.)
//  - '*p.first'       : amortized constant time.
//  - 'num_facets(HDS)': amortized constant time.
//
///Invariants 
///----------
// 'FacetListHDSConcept' should validate all the invariants for the 
// 'HDSConcept'.
//
///Concept-checking class
///----------------------
//..
// template <class HDS> 
// struct FacetListHDSConcept {
//     typedef typename hds_traits<HDS>::facet_iterator  facet_iterator;
//     typedef typename hds_traits<HDS>::facet_descriptor facet_descriptor;
//     typedef typename hds_traits<HDS>::facets_size_type facets_size_type;
//     void constraints() 
//     {
//         using namespace boost;
//         function_requires<HDSConcept>();
//         function_requires<FacetHDSConcept>();
//         function_requires<hdstl_detail::MultiPassInputIteratorConcept<
//                                                         facet_iterator> >();
//         p = facets(hds);
//         f = *p.first;
//         F = num_facets(hds);
//         const_constraints(hds);
//     }
//     void const_constraints(HDS const& hds) 
//     {
//         p = facets(hds);
//         f = *p.first;
//         F = num_facets(hds);
//     }
//     private:
//     FacetListHDS hds;   
//     facet_descriptor f;
//     std::pair<facet_iterator,facet_iterator> p;
//     facet_size_type F; 

// };
//..

#ifndef BOOST_HDSTL_CONCEPTS_FACET_LIST_HDS_CONCEPT_HPP
#define BOOST_HDSTL_CONCEPTS_FACET_LIST_HDS_CONCEPT_HPP 1

#include <boost/concepts.h>

namespace hdstl{
namespace concepts {

   template <class HDS> 
   struct FacetListHDSConcept {
       // This class provides a concept checking class for the 
       // 'FacetListHDSConcept', i.e., using the statement
       //..
       //  boost::function_requires<FacetListHDSConcept<HDS>()
       //..
       // inside a function body, or
       //..
       //  BOOST_CLASS_REQUIRE(HDS, boost::hdstl, FacetListHDSConcept);
       //..
       // inside a class definition body, should trigger a compile-time error
       // if the type HDS does not model the 'FacetListHDSConcept'.

       // TYPES
       typedef typename hds_traits<HDS>::facet_iterator  facet_iterator;
       typedef typename hds_traits<HDS>::facet_descriptor facet_descriptor;
       typedef typename hds_traits<HDS>::facets_size_type facets_size_type;
       // The specialization of 'hds_traits<HDS>' must have these required
       // types, obeying the types requirements stated in the detailed
       // description of the 'FacetListHDS' concept on page 
       // [facetlisthdsconcept].

       //MANIPULATORS
       void constraints() 
           // Check that the 'HDS' template parameters satisfies all the
           // constraints of 'FacetListHDSConcept' on page 
           // [facetlisthdsconcept].
       {
           using namespace boost;
           function_requires<HDSConcept>();
           function_requires<FacetHDSConcept>();
           function_requires<hdstl_detail::MultiPassInputIteratorConcept<
                                                           facet_iterator> >();
           p = facets(hds);
           f = *p.first;
           F = num_facets(hds);
           const_constraints(hds);
       }

       // ACCESSORS
       void const_constraints(HDS const& hds) 
           // Check that the non-modifiable 'HDS' template parameters
           // satisfies all the constraints of 'FacetListHDSConcept'.
       {
           p = facets(hds);
           f = *p.first;
           F = num_facets(hds);
       }

       private:
       //DATA
       FacetListHDS hds;     // a halfedge data structure object
       facet_descriptor f;   // a facet descriptor
       std::pair<facet_iterator,facet_iterator> p; // a facet iterator pair
       facet_size_type F;    // facet size type

   };

}  // close namespace concepts
}  // close namespace hdstl

#endif
