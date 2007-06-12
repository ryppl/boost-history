/**
 * @file hds_concept_archetype.hpp
 *
 * @brief HDS concept archetype 
 *
 * @par Definition
 * This file provides an archetype class for compile time testing of the 
 * HDSConcept class.
 *
 * @par Archetype class
 * \code
 * class HDSConcept_archetype {
 *    public:
 *       halfedge_descriptor& opposite(halfedge_descriptor&) const{};
 * };
 * template<>
 * class hds_traits<HDSConcept_archetype> {
 *    struct halfedge_descriptor 
 *    :public boost::default_constructible_archetype,
 *            boost::copy_constructible_archetype,
 *            boost::equality_comparable_archetype,
 *            boost::assignable_archetype {};
 * };
 * \endcode
 */
#include <boost/concept_archetype.hpp>

class HDSConcept_archetype {
   public:
      halfedge_descriptor& opposite(halfedge_descriptor&) const{};
};

template<>
class hds_traits<HDSConcept_archetype> {
   struct halfedge_descriptor 
   :public boost::default_constructible_archetype,
      boost::copy_constructible_archetype,
      boost::equality_comparable_archetype,
      boost::assignable_archetype {};
};
