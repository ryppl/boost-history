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
// to iterate over facets in the 'HDS' data structure. 
//
///Refinement of:
///--------------
//  - 'FacetHDSConcept'
//
///Notation
///--------
//  - 'HDS'  A type that is a model of FacetListHDSConcept
//  - 'hds'  A non-modifiable instance of HDS
//  - 'f'    Facet descriptor, of type 'hds_traits<HDS>::facet_descriptor'
//  - 'p'    Facet iterator, of type 'hds_traits<HDS>::facet_iterator'
//  - 'n'    Facet list size, of type 'hds_traits<HDS>::size_type'
//
///Associated types
///----------------
// In addition to the types of the 'FacetHDS' concept:
//  - 'hds_traits<HDS>::facet_iterator': must be a model of the
//    'ForwardIterator' concept.  The value type of the facet iterator
//    must be the same as the 'facet_descriptor'.
//  - 'hds_traits<HDS>::size_type': integral size type.
//
///Definitions
///-----------
//  - 'facet_descriptor' is a type that contains information to access 
//     the facet.  (See the 'FacetHDSConcept' for a full definition.)
//  - 'facet_iterator' is an iterator type for the facets.
//  - 'size_type' defines the size type.
//
///Valid Expressions
///-----------------
// In addition to the valid expressions of the 'HDS' concept:
//  - 'facets_begin(hds)' must return a value assignable to 'p'.  
//  - 'facets_end(hds)' must return a value assignable to 'p'.  
//  - '*p.first' must return a value assignable to 'f'.
//  - 'num_facets(hds)' must return a value assignable to 'n'.
//
///Expression Semantics
///--------------------
// In addition to the valid expression semantics of the 'HDS' concept:
//  - 'facets_begin(hds)' returns a 'facet_iterator' 'p' pointing to the 
//    beginning of the "facet list".
//  - 'facets_end(hds)' returns a 'facet_iterator' 'p' pointing to the 
//    end of the "facet list".
//  - 'num_facets(HDS)' returns the number of facets in the 'HDS' data
//    structure.
//
///Complexity guarantees
///---------------------
// In addition to the complexity guarantees of the 'FacetHDS' concept:
//  - 'facets_begin(hds)': amortized constant time.  
//  - 'facets_end(hds)': amortized constant time.  
//  - 'num_facets(hds)': amortized constant time.
//
///Invariants 
///----------
//  - 'std::distance(facets_begin(hds), facets_end(hds)) ==  num_facets(hds)'
//
///Concept-checking class
///----------------------
//..
// template <class HDS> 
// struct FacetListHDSConcept {
//     typedef typename hds_traits<HDS>::facet_iterator  facet_iterator;
//     typedef typename hds_traits<HDS>::facet_descriptor facet_descriptor;
//     typedef typename hds_traits<HDS>::size_type size_type;
//     void constraints() 
//     {
//         using namespace boost;
//         function_requires<HDSConcept>();
//         function_requires<FacetHDSConcept>();
//         function_requires<ForwardIteratorConcept<facet_iterator> >();
//         function_requires<ConvertibleConcept<size_type,int> >();
//         const_constraints(hds);
//     }
//     void const_constraints(HDS const& hds) 
//     {
//         b = facets_begin(hds);
//         e = facets_end(hds);
//         n = num_facets(hds);
//     }
//     private:
//     FacetListHDS hds;   
//     facet_descriptor f;
//     facet_iterator b;
//     facet_iterator e;
//     size_type n; 

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
       typedef typename hds_traits<HDS>::size_type size_type;
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
           function_requires<hdstl_detail::ForwardIteratorConcept<
                                                           facet_iterator> >();
           function_requires<ConvertibleConcept<size_type,int> >();
           b = facets_begin(hds);
           e = facets_end(hds);
           n = num_facets(hds);
           const_constraints(hds);
       }

       // ACCESSORS
       void const_constraints(HDS const& hds) 
           // Check that the non-modifiable 'HDS' template parameters
           // satisfies all the constraints of 'FacetListHDSConcept'.
       {
           b = facets_begin(hds);
           e = facets_end(hds);
           n = num_facets(hds);
       }

     private:
       //DATA
       FacetListHDS hds;     // a halfedge data structure object
       facet_descriptor f;   // a facet descriptor
       facet_iterator b;     // a facet iterator
       facet_iterator e;     // a facet iterator 
       size_type n;          // facet size type

   };

}  // close namespace concepts
}  // close namespace hdstl

#endif
