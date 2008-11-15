
#ifndef OUT_EDGE_TRAITS_HPP
#define OUT_EDGE_TRAITS_HPP

namespace boost { namespace graphs { namespace adjacency_list {

struct out_vector_tag : vector_tag, unstable_remove_tag { };
struct out_list_tag : list_tag, stable_mutators_tag { };
struct out_set_tag : set_tag, stable_mutators_tag { };

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

} } }

#endif
