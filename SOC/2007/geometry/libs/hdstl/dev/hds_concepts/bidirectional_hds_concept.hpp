/**
 * @file bidirectional_hds_concept.hpp
 *
 * @brief Bidirectional HDS concept definition.
 *
 * @par Definition
 * This concept refines the ForwardHDSConcept and BackwardHDSConcept, and
 * specifies additional bidirectional traversal category. Bidirectional
 * category requires both the forward_category and the backward_category
 * defined to iterate over the HDS data structure, which means both clockwise &
 * counter clockwise movement around the facet and vertex. From these two
 * categories all others can be accessed with the help of the opposite of a
 * halfedge.
 *
 * @par Refinement of
 *  - ForwardHDSConcept
 *  - BackwardHDSConcept
 *
 * @par Notation
 *  - HDS        A type that is a model of BidirectionalHDSConcept
 *  - hds        A non-modifiable instance of HDS
 *  - h,g        Halfedge descriptors, of type X::halfedge_descriptor
 *
 * @par Associated types
 *  - hds_traits<HDS>::halfedge_descriptor: must be DefaultConstructible and 
 *                         CopyConstructible, EqualityComparable, Assignable.
 *  - hds_traits<HDS>::traversal_category:  must be convertible to 
 *                           hds_traits<HDS>::bidirectional_traversal_tag.
 *
 * @par Definitions
 *  - @e halfedge @e descriptor is a type that contains information to access 
 *                                                               the halfedge.
 *  - @e traversal @e category is a type that contains information about the 
 *    direction of the traversal, which is forward, backward or bidirectional.
 *    For bidirectional hds concept, it has to be convertible to  
 *    hds_traits<HDS>::bidirectional_traversal_tag.
 *
 * @par Valid Expressions
 *  - see ForwardHDSConcept and BackwardHDSConcept.
 *
 * @par Expression Semantics
 *  - see ForwardHDSConcept and BackwardHDSConcept.
 *
 * @par Complexity guarantees
 *  - see ForwardHDSConcept and BackwardHDSConcept.
 *
 * @par Invariants 
 * The invariants for ForwardHDSConcept, and BackwardHDSConcept should hold
 * for BidirectionalHDSConcept also.
 *
 * @par Concept-checking class
 * \code
 * template <class HDS> 
 * struct BidirectionalHDSConcept {
 *   typedef typename hds_traits<HDS>::halfedge_descriptor halfedge_descriptor; 
 *   typedef typename hds_traits<HDS>::traversal_category traversal_category;
 *    void constraints() {
 *       using namespace boost;
 *       function_requires<ForwardHDSConcept<HDS> >();
 *       function_requires<BackwardHDSConcept<HDS> >();
 *       function_requires<ConvertibleConcept<traversal_category,
 *                                          bidirectional_traversal_tag> >();  
 *    }
 *    BidirectionalHDS hds;
 * };
 * \endcode
 */

#ifdef BOOST_HDSTL_CONCEPTS_BIDIRECTIONALHDSCONCEPT_HPP
#define BOOST_HDSTL_CONCEPTS_BIDIRECTIONALHDSCONCEPT_HPP 1

namespace hdstl{
namespace concepts{
   /**
    * @class BidirectionalHDSConcept
    *
    * @brief Bidirectional HDS concept checking class.
    * 
    * @par Definition
    * This class provides a concept checking class for the
    * BidirectionalHDSConcept, i.e., using the statement
    * boost::function_requires<BidirectionalHDSConcept<HDS> >() inside a
    * function body, or BOOST_CLASS_REQUIRE(HDS, boost::hdstl,
    * BidirectionalHDSConcept) inside a class definition body, should trigger a
    * compile-time error if the type HDS does not model the
    * BidirectionalHDSConcept.
    */
   template <class HDS> 
   struct BidirectionalHDSConcept {
      //@{
      /** @name Required types: 
       * The specialization of hds_traits<HDS> must have
       * the required types below
       */
      typedef typename hds_traits<HDS>::halfedge_descriptor halfedge_descriptor; 
      typedef typename hds_traits<HDS>::traversal_category traversal_category;
      //@}

      /**
       * @name Defined constraints:
       */
      void constraints() {
         using namespace boost;

         function_requires<ForwardHDSConcept<HDS> >();
         function_requires<BackwardHDSConcept<HDS> >();
         function_requires<ConvertibleConcept<traversal_category,
                                            bidirectional_traversal_tag> >();  
      }
      BidirectionalHDS hds;
   };
}  // close namespace concepts
}  // close namespace hdstl

#endif
