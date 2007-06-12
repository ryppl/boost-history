/**
 * @file forward_hds_concept_archetype.hpp
 *
 * @brief ForwardHDS concept archetype 
 *
 * @par Definition
 * This file provides an archetype class for compile time testing of the 
 * ForwardHDSConcept class.
 *
 * @par Archetype class
 * \code
 * class ForwardHDSConcept_archetype
 *    :HDSConcept_archetype {
 *    public:
 *       halfedge_descriptor& next_in_faccet(ForwardHDSConcept_archetype,
 *                                               halfedge_descriptor&) const{};
 *       halfedge_descriptor& next_at_source(ForwardHDSConcept_archetype,
 *                                               halfedge_descriptor&) const{};
 *       halfedge_descriptor& next_at_target(ForwardHDSConcept_archetype,
 *                                               halfedge_descriptor&) const{};
 *
 * };
 * template<>
 * class hds_traits<ForwardHDSConcept_archetype> {
 *    struct halfedge_descriptor 
 *    :public boost::default_constructible_archetype,
 *            boost::copy_constructible_archetype,
 *            boost::equality_comparable_archetype,
 *            boost::assignable_archetype {};
 *
 *    struct traversal_category 
 *    :public boost::convertible_to_archetype<forward_traversal_tag>
 *       {};
 * };
 * \endcode
 */
#include <boost/concept_archetype.hpp>

class ForwardHDSConcept_archetype
  :HDSConcept_archetype {
  public:
     halfedge_descriptor& next_in_faccet(ForwardHDSConcept_archetype,
                                             halfedge_descriptor&) const{};
     halfedge_descriptor& next_at_source(ForwardHDSConcept_archetype,
                                             halfedge_descriptor&) const{};
     halfedge_descriptor& next_at_target(ForwardHDSConcept_archetype,
                                             halfedge_descriptor&) const{};

};
template<>
class hds_traits<ForwardHDSConcept_archetype> {
  struct halfedge_descriptor 
  :public boost::default_constructible_archetype,
          boost::copy_constructible_archetype,
          boost::equality_comparable_archetype,
          boost::assignable_archetype {};

  struct traversal_category 
  :public boost::convertible_to_archetype<forward_traversal_tag>
     {};
};
