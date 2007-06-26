//mutable_hds_concept.hpp    -*- C++ -*-   
//
//@PURPOSE: Provide 'MutableHDS' concept definition and concept-checking class.
//
//@DESCRIPTION: This file provides a single class template that,
// when used with the following concept-checking utilities:
//..
//  boost::function_requires<MutableHDSConcept<HDS> >();
//..
// inside a function body, or
//..
//  BOOST_CLASS_REQUIRE(HDS, boost::hdstl, MutableHDSConcept);
//..
// inside a class body, checks whether the type given as template
// argument is a model of the MutableHDS concept described on the page
// [mutablehdsconcept].
//
//@CONCEPT: [mutablehdsconcept] Mutable HDS concept definition
//
///Definition
///----------
// This concept is the basic 'MutableHDSConcept', which all the other 
// 'Mutable HDS concepts' refine.  It contains a few requirements which are
// common to all the mutable graph concepts.  One should note that a model of
// 'MutableHDSConcept' is not required to be a model of 'Assignable', so
// algorithms should pass 'MutableHDS' objects by reference.
//
///Refinement of:
///--------------
//  - HDSConcept
//
///Notation
///--------
//  - 'HDS'  A type that is a model of 'MutableHDSConcept'
//  - 'hds'  A modifiable instance of 'HDS'
//  - 'h'    Halfedge descriptors, of type 'hds_traits<HDS>::halfedge_descriptor'
//
///Associated types
///----------------
// None besides the types of the 'HDS' concept,
//
///Valid Expressions
///-----------------
// In addition to the valid expressions of the 'HDS' concept:
//  - 'h = new_edge(hds)'
//  - 'delete_edge(h, hds)'
//
///Expression Semantics
///--------------------
// In addition to the expression semantics of the 'HDS' concept:
//  - 'new_edge(hds)' creates a pair of opposite halfedges in 'hds', and
//    returns the halfedge descriptor of one of the two halfedges created. 
//    The other new halfedge can be obtained using 'opposite(h, hds)'.
//    Note that halfedges can only be created in opposite pairs.
//  - 'delete_edge(h, hds)' removes the halfedges 'h' and 'opposite(h, hds)'
//    from the data structure 'hds'. Note that only the halfedges that form 
//    a pair by validating the 'opposite' function can be deleted by this 
//    method.
//
///Complexity guarantees
///---------------------
// In addition to the complexity guarantees of the 'HDS' concept:
//  - 'new_edge(hds)': amortized constant time.
//  - 'delete_edge(h, hds)' : amortized constant time.
//
///Invariants
///----------
// None.
//
///Concept-checking class
///----------------------
//..
// template <class HDS> 
// struct MutableHDSConcept {
//   typedef typename hds_traits<HDS>::halfedge_descriptor halfedge_descriptor; 
//    void constraints() {
//       using namespace boost;
//       function_requires<HDSConcept<HDS> >();
//
//       h = new_edge(hds);
//       delete_edge(h, hds);
//       const_constraints(hds);
//    }
//    void const_constraints(HDS const& hds) {
//    }
//    HDS hds;
//    halfedge_descriptor h;
// };
//.. 

#ifndef BOOST_HDSTL_CONCEPTS_MUTABLE_HDS_CONCEPT_HPP
#define BOOST_HDSTL_CONCEPTS_MUTABLE_HDS_CONCEPT_HPP 1

#include <boost/concepts.h>

namespace hdstl {
namespace concepts {

    template <class HDS> 
    struct MutableHDSConcept {
        // This class provides a concept checking class for the
        // 'MutableHDS' concept defined in [mutablehdsconcept], i.e., using 
        // the statement
        //..
        // boost::function_requires<MutableHDSConcept<HDS> >();
        //..
        // inside a function body, or
        //..
        // BOOST_CLASS_REQUIRE(HDS, boost::hdstl, MutableHDSConcept);
        //..
        // inside a class definition body, should trigger a compile-time
        // error if the type 'HDS' does not model the 'MutableHDS' concept.

        // TYPES
        typedef typename hds_traits<HDS>::halfedge_descriptor 
                                          halfedge_descriptor; 
            // The specialization of 'hds_traits<HDS>' must have these
            // required types, obeying the types requirements stated in the
            // detailed description of the 'MutableHDS' concept on page
            // [mutablehdsconcept].

        // MANIPULATORS
        void constraints() 
            // Check that the 'HDS' template parameters satisfies all the
            // constraints of 'MutableHDSConcept' on page [mutablehdsconcept].
        {
            using namespace boost;
            function_requires<HDSConcept<HDS> >();

            h = new_edge(hds);
            delete_edge(h, hds);
            const_constraints(hds);
        }

        // ACCESSORS
        void const_constraints(HDS const& hds) 
            // Check that the non-modifiable 'HDS' template parameters
            // satisfies all the constraints of 'MutableHDSConcept'.
        {
        }

        private:
        // DATA
            HDS hds;                // a halfedge data structure object
            halfedge_descriptor h;  // a halfedge descriptor
   };

}  // close namespace concepts
}  // close namespace hdstl

#endif
