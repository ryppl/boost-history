namespace hdstl{
namespace concepts{
   /**
    * @file bidirectional_hds_concept.hpp
    *
    * @class BidirectionalHDSConcept
    *
    * @brief Bidirectional HDS concept definition.
    * 
    * @par Definition
    * This class provides a concept checking for the BidirectionalHDSConcept,
    * i.e., using the statement
    * boost::function_requires<BidirectionalHDSConcept<HDS> >() inside a
    * function body, or BOOST_CLASS_REQUIRE(HDS, boost::hdstl,
    * BidirectionalHDSConcept) inside a class definition body, should trigger a
    * compile-time error if the type HDS does not model the
    * BidirectionalHDSConcept.
    *
    * @par Refinement of
    * @par ForwardHDSConcept, @par BackwardHDSConcept
    *
    * @par Associated types
    *
    * @par Notation
    *  - X    A type that is a model of BidirectionalHDSConcept
    *  - h,g  halfedge descriptors
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
   struct BidirectionalHDSConcept {
   };
}} //namespaces
