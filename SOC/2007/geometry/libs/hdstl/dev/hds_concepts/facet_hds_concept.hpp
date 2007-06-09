namespace hdstl{
namespace concepts{
   /**
    * @file facet_hds_concept.hpp
    *
    * @class FacetHDSConcept
    *
    * @brief Facet HDS concept definition.
    * 
    * @par Definition
    * This class provides a concept checking for the FacetHDSConcept,
    * i.e., using the statement
    * boost::function_requires<FacetHDSConcept<HDS> >() inside a
    * function body, or BOOST_CLASS_REQUIRE(HDS, boost::hdstl,
    * FacetHDSConcept) inside a class definition body, should trigger a
    * compile-time error if the type HDS does not model the
    * FacetHDSConcept.
    *
    * @par Refinement of
    * @par HDSConcept
    *
    * @par Associated types
    *  - X::facet_descriptor : Must be DefaultConstructible, 
    *    CopyConstuctible, EqualityComparable, Assignable
    *
    * @par Notation
    *  - X    A type that is a model of FacetHDSConcept
    *  - h    halfedge descriptors
    *  - f    facet descriptor
    *
    * @par Definitions
    * @e halfedge @e descriptor is a pointer to access the halfedge
    *
    * @par Valid Expressions
    *
    * @par Expression Semantics
    *
    * @par Complexity guarantees
    *
    * @par Invariants 
    */
   template <class HDS> 
   struct FacetHDSConcept {
   };
}} //namespaces
