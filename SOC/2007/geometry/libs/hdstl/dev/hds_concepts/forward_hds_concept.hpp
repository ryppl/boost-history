namespace hdstl{
namespace concepts{
   /**
    * @file forward_hds_concept.hpp
    *
    * @class ForwardHDSConcept
    *
    * @brief Forward HDS concept definition.
    * 
    * @par Definition
    * This class provides a concept checking for the ForwardHDSConcept, i.e.,
    * using the statement boost::function_requires<ForwardHDSConcept<HDS> >()
    * inside a function body, or BOOST_CLASS_REQUIRE(HDS, boost::hdstl,
    * ForwardHDSConcept) inside a class definition body, should trigger a
    * compile-time error if the type HDS does not model the ForwardHDSConcept.
    *
    * @par Refinement of
    * @par HDSConcept
    *
    * @par Associated types
    *  - X::halfedge_descriptor: must be DefaultConstructible, 
    *    EqualityComparable, Assignable.  
    *  - X::traversal_category:  must exist as a type, no other requirement.
    *  - X::forward_category:  Defines the pointer type for forward iteration.
    *
    * @par Notation
    *  - X    A type that is a model of ForwardHDSConcept
    *  - h,g  halfedge descriptors
    *
    * @par Definitions
    * @e halfedge @e descriptor is a pointer to access the halfedge
    *
    * @par Valid Expressions
    *   - next_in_facet(hds,h) : Pointer to next halfedge in forward direction
    *     around the facet.
    *   - next_at_source(hds,h) : Pointer to next halfedge in forward direction
    *     around the source.
    *   - next_at_target(hds,h) : Pointer to next halfedge in forward direction
    *     around the target.
    *
    * @par Expression Semantics
    *
    * @par Complexity guarantees
    *  - next_in_facet(hds,h) : Amortized constant time
    *  - next_at_source(hds,h) : Amortized constant time
    *  - next_at_target(hds,h) : Amortized constant time
    *
    * @par Invariants 
    *   - This concept should validate HDSConcept also. 
    *   - forward_category has to be convertible to one of: next_in_facet, 
    *     next_at_source, or next_at_target.
    *   - traversal_category has to be convertible to forwardS
    *   - g = next_in_facet(hds,h), if defined, g should be a valid type
    *   - g = next_at_source(hds,h), if defined, g should be a valid type
    *   - g = next_at_target(hds,h), if defined, g should be a valid type
    */
   template <class HDS> 
   struct ForwardHDSConcept {
      /// \cond
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
            is_convertible<forward_category,next_at_sourceS>::value ||
            is_convertible<forward_category,next_at_targetS>::value ||
            is_convertible<forward_category,next_in_facetS>::value;
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
      /// \endcond
   };
}} //namespaces
