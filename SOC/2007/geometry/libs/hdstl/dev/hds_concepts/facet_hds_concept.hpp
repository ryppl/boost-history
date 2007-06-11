/**
 * @file facet_hds_concept.hpp
 *
 * @brief Facet HDS concept definition.
 *
 * @par Definition
 * This concept refines the HDSConcept, and specifies additional facet
 * descriptor to attach facets to the HDS. 
 *
 * @par Refinement of:
 *  - HDSConcept
 *
 * @par Notation
 *  - HDS        A type that is a model of FacetHDSConcept
 *  - hds        A non-modifiable instance of HDS
 *  - f          Facet descriptor, of type X::facet_descriptor
 *
 * @par Associated types
 *  - hds_traits<HDS>::facet_descriptor: must be DefaultConstructible and 
 *                         CopyConstructible, EqualityComparable, Assignable.
 *
 * @par Definitions
 * @e facet @e descriptor is a type that contains information to access
 * the facets attached to the halfedge data structure. 
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
 * struct FacetHDSConcept {
 *    typedef typename hds_traits<HDS>::facet_descriptor facet_descriptor; 
 *    void constraints() {
 *       using namespace boost;
 *       function_requires<HDSConcept>();
 *       function_requires<DefaultConstructibleConcept<facet_descriptor> >();
 *       function_requires<CopyConstructibleConcept<facet_descriptor> >();
 *       function_requires<EqualityComparableConcept<facet_descriptor> >();
 *       function_requires<AssignableConcept<facet_descriptor> >();
 *       const_constraints(hds);
 *    }
 *    void const_constraints(HDS const& hds) {
 *    }
 *    HDS hds;
 *    facet_descriptor f;
 * };
 * \endcode
 */

#ifdef BOOST_HDSTL_CONCEPTS_FACETHDSCONCEPT_HPP
#define BOOST_HDSTL_CONCEPTS_FACETHDSCONCEPT_HPP 1

namespace hdstl{
namespace concepts {

   /**
    * @class FacetHDSConcept
    *
    * @brief Facet HDS concept checking class
    *
    * @par Definition
    * This class provides a concept checking class for the FacetHDSConcept,
    * i.e., using the statement boost::function_requires<FacetHDSConcept<HDS>
    * >() inside a function body, or BOOST_CLASS_REQUIRE(HDS, boost::hdstl,
    * FacetHDSConcept) inside a class definition body, should trigger a
    * compile-time error if the type HDS does not model the FacetHDSConcept.
    */
   template <class HDS> 
   struct FacetHDSConcept {
      //@{
      /** @name Required types: 
       * The specialization of hds_traits<HDS> must have the required types
       * below, obeying the types requirements stated in the detailed
       * description.
       */
      typedef typename hds_traits<HDS>::facet_descriptor facet_descriptor; 
      //@}

      /**
       * @name Defined constraints:
       */
      void constraints() {
         using namespace boost;
         function_requires<HDSConcept>();
         function_requires<DefaultConstructibleConcept<facet_descriptor> >();
         function_requires<CopyConstructibleConcept<facet_descriptor> >();
         function_requires<EqualityComparableConcept<facet_descriptor> >();
         function_requires<AssignableConcept<facet_descriptor> >();

         const_constraints(hds);
      }

      /**
       * Defined const constraints:
       */
      void const_constraints(HDS const& hds) {
      }
      HDS hds;
      facet_descriptor f;
   };

}  // close namespace concepts
}  // close namespace hdstl

#endif
