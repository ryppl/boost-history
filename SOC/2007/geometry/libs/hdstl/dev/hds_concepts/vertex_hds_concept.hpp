/**
 * @file vertex_hds_concept.hpp
 *
 * @brief Vertex HDS concept definition.
 *
 * @par Definition
 * This concept refines the HDSConcept, and specifies additional vertex
 * descriptor to attach vertices to the HDS. 
 *
 * @par Refinement of:
 *  - HDSConcept
 *
 * @par Notation
 *  - HDS        A type that is a model of VertexHDSConcept
 *  - hds        A non-modifiable instance of HDS
 *  - v          Vertex descriptor, of type X::vertex_descriptor
 *
 * @par Associated types
 *  - hds_traits<HDS>::vertex_descriptor: must be DefaultConstructible and 
 *                         CopyConstructible, EqualityComparable, Assignable.
 *
 * @par Definitions
 * @e vertex @e descriptor is a type that contains information to access
 * the vertices attached to the halfedge data structure. 
 *
 * @par Valid Expressions
 *  None.
 *
 * @par Expression Semantics
 *  None.
 *
 * @par Complexity guarantees
 *  None.
 *
 * @par Invariants
 *  None.
 *
 * @par Concept-checking class
 * \code
 * template <class HDS> 
 * struct VertexHDSConcept {
 *    typedef typename hds_traits<HDS>::vertex_descriptor vertex_descriptor; 
 *    void constraints() {
 *       using namespace boost;
 *       function_requires<HDSConcept>();
 *       function_requires<DefaultConstructibleConcept<vertex_descriptor> >();
 *       function_requires<CopyConstructibleConcept<vertex_descriptor> >();
 *       function_requires<EqualityComparableConcept<vertex_descriptor> >();
 *       function_requires<AssignableConcept<vertex_descriptor> >();
 *       const_constraints(hds);
 *    }
 *    void const_constraints(HDS const& hds) {
 *    }
 *    HDS hds;
 *    vertex_descriptor v;
 * };
 * \endcode
 */

#ifdef BOOST_HDSTL_CONCEPTS_VERTEXHDSCONCEPT_HPP
#define BOOST_HDSTL_CONCEPTS_VERTEXHDSCONCEPT_HPP 1

namespace hdstl{
namespace concepts {

   /**
    * @class VertexHDSConcept
    *
    * @brief Vertex HDS concept checking class
    *
    * @par Definition
    * This class provides a concept checking class for the VertexHDSConcept,
    * i.e., using the statement boost::function_requires<VertexHDSConcept<HDS>
    * >() inside a function body, or BOOST_CLASS_REQUIRE(HDS, boost::hdstl,
    * VertexHDSConcept) inside a class definition body, should trigger a
    * compile-time error if the type HDS does not model the VertexHDSConcept.
    */
   template <class HDS> 
   struct VertexHDSConcept {
      //@{
      /** @name Required types: 
       * The specialization of hds_traits<HDS> must have the required types
       * below, obeying the types requirements stated in the detailed
       * description.
       */
      typedef typename hds_traits<HDS>::vertex_descriptor vertex_descriptor; 
      //@}

      /**
       * @name Defined constraints:
       */
      void constraints() {
         using namespace boost;
         function_requires<HDSConcept>();
         function_requires<DefaultConstructibleConcept<vertex_descriptor> >();
         function_requires<CopyConstructibleConcept<vertex_descriptor> >();
         function_requires<EqualityComparableConcept<vertex_descriptor> >();
         function_requires<AssignableConcept<vertex_descriptor> >();

         const_constraints(hds);
      }

      /**
       * Defined const constraints:
       */
      void const_constraints(HDS const& hds) {
      }
      HDS hds;
      vertex_descriptor v;
   };

}  // close namespace concepts
}  // close namespace hdstl

#endif
