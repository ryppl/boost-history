
#ifndef IN_EDGE_TRAITS_HPP
#define IN_EDGE_TRAITS_HPP

struct in_vector_tag : sequence_tag, unstable_remove_tag { };
struct in_list_tag : sequence_tag, stable_mutators_tag { };
struct in_set_tag : associative_container_tag, stable_mutators_tag { };

template <typename Ins>
struct in_edge_traits
{ typedef typename Ins::category cateogry; };

template <typename Ins>
typename in_edge_traits<Ins>::category
in_edge_category(Ins const&)
{ return typename in_edge_traits<Ins>::category(); }


template <typename Edge, typename Alloc>
struct in_edge_traits<in_vector<Edge, Alloc>>
{ typedef in_vector_tag category; };

template <typename Edge, typename Alloc>
struct in_edge_traits<in_list<Edge, Alloc>>
{ typedef in_list_tag category; };

template <typename Edge, typename Comp, typename Alloc>
struct in_edge_traits<in_set<Edge, Comp, Alloc>>
{ typedef in_set_tag category; };

#endif
