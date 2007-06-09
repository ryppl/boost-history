namespace hdstl{
namespace concepts{
   /**
    * @brief Mutable HDS concept definition.
    * 
    * @par Definition
    * This class provides a concept checking for the Mutable HDSConcept, i.e.,
    * using the statement boost::function_requires<HDSConcept<HDS> >() inside a
    * function body, or BOOST_CLASS_REQUIRE(HDS, boost::hdstl, HDSConcept)
    * inside a class definition body, should trigger a compile-time error if
    * the type HDS does not model the MutableHDSConcept.
    *
    * @par Refinement of
    * @HDSConcept
    *
    * @par Associated types
    *  - X::halfedge_descriptor: must be DefaultConstructible, 
    *    EqualityComparable, Assignable.  
    *  - X::traversal_category:  must exist as a type, no other requirement.
    *
    * @par Notation
    *  - X    A type that is a model of HDSConcept
    *  - h, h1, h2  halfedge descriptors
    *
    * @par Definitions
    * @e halfedge @e descriptor is a pointer to access the halfedge
    *
    * @par Valid Expressions
    *   - set_opposite(hds,h1,h2): Sets the the opposite halfedge.
    *   - h = new_edge(h1,h2) : Creates a new edge, with two halfedges. (Note
    *     that halfedges are opposites of each other.)
    *   - delete_edge(h) : Deletes the edge defined by @e h. (Deletes two 
    *     halfedges.)
    *
    * @par Expression Semantics
    *
    * @par Complexity guarantees
    *   - opposite(hds, h): Amortized constant time.
    *   - set_opposite(hds, h1, h2): Amortized constant time.
    *   - h = new_edge(h1, h2): Amortized constant time.
    *
    * @par Invariants 
    *   - set_opposite(hds,h1,h2) : after setting should verify 
    *     h1 = opposite(hds,h2),  and h2 = opposite(h1).
    *   - h = new_edge(h1,h2) : should verify h1 = opposite(hds,h2), and 
    *     h2 = opposite(hds,h1).
    */
   template <class HDS> 
   struct MutableHDSConcept {
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
         function_requires<HDSConcept<HDS> >();

         set_opposite(hds,h1,h2);
         h = new_edge(h1,h2);
         delete_edge(h);
         const_constraints(hds);
      }

      /**
       * Defined const constraints:
       */
      void const_constraints(HDS const& hds) {
         set_opposite(hds,h1,h2);
         h = new_edge(h1,h2);
         delete_edge(h);
      }
      /// \cond
      MutableHDS hds;
      halfedge_descriptor h;
      halfedge_descriptor h1;
      halfedge_descriptor h2;
      /// \endcond
   };
}} //namespaces
