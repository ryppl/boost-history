//mutable_facet_hds_concept.hpp    -*- C++ -*- 
//
//@PURPOSE: Provide 'MutableFacetHDS' concept definition, and concept checking class.
//
//@DESCRIPTION: This file provides a single class template that,
// when used with the following concept-checking utilities:
//..
//  boost::function_requires<MutableFacetHDSConcept<HDS> >();
//..
// inside a function body, or
//..
//  BOOST_CLASS_REQUIRE(HDS, boost::hdstl,MutableFacetHDSConcept);
//..
// inside a class body, checks whether the type given as template
// argument is a model of the HDS concept described on the page 
// [mutablefacethds].
//
//@CONCEPT: [mutablefacethds] Mutable Facet HDS concept
//
///Definition
///----------
// This concept refines the 'MutableHDSConcept' and 'FacetHDSConcept', 
// and specifies additional mutable operations to manipulate facet descriptors
// in the 'HDS' data structure. 
//
///Refinement of:
///--------------
//  - 'MutableHDSConcept'
//  - 'FacetHDSConcept'
//
///Notation
///--------
//  - 'HDS'   A type that is a model of MutableFacetHDSConcept
//  - 'hds'   A non-modifiable instance of HDS
//  - 'f'     Facet descriptor, of type 'hds_traits<HDS>::facet_descriptor'
//
///Associated types
///----------------
//  - 'hds_traits<HDS>::facet_descriptor': must be 'DefaultConstructible', 
//    'CopyConstructible', 'EqualityComparable', and 'Assignable'.
//
///Definitions
///-----------
//  - 'halfedge_descriptor' is a type that contains information to access 
//     the halfedge.  (See the 'HDSConcept' for a full definition.)
//  - 'facet_descriptor' is a type that contains information to access the
//     facets. (See the 'FacetHDSConcept' for a full definition.) 
//
///Valid Expressions
///-----------------
// In addition to the valid expressions of the 'FacetHDS' concept, and the 
// 'MutableHDS' concept:
//  - 'set_facet(hds,h,f)' sets the facet descriptor value of 'h' to 'f'.
//  - 'add_facet(hds,f)'   adds a new facet 'f' to the 'hds' data structure.
//  - 'remove_facet(hds,f)' removes the facet 'f' from the 'hds' data structure.
//
///Expression Semantics
///--------------------
// In addition to the expression semantics of the 'FacetHDS' concept, and the 
// 'MutableHDS' concept.
//  - 'set_facet(hds,h,f)' sets the facet descriptor value of 'h' to 'f' for a
//    single halfedge in the 'hds' data structure.
//  - 'add_facet(hds,f)'   adds a new facet 'f' to the 'hds' data structure.
//    By this operation the facet is added but no connections to the halfedges
//    are set. In order to assign facets to halfedges 'set_facet(hds,h,f)' 
//    operation should be used.
//  - 'remove_facet(hds,f)' removes the facet 'f' from the 'hds' data 
//    structure. All 'facet_descriptors' related to this facet will be 
//    invalidated after this call.
//
///Complexity guarantees
///---------------------
//  - 'set_facet(hds,h,f)' : amortized constant time.
//  - 'add_facet(hds,f)'   : amortized constant time.
//  - 'remove_facet(hds,f)': amortized constant time.
//
///Invariants 
///----------
// 'MutableFacetHDSConcept' should validate all the invariants for the 
// 'FacetHDSConcept' and the 'MutableHDSConcept'.
//
///Concept-checking class
///----------------------
//..
// template <class HDS> 
// struct MutableFacetHDSConcept {
//    typedef typename hds_traits<HDS>::facet_descriptor facet_descriptor; 
//    void constraints() {
//       using namespace boost;
//       function_requires<HDSConcept>();
//       function_requires<DefaultConstructibleConcept<facet_descriptor> >();
//       function_requires<CopyConstructibleConcept<facet_descriptor> >();
//       function_requires<EqualityComparableConcept<facet_descriptor> >();
//       function_requires<AssignableConcept<facet_descriptor> >();
//       set_facet(hds,h,f);
//       add_facet(hds,f);
//       delete_facet(hds,f);
//       const_constraints(hds);
//    }
//    void const_constraints(HDS const& hds) {
//       set_facet(hds,h,f);
//       add_facet(hds,f);
//       delete_facet(hds,f);
//    }
//    HDS hds;
//    halfedge_descriptor h;
//    facet_descriptor f;
// };
//..

#ifndef BOOST_HDSTL_CONCEPTS_MUTABLE_FACET_HDS_CONCEPT_HPP
#define BOOST_HDSTL_CONCEPTS_MUTABLE_FACET_HDS_CONCEPT_HPP 1

#include <boost/concepts.h>

namespace hdstl{
namespace concepts {

   template <class HDS> 
   struct MutableFacetHDSConcept {
       // This class provides a concept checking class for the 
       // 'MutableFacetHDSConcept', i.e., using the statement
       //..
       //  boost::function_requires<MutableFacetHDSConcept<HDS>()
       //..
       // inside a function body, or
       //..
       //  BOOST_CLASS_REQUIRE(HDS, boost::hdstl, MutableFacetHDSConcept);
       //..
       // inside a class definition body, should trigger a compile-time error
       // if the type HDS does not model the 'MutableFacetHDSConcept'.

       // TYPES
       typedef typename hds_traits<HDS>::facet_descriptor facet_descriptor; 
       // The specialization of 'hds_traits<HDS>' must have these required
       // types, obeying the types requirements stated in the detailed
       // description of the 'MutableFacetHDS' concept on page 
       // [mutablefacethdsconcept].

       //MANIPULATORS
       void constraints() 
           // Check that the 'HDS' template parameters satisfies all the
           // constraints of 'MutableFacetHDSConcept' on page 
           // [mutablefacethdsconcept].
       {
           using namespace boost;
           function_requires<HDSConcept>();
           function_requires<DefaultConstructibleConcept<facet_descriptor> >();
           function_requires<CopyConstructibleConcept<facet_descriptor> >();
           function_requires<EqualityComparableConcept<facet_descriptor> >();
           function_requires<AssignableConcept<facet_descriptor> >();

           set_facet(hds,h,f);
           add_facet(hds,f);
           delete_facet(hds,f);

           const_constraints(hds);
       }

       // ACCESSORS
       void const_constraints(HDS const& hds) 
           // Check that the non-modifiable 'HDS' template parameters
           // satisfies all the constraints of 'MutableFacetHDSConcept'.
       {
           set_facet(hds,h,f);
           add_facet(hds,f);
           delete_facet(hds,f);
       }

       private:
       //DATA
       MutableFacetHDS hds;     // a halfedge data structure object
       halfedge_descriptor h;   // a halfedge descriptor
       facet_descriptor f;      // a facet descriptor
   };

}  // close namespace concepts
}  // close namespace hdstl

#endif
