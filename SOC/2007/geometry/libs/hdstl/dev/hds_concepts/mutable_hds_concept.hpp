/**
 * @file mutable_hds_concept.hpp
 *
 * @brief Mutable HDS concept definition.
 *
 * @par Definition
 * This concept is the basic Mutable HDS concept, which all the other Mutable
 * HDS concepts refine.  It contains a few requirements which are common to all
 * the graph concepts.  One should note that a model of Mutable HDS is not
 * required to be a model of Assignable, so algorithms should pass Mutable HDS
 * objects by reference.
 *
 * @par Refinement of:
 *  - HDSConcept
 *
 * @par Notation
 *  - HDS        A type that is a model of MutableHDSConcept
 *  - hds        A non-modifiable instance of HDS
 *  - h1,h2  Halfedge descriptors, of type X::halfedge_descriptor
 *
 * @par Associated types
 *  - hds_traits<HDS>::halfedge_descriptor: must be DefaultConstructible and 
 *                         CopyConstructible, EqualityComparable, Assignable.
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
 *  - set_opposite(hds,h1,h2): Sets the opposite halfedge.
 *  - h = new_edge(hds,h1,h2) : Creates a new edge, with two halfedges. 
 *  - delete_edge(hds,h1,h2) : Deletes the edge defined by halfedge pairs h1
 *                                                                    and h2.
 *
 * @par Expression Semantics
 *  - set_opposite(hds,h1,h2): Sets h1 and h2 as opposites of each other in 
 *                                                    the data structure hds. 
 *  - h = new_edge(hds,h1,h2): Creates a new edge in data structure hds. Since
 *   halfedges are actually pairs, the new edge consists of two halfedges, 
 *   h1 and h2, which are set as opposites of each other.
 *  - delete_edge(hds,h1,h2): Since halfedges are defined as pairs, and a single
 *  halfedge is useless, they are deleted as pairs also. Delete edge removes
 *  the opposite halfedges h1, and h2 from the data structure hds. Note that
 *  only the halfedges that form a pair by validating the opposite function
 *  can be deleted by this method.
 *
 * @par Complexity guarantees
 *  - set_opposite(hds,h1,h2): Amortized constant time.
 *  - new_edge(hds,h1,h2): Amortized constant time.
 *  - delete_edge(hds,h1,h2): Amortized constant time.
 *
 * @par Invariants
 *  - After set_opposite and new_edge functions, h1=opposite(h2), and
 *                                     h2=opposite(h1) should be true.
 *  - Before the delete_edge method, h1=opposite(h2), and h2=opposite(h1) 
 *                                                         should be true.
 *
 * @par Concept-checking class
 * \code
 * template <class HDS> 
 * struct MutableHDSConcept {
 *   typedef typename hds_traits<HDS>::halfedge_descriptor halfedge_descriptor; 
 *    void constraints() {
 *       using namespace boost;
 *       function_requires<HDSConcept<HDS> >();
 *       set_opposite(hds,h1,h2);
 *       h = new_edge(hds,h1,h2);
 *       delete_edge(hds,h1,h2);
 *       const_constraints(hds);
 *    }
 *    void const_constraints(HDS const& hds) {
 *       set_opposite(hds,h1,h2);
 *       h = new_edge(hds,h1,h2);
 *       delete_edge(hds,h1,h2);
 *    }
 *    HDS hds;
 *    halfedge_descriptor h1;
 *    halfedge_descriptor h2;
 * };
 * \endcode
 */

#ifdef BOOST_HDSTL_CONCEPTS_MUTABLEHDSCONCEPT_HPP
#define BOOST_HDSTL_CONCEPTS_MUTABLEHDSCONCEPT_HPP 1

namespace hdstl{
namespace concepts {

   /**
    * @class MutableHDSConcept
    *
    * @brief MutableHDS concept checking class
    *
    * @par Definition
    * This class provides a concept checking for the MutableHDSConcept, i.e.,
    * using the statement boost::function_requires<MutableHDSConcept<HDS> >()
    * inside a function body, or BOOST_CLASS_REQUIRE(HDS, boost::hdstl,
    * MutableHDSConcept) inside a class definition body, should trigger a
    * compile-time error if the type HDS does not model the MutableHDSConcept.
    */
   template <class HDS> 
   struct MutableHDSConcept {
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
         function_requires<HDSConcept<HDS> >();

         set_opposite(hds,h1,h2);
         h = new_edge(hds,h1,h2);
         delete_edge(hds,h1,h2);
         const_constraints(hds);
      }

      /**
       * Defined const constraints:
       */
      void const_constraints(HDS const& hds) {
         set_opposite(hds,h1,h2);
         h = new_edge(hds,h1,h2);
         delete_edge(hds,h1,h2);
      }
      HDS hds;
      halfedge_descriptor h1;
      halfedge_descriptor h2;
   };
}  // close namespace concepts
}  // close namespace hdstl

#endif
