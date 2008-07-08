
#ifndef OUT_EDGE_TRAITS_HPP
#define OUT_EDGE_TRAITS_HPP

struct out_vector_tag : sequence_tag, unstable_remove_tag { };
struct out_list_tag : sequence_tag, stable_mutators_tag { };
struct out_set_tag : associative_container_tag, stable_mutators_tag { };

template <typename Outs>
struct out_edge_traits
{ typedef typename Outs::category cateogry; };

template <typename Outs>
typename out_edge_traits<Outs>::category
out_edge_category(Outs const&)
{ return typename out_edge_traits<Outs>::category(); }


template <typename Edge, typename Alloc>
struct out_edge_traits<out_vector<Edge, Alloc>>
{ typedef out_vector_tag category; };

template <typename Edge, typename Alloc>
struct out_edge_traits<out_list<Edge, Alloc>>
{ typedef out_list_tag category; };

template <typename Edge, typename Comp, typename Alloc>
struct out_edge_traits<out_set<Edge, Comp, Alloc>>
{ typedef out_set_tag category; };

#endif
