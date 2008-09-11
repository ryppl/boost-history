#ifndef PROPERTIES_TRAITS_HPP
#define PROPERTIES_TRAITS_HPP

// These actually determine the mutability of the entire edge set.
struct property_vector_tag : unstable_remove_tag { };
struct property_list_tag : stable_mutators_tag { };

template <typename Props>
struct properties_traits
{ typedef typename Props::category category; };

template <typename Props>
typename properties_traits<Props>::category
properties_category(Props const&)
{ return typename properties_traits<Props>::category(); }

template <typename Edge, typename Alloc>
struct properties_traits<property_vector<Edge, Alloc>>
{ typedef property_vector_tag category; };

template <typename Edge, typename Alloc>
struct properties_traits<property_list<Edge, Alloc>>
{ typedef property_list_tag category; };

#endif
