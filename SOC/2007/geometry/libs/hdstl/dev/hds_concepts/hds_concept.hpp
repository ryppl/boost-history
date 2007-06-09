namespace hdstl{
namespace concepts {

   /**
    * @file hds_concept.hpp
    *
    * @class HDSConcept
    *
    * @brief HDS concept definition.
    *
    * @par Definition
    * This class provides a concept checking class for the HDSConcept, i.e.,
    * using the statement boost::function_requires<HDSConcept<HDS> >() inside a
    * function body, or BOOST_CLASS_REQUIRE(HDS, boost::hdstl, HDSConcept)
    * inside a class definition body, should trigger a compile-time error if
    * the type HDS does not model the HDSConcept.
    *
    * @par Refinement of:
    * This concept is not a refinement of another concept, in   particular a
    * model HDS of HDSConcept does @e not need to be DefaultConstructible,
    * CopyConstructible, or Assignable.
    *
    * @par Notation
    *  - HDS        A type that is a model of HDSConcept
    *  - X          The type HDSConcept<HDS>
    *  - hds        A non-modifiable instance of HDS
    *  - h,g        Halfedge descriptors, of type X::halfedge_descriptor
    *
    * @par Associated types
    *  - X::halfedge_descriptor: must be DefaultConstructible and 
    *    CopyConstructible, EqualityComparable, Assignable.
    *
    * @par Definitions
    * @e halfedge @e descriptor is a type that contains information to access
    * the halfedge.  Note that the halfedge descriptor type (accessible as a
    * member of the hds_traits<HDS> type) is an opaque type.  All that can be
    * done to it is passing it to the function opposite and letting the hds use
    * the information it contains to find the opposite halfedge.  In
    * particular, there is no guarantee that it is a pointer type.
    *
    * @par Valid Expressions
    *   - opposite(hds, h): Must return a value assignable to h.
    *
    * @par Expression Semantics
    *   - opposite(hds, h): Returns a descriptor to the halfedge opposite h in
    *     the data structure hds. Does not modify the state of either hds,
    *     or h
    *
    * @par Complexity guarantees
    *   - opposite(hds, h): Amortized constant time.
    *
    * @par Invariants
    *   - Every valid halfedge descriptor h must have a valid opposite
    *     g = opposite(hds, h), @e distinct from h, such that
    *     opposite(hds, g) returns h.  (Hence, opposite is an involution.)
    */
   template <class HDS> 
   struct HDSConcept {
      /// \cond
      //@{
      /** @name Required types: 
       * The specialization of hds_traits<HDS> must have the required types
       * below, obeying the types requirements stated in the detailed
       * description.
       */
      typedef typename hds_traits<HDS>::halfedge_descriptor halfedge_descriptor; 
      //@}

      /**
       * @name Defined constraints:
       */
      void constraints() {
         using namespace boost;
         function_requires<DefaultConstructibleConcept<halfedge_descriptor> >();
         function_requires<CopyConstructibleConcept<halfedge_descriptor> >();
         function_requires<EqualityComparableConcept<halfedge_descriptor> >();
         function_requires<AssignableConcept<halfedge_descriptor> >();

         h = opposite(hds,h);
         const_constraints(hds);
      }

      /**
       * Defined const constraints:
       */
      void const_constraints(HDS const& hds) {
         h = opposite(hds,h);
      }
      HDS hds;
      halfedge_descriptor h;
      /// \endcond
   };

 }}//namespaces
