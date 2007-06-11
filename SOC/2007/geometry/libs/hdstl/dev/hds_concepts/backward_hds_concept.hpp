/**
 * @file backward_hds_concept.hpp
 *
 * @brief Backward HDS concept definition.
 *
 * @par Definition
 * This concept refines the HDSConcept, and specifies additional backward
 * category to iterate over the HDS data structure, which means clockwise 
 * movement around the facet and counter clockwise movement around the
 * vertex. Possible backward categories are prev_in_facet, 
 * prev_at_source, and prev_at_target. Once one of these categories 
 * are given all others can be accessed with the help of the opposite of a 
 * halfedge.
 *
 * @par Refinement of
 *  - HDSConcept
 *
 * @par Notation
 *  - HDS        A type that is a model of BackwardHDSConcept
 *  - hds        A non-modifiable instance of HDS
 *  - h,g        Halfedge descriptors, of type X::halfedge_descriptor
 *
 * @par Associated types
 *  - hds_traits<HDS>::halfedge_descriptor: must be DefaultConstructible and 
 *                         CopyConstructible, EqualityComparable, Assignable.
 *  - hds_traits<HDS>::traversal_category:  must be convertible to 
 *                           hds_traits<HDS>::backward_traversal_tag.
 *  - hds_traits<HDS>::backward_category:  Defines the pointer type for 
 *                                                   backward iteration.
 *
 * @par Definitions
 *  - @e halfedge @e descriptor is a type that contains information to access 
 *                                                               the halfedge.
 *  - @e traversal @e category is a type that contains information about the 
 *    direction of the traversal, which is forward, backward or bidirectional.
 *    For backward hds concept, it has to be convertible to  
 *    hds_traits<HDS>::backward_traversal_tag.
 *  - @e backward @e category defines the type of the backward iteration. 
 *
 * @par Valid Expressions
 *   - prev_in_facet(hds,h) : Must return a value assignable to h.
 *   - prev_at_source(hds,h) : Must return a value assignable to h.
 *   - prev_at_target(hds,h) : Must return a value assignable to h.
 *
 * @par Expression Semantics
 *   - prev_in_facet(hds,h) : returns an halfedge descriptor to next halfedge 
 *                                in clockwise order around the facet.
 *   - prev_at_source(hds,h) : returns an halfedge descriptor to next halfedge 
 *                          in counter clockwise order around the source vertex.
 *   - prev_at_target(hds,h) : returns an halfedge descriptor to next halfedge 
 *                          in counter clockwise order around the target vertex.
 *
 * @par Complexity guarantees
 *  - prev_in_facet(hds,h) : Amortized constant time
 *  - prev_at_source(hds,h) : Amortized constant time
 *  - prev_at_target(hds,h) : Amortized constant time
 *
 * @par Invariants 
 *   - prev_in_facet(hds,h) == prev_at_target(opposite(hds,h)) 
 *                          == opposite(prev_at_source(hds,h))
 *   - prev_at_source(hds,h) == opposite(prev_in_facet(hds,h)) 
 *                           == opposite(prev_at_target(opposite(hds,h)))
 *   - prev_at_target(hds,h) == prev_in_facet(opposite(hds,h)) 
 *                           == opposite(prev_at_source(opposite(hds,h)))
 *
 * @par Concept-checking class
 * \code
 * template <class HDS> 
 * struct BackwardHDSConcept {
 *   typedef typename hds_traits<HDS>::halfedge_descriptor halfedge_descriptor; 
 *   typedef typename hds_traits<HDS>::traversal_category traversal_category;
 *   typedef typename hds_traits<HDS>::backward_category backward_category;
 *    void constraints() {
 *       using namespace boost;
 *       function_requires<HDSConcept<HDS> >();
 *       function_requires<ConvertibleConcept<traversal_category,
 *                                                backward_traversal_tag> >();  
 *       const bool is_valid_storage_tag =
 *          is_convertible<backward_category,prev_at_source_tag>::value ||
 *          is_convertible<backward_category,prev_at_target_tag>::value ||
 *          is_convertible<backward_category,prev_in_facet_tag>::value;
 *       BOOST_STATIC_ASSERT( is_valid_storage_tag ); 
 *       // Error message could be improved
 *       h = prev_in_facet(hds,h);
 *       h = prev_at_source(hds,h);
 *       h = prev_at_target(hds,h);
 *       const_constraints(hds);
 *    }
 *    void const_constraints(HDS const& hds){
 *       h = prev_in_facet(hds,h);
 *       h = prev_at_source(hds,h);
 *       h = prev_at_target(hds,h);
 *    }
 *    BackwardHDS hds;
 *    halfedge_descriptor h;
 *    halfedge_descriptor g;
 * };
 * \endcode
 */

#ifdef BOOST_HDSTL_CONCEPTS_BACKWARDHDSCONCEPT_HPP
#define BOOST_HDSTL_CONCEPTS_BACKWARDHDSCONCEPT_HPP 1

namespace hdstl{
namespace concepts{
   /**
    * @class BackwardHDSConcept
    *
    * @brief Backward HDS concept checking class.
    * 
    * @par Definition
    * This class provides a concept checking class for the BackwardHDSConcept,
    * i.e., using the statement boost::function_requires<BackwardHDSConcept<HDS>
    * >() inside a function body, or BOOST_CLASS_REQUIRE(HDS, boost::hdstl,
    * BackwardHDSConcept) inside a class definition body, should trigger a
    * compile-time error if the type HDS does not model the BackwardHDSConcept.
    */
   template <class HDS> 
   struct BackwardHDSConcept {
      //@{
      /** @name Required types: 
       * The specialization of hds_traits<HDS> must have
       * the required types below
       */
      typedef typename hds_traits<HDS>::halfedge_descriptor halfedge_descriptor; 
      typedef typename hds_traits<HDS>::traversal_category traversal_category;
      typedef typename hds_traits<HDS>::backward_category backward_category;
      //@}

      /**
       * @name Defined constraints:
       */
      void constraints() {
         using namespace boost;

         function_requires<HDSConcept<HDS> >();
         function_requires<ConvertibleConcept<traversal_category,
                                                  backward_traversal_tag> >();  
         const bool is_valid_storage_tag =
            is_convertible<backward_category,prev_at_source_tag>::value ||
            is_convertible<backward_category,prev_at_target_tag>::value ||
            is_convertible<backward_category,prev_in_facet_tag>::value;
         BOOST_STATIC_ASSERT( is_valid_storage_tag ); 
         // Error message could be improved

         h = prev_in_facet(hds,h);
         h = prev_at_source(hds,h);
         h = prev_at_target(hds,h);
         const_constraints(hds);
      }
      /**
       * Defined const constraints
       */
      void const_constraints(HDS const& hds){
         h = prev_in_facet(hds,h);
         h = prev_at_source(hds,h);
         h = prev_at_target(hds,h);
      }
      
      BackwardHDS hds;
      halfedge_descriptor h;
      halfedge_descriptor g;
   };
}  // close namespace concepts
}  // close namespace hdstl

#endif
