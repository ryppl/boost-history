namespace hdstl{
namespace concepts{
   /**
    * @file vector_hds_concept.hpp
    *
    * @class VectorHDSConcept
    *
    * @brief Vertex HDS concept definition.
    * 
    * @par Definition
    * This class provides a concept checking for the VertexHDSConcept,
    * i.e., using the statement
    * boost::function_requires<VertexHDSConcept<HDS> >() inside a
    * function body, or BOOST_CLASS_REQUIRE(HDS, boost::hdstl,
    * VertexHDSConcept) inside a class definition body, should trigger a
    * compile-time error if the type HDS does not model the
    * VertexHDSConcept.
    *
    * @par Refinement of
    * @par HDSConcept
    *
    * @par Associated types
    *  - X::vertex_descriptor : Must be DefaultConstructible, 
    *    CopyConstuctible, EqualityComparable, Assignable
    *
    * @par Notation
    *  - X    A type that is a model of VertexHDSConcept
    *  - h    halfedge descriptors
    *  - v    vertex descriptor
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
   struct VertexHDSConcept {
   };
}} //namespaces
