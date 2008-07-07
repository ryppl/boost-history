
#ifndef INCIDENCE_TRAITS_HPP
#define INCIDENCE_TRAITS_HPP

struct incidence_vector_tag : sequence_tag, unstable_remove_tag { };
struct incidence_list_tag : sequence_tag, stable_mutators_tag { };
struct incidence_set_tag : associative_container_tag, stable_mutators_tag { };

template <typename IncStore>
struct incidence_traits
{ typedef typename IncStore::category category; };

template <typename IncStore>
typename incidence_traits<IncStore>::category
incidence_category(IncStore const&)
{ return typename incidence_traits<IncStore>::category(); }

template <typename Edge, typename Alloc>
struct incidence_traits<incidence_vector<Edge, Alloc>>
{ typedef incidence_vector_tag category; };

template <typename Edge, typename Alloc>
struct incidence_traits<incidence_list<Edge, Alloc>>
{ typedef incidence_list_tag category; };

template <typename Edge, typename Comp, typename Alloc>
struct incidence_traits<incidence_set<Edge, Comp, Alloc>>
{ typedef incidence_set_tag category; };

#endif
