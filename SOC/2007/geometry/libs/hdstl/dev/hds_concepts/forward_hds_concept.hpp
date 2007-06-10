/**
 * @file forward_hds_concept.hpp
 *
 * @brief Forward HDS concept definition.
 *
 * @par Definition
 * This concept refines the HDSConcept, and specifies additional forward
 * category to iterate over the HDS data structure, which means counter
 * clockwise movement around the facet and clockwise movement around the
 * vertex. Possible forward categories are next_in_facet, 
 * next_at_source, and next_at_target. Once one of these categories 
 * are given all others can be accessed with the help of the opposite of a 
 * halfedge.
 *
 * @par Refinement of
 *  - HDSConcept
 *
 * @par Notation
 *  - HDS        A type that is a model of ForwardHDSConcept
 *  - hds        A non-modifiable instance of HDS
 *  - h,g        Halfedge descriptors, of type X::halfedge_descriptor
 *
 * @par Associated types
 *  - hds_traits<HDS>::halfedge_descriptor: must be DefaultConstructible and 
 *                         CopyConstructible, EqualityComparable, Assignable.
 *  - hds_traits<HDS>::traversal_category:  must be convertible to 
 *                           hds_traits<HDS>::forward_traversal_tag.
 *  - hds_traits<HDS>::forward_category:  Defines the pointer type for forward 
 *                                                                   iteration.
 *
 * @par Definitions
 *  - @e halfedge @e descriptor is a type that contains information to access 
 *                                                               the halfedge.
 *  - @e traversal @e category is a type that contains information about the 
 *    direction of the traversal, which is forward, backward or bidirectional.
 *    For forward hds concept, it has to be convertible to  
 *    hds_traits<HDS>::forward_traversal_tag.
 *  - @e forward @e category defines the type of the forward iteration. 
 *
 * @par Valid Expressions
 *   - next_in_facet(hds,h) : Must return a value assignable to h.
 *   - next_at_source(hds,h) : Must return a value assignable to h.
 *   - next_at_target(hds,h) : Must return a value assignable to h.
 *
 * @par Expression Semantics
 *   - next_in_facet(hds,h) : returns an halfedge descriptor to next halfedge 
 *                                in counter clockwise order around the facet.
 *   - next_at_source(hds,h) : returns an halfedge descriptor to next halfedge 
 *                                in clockwise order around the source vertex.
 *   - next_at_target(hds,h) : returns an halfedge descriptor to next halfedge 
 *                                in clockwise order around the target vertex.
 *
 * @par Complexity guarantees
 *  - next_in_facet(hds,h) : Amortized constant time
 *  - next_at_source(hds,h) : Amortized constant time
 *  - next_at_target(hds,h) : Amortized constant time
 *
 * @par Invariants 
 *   - next_in_facet(hds,h) == next_at_source(opposite(hds,h)) 
 *                          == opposite(next_at_target(hds,h))
 *   - next_at_source(hds,h) == next_in_facet(opposite(hds,h)) 
 *                           == opposite(next_at_target(opposite(hds,h)))
 *   - next_at_target(hds,h) == opposite(next_in_facet(hds,h)) 
 *                           == opposite(next_at_source(opposite(hds,h)))
 *
 * @par Concept-checking class
 * \code
 * template <class HDS> 
 * struct ForwardHDSConcept {
 *   typedef typename hds_traits<HDS>::halfedge_descriptor halfedge_descriptor; 
 *   typedef typename hds_traits<HDS>::traversal_category traversal_category;
 *   typedef typename hds_traits<HDS>::forward_category forward_category;
 *    void constraints() {
 *       using namespace boost;
 *       function_requires<HDSConcept<HDS> >();
 *       function_requires<ConvertibleConcept<traversal_category
 *                                                ,forward_traversal_tag> >();  
 *       const bool is_valid_storage_tag =
 *          is_convertible<forward_category,next_at_source_tag>::value ||
 *          is_convertible<forward_category,next_at_target_tag>::value ||
 *          is_convertible<forward_category,next_in_facet_tag>::value;
 *       BOOST_STATIC_ASSERT( is_valid_storage_tag ); 
 *       // Error message could be improved
 *       h = next_in_facet(hds,h);
 *       h = next_at_source(hds,h);
 *       h = next_at_target(hds,h);
 *       const_constraints(hds);
 *    }
 *    void const_constraints(HDS const& hds){
 *       h = next_in_facet(hds,h);
 *       h = next_at_source(hds,h);
 *       h = next_at_target(hds,h);
 *    }
 *    ForwardHDS hds;
 *    halfedge_descriptor h;
 *    halfedge_descriptor g;
 * };
 * \endcode
 */

#ifdef BOOST_HDSTL_CONCEPTS_FORWARDHDSCONCEPT_HPP
#define BOOST_HDSTL_CONCEPTS_FORWARDHDSCONCEPT_HPP 1

namespace hdstl{
namespace concepts{
   /**
    * @class ForwardHDSConcept
    *
    * @brief Forward HDS concept checking class.
    * 
    * @par Definition
    * This class provides a concept checking class for the ForwardHDSConcept,
    * i.e., using the statement boost::function_requires<ForwardHDSConcept<HDS>
    * >() inside a function body, or BOOST_CLASS_REQUIRE(HDS, boost::hdstl,
    * ForwardHDSConcept) inside a class definition body, should trigger a
    * compile-time error if the type HDS does not model the ForwardHDSConcept.
    */
   template <class HDS> 
   struct ForwardHDSConcept {
      //@{
      /** @name Required types: 
       * The specialization of hds_traits<HDS> must have
       * the required types below
       */
      typedef typename hds_traits<HDS>::halfedge_descriptor halfedge_descriptor; 
      typedef typename hds_traits<HDS>::traversal_category traversal_category;
      typedef typename hds_traits<HDS>::forward_category forward_category;
      //@}

      /**
       * @name Defined constraints:
       */
      void constraints() {
         using namespace boost;

         function_requires<HDSConcept<HDS> >();
         function_requires<ConvertibleConcept<traversal_category
                                                       ,forwardS> >();  
         const bool is_valid_storage_tag =
            is_convertible<forward_category,next_at_source_tag>::value ||
            is_convertible<forward_category,next_at_target_tag>::value ||
            is_convertible<forward_category,next_in_facet_tag>::value;
         BOOST_STATIC_ASSERT( is_valid_storage_tag ); 
         // Error message could be improved

         h = next_in_facet(hds,h);
         h = next_at_source(hds,h);
         h = next_at_target(hds,h);
         const_constraints(hds);
      }
      /**
       * Defined const constraints
       */
      void const_constraints(HDS const& hds){
         h = next_in_facet(hds,h);
         h = next_at_source(hds,h);
         h = next_at_target(hds,h);
      }
      
      ForwardHDS hds;
      halfedge_descriptor h;
      halfedge_descriptor g;
   };
}  // close namespace concepts
}  // close namespace hdstl

#endif
