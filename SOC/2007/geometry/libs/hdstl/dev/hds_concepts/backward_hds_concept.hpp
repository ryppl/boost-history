namespace hdstl{
namespace concepts{
   /**
    * @file backward_hds_concept.hpp
    *
    * @class BackwardHDSConcept
    * 
    * @brief Backward HDS concept definition.
    * 
    * @par Definition
    * This class provides a concept checking for the BackwardHDSConcept, i.e.,
    * using the statement boost::function_requires<BackwardHDSConcept<HDS> >()
    * inside a function body, or BOOST_CLASS_REQUIRE(HDS, boost::hdstl,
    * BackwardHDSConcept) inside a class definition body, should trigger a
    * compile-time error if the type HDS does not model the BackwardHDSConcept.
    *
    * @par Refinement of
    * @par HDSConcept
    *
    * @par Associated types
    *  - X::halfedge_descriptor: must be DefaultConstructible, 
    *    EqualityComparable, Assignable.  
    *  - X::traversal_category:  must exist as a type, convertible to 
    *    backward_traversal_tag.
    *  - X::backward_category:  Defines the pointer type for backward iteration.
    *
    * @par Notation
    *  - X    A type that is a model of BackwardHDSConcept
    *  - h,g  halfedge descriptors
    *
    * @par Definitions
    * @e halfedge @e descriptor is a pointer to access the halfedge
    *
    * @par Valid Expressions
    *   - prev_in_facet(hds,h) : Must return pointer to prev halfedge in 
    *     backward direction around the facet.
    *   - prev_at_source(hds,h) : Must return pointer to prev halfedge in 
    *     backward direction around the source.
    *   - prev_at_target(hds,h) : Must return pointer to prev halfedge in 
    *     backward direction around the target.
    *
    * @par Expression Semantics
    *
    * @par Complexity guarantees
    *   - prev_in_facet(hds,h) :  Amortized constant time.
    *   - prev_at_source(hds,h) :  Amortized constant time.
    *   - prev_at_target(hds,h) :  Amortized constant time.
    *
    * @par Invariants 
    *   - This concept should validate HDSConcept also. 
    *   - backward_category has to be convertible to one of: prev_in_facet, 
    *     prev_at_source, or prev_at_target.
    *   - traversal_category has to be convertible to backward_traversal_tag
    *   - g = prev_in_facet(hds,h), if defined, g should be a valid type
    *   - g = prev_at_source(hds,h), if defined, g should be a valid type
    *   - g = prev_at_target(hds,h), if defined, g should be a valid type
    */
   template <class HDS> 
   struct BackwardHDSConcept {
      /// \cond
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
         function_requires<ConvertibleConcept<traversal_category
                                                       ,backwardS> >();  
         const bool is_valid_storage_tag =
            is_convertible<backward_category,prev_at_sourceS>::value ||
            is_convertible<backward_category,prev_at_targetS>::value ||
            is_convertible<backward_category,prev_in_facetS>::value;
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
      /// \endcond
   };
}} //namespaces
