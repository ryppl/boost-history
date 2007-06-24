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
//  - 'HDS'      A type that is a model of 'MutableHDSConcept'
//  - 'hds'      A modifiable instance of 'HDS'
//  - 'h', h1','h2'
//               Halfedge descriptors, of type 'hds_traits<HDS>::halfedge_descriptor'
//
///Associated types
///----------------
//  - 'hds_traits<HDS>::halfedge_descriptor': must be 'DefaultConstructible', 
//    'CopyConstructible', 'EqualityComparable', and 'Assignable'.
//
///Definitions
///-----------
// 'halfedge descriptor' is a type that contains information to access
// the halfedge.  Note that the halfedge descriptor type (accessible as a
// member of the 'hds_traits<HDS>' type) is an opaque type.  All that can be
// done to it is passing it to the function opposite and letting the hds use
// the information it contains to find the opposite halfedge.  In
// particular, there is no guarantee that it is a pointer type.
//
///Valid Expressions
///-----------------
// In addition to the valid expressions of the 'HDS' concept,
// the following expressions must be valid and obey the syntactic requirement:
//  - 'set_opposite(hds,h1,h2)'
//  - 'h = new_edge(hds)'
//  - 'delete_edge(hds,h)'
//
///Expression Semantics
///--------------------
// The expressions semantics are as follows:
//  - 'set_opposite(hds,h1,h2)' sets 'h1' and 'h2' as opposites of each other 
//    in the data structure 'hds'. 
//  - 'new_edge(hds)' creates a new edge in data structure 'hds', and
//    returns the halfedge descriptor of one of the two halfedges created. 
//    The other new halfedge can be obtained using 'opposite(hds,h)'.
//    Note that halfedges can only be created in opposite pairs.
//  - 'delete_edge(hds,h)' removes the halfedges 'h' and 'opposite(hds,h)'
//    from the data structure 'hds'. Note that only the halfedges that form 
//    a pair by validating the 'opposite' function can be deleted by this 
//    method.
//
///Complexity guarantees
///---------------------
//  - 'set_opposite(hds,h1,h2)': amortized constant time.
//  - 'new_edge(hds)': amortized constant time.
//  - 'delete_edge(hds,h)' : amortized constant time.
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
//       set_opposite(hds,h1,h2);
//       h = new_edge(hds);
//       delete_edge(hds,h);
//       const_constraints(hds);
//    }
//    void const_constraints(HDS const& hds) {
//    }
//    HDS hds;
//    halfedge_descriptor h1;
//    halfedge_descriptor h2;
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

            set_opposite(hds,h1,h2);
            h = new_edge(hds);
            delete_edge(hds,h);
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
            HDS hds;                   // a halfedge data structure object
            halfedge_descriptor h;     // a halfedge descriptor
            halfedge_descriptor h1;    // a halfedge descriptor
            halfedge_descriptor h2;    // a halfedge descriptor
   };

}  // close namespace concepts
}  // close namespace hdstl

#endif
