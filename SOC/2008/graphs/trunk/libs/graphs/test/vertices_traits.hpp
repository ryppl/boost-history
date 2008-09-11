
#ifndef VERTICES_TRAITS_HPP
#define VERTICES_TRAITS_HPP

struct simple_vertex_store_tag { };
struct mapped_vertex_store_tag { };

struct vertex_vector_tag : simple_vertex_store_tag, unstable_remove_tag { };
struct vertex_list_tag : simple_vertex_store_tag, stable_mutators_tag { };
struct vertex_set_tag : simple_vertex_store_tag, stable_mutators_tag { };
struct vertex_map_tag : mapped_vertex_store_tag, stable_mutators_tag { };

template <typename Verts>
struct vertices_traits
{ typedef typename Verts::category category; };

template <typename Verts>
typename vertices_traits<Verts>::category
vertices_category(Verts const&)
{ return typename vertices_traits<Verts>::category(); }


template <typename Vertex, typename Alloc>
struct vertices_traits<vertices_vector<Vertex, Alloc>>
{ typedef vertex_vector_tag category; };

template <typename Vertex, typename Alloc>
struct vertices_traits<vertices_list<Vertex, Alloc>>
{ typedef vertex_list_tag category; };

template <typename Vertex, typename Compare, typename Alloc>
struct vertices_traits<vertices_set<Vertex, Compare, Alloc>>
{ typedef vertex_set_tag category; };

template <typename Vertex, typename Key, typename Compare, typename Alloc>
struct vertices_traits<vertices_map<Vertex, Key, Compare, Alloc>>
{ typedef vertex_map_tag category; };

#endif
