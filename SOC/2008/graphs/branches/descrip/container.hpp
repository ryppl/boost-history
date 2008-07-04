//  (C) Copyright 2004 Jeremy Siek 
//  (C) Copyright 2008 Andrew Sutton
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#ifndef CONTAINER_HPP
#define CONTAINER_HPP

// Forward declarations of stdandard types. Jeremy's right! There does need
// to be an <stlfwd> header since there's no guarantee that these are the
// correct signatures for these types.
namespace std
{
    template <typename, typename> class vector;
    template <typename, typename> class list;
    template <typename, typename, typename> class set;
    template <typename, typename, typename> class multiset;
    template <typename, typename, typename, typename> class map;
    template <typename, typename, typename, typename> class multimap;
}

// TODO: This probably all goes away with concepts. Seeing as how concepts
// aren't in C++ just yet, we'll still do things this way.

// Container Category Tags
// These define the basic concepts of STL containers. Note the use of
// virtual inheritance because there are lots of inheritance diamonds.
struct container_tag { };
struct forward_container_tag : virtual public container_tag { };
struct reversible_container_tag : virtual public forward_container_tag { };
struct random_access_container_tag : virtual public reversible_container_tag { };
struct sequence_tag : virtual public forward_container_tag { };
struct associative_container_tag : virtual public forward_container_tag { };
struct sorted_associative_container_tag : virtual public associative_container_tag , virtual public reversible_container_tag { };
struct front_insertion_sequence_tag : virtual public sequence_tag { };
struct back_insertion_sequence_tag : virtual public sequence_tag { };
struct unique_associative_container_tag  : virtual public associative_container_tag { };
struct multiple_associative_container_tag  : virtual public associative_container_tag { };
struct simple_associative_container_tag : virtual public associative_container_tag { };
struct pair_associative_container_tag : virtual public associative_container_tag { };

// Iterator Stability Tags
// These tags determine the "stability" of iterators. Do mutating operations
// such as insert/erase/resize invalidate all outstanding iterators, or just
// those being removed. Most node-based implementations have stable iteraors,
// while vectors are generally unstable.
struct stable_iterator_tag { };
struct unstable_iterator_tag { };

/**
 * The container traits class defines the basic container concepts and iterator
 * stability properties of the given container.
 */
template <class Container>
struct container_traits
{
    typedef typename Container::category category;
    typedef typename Container::iterator_stability iterator_stability;
};

/** Return the container category tag of the given container. */
template <typename Container>
inline typename container_traits<Container>::category
container_category(Container const&)
{ return typename container_traits<Container>::category(); }

/** Return the iterator stability tag of the given container. */
template <class Container>
inline typename container_traits<Container>::iterator_stability
iterator_stability(Container const&)
{ return typename container_traits<Container>::iterator_stability(); }

// Pull specializtions and metafunctions.
#include "container/functions.hpp"
#include "container/vector.hpp"
#include "container/list.hpp"
#include "container/set.hpp"
#include "container/map.hpp"
#include "container/multiset.hpp"
#include "container/multimap.hpp"

// Use this as a compile-time assertion that X is stable
// inline void require_stable(stable_tag) { }

#if 0
  // deque

  // std::map

  template <class Key, class T, class Cmp, class Alloc> 
  map_tag container_category(const std::map<Key,T,Cmp,Alloc>&)
  { return map_tag(); }

  template <class Key, class T, class Cmp, class Alloc> 
  stable_tag iterator_stability(const std::map<Key,T,Cmp,Alloc>&)
  { return stable_tag(); }

  // std::multimap

  template <class Key, class T, class Cmp, class Alloc> 
  multimap_tag container_category(const std::multimap<Key,T,Cmp,Alloc>&)
  { return multimap_tag(); }

  template <class Key, class T, class Cmp, class Alloc> 
  stable_tag iterator_stability(const std::multimap<Key,T,Cmp,Alloc>&)
  { return stable_tag(); }


 // hash_set, hash_map

#ifndef BOOST_NO_HASH
#ifndef BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
  template <class Key, class Eq, class Hash, class Alloc> 
  struct container_traits< BOOST_STD_EXTENSION_NAMESPACE::hash_set<Key,Eq,Hash,Alloc> > {
    typedef set_tag category;
    typedef stable_tag iterator_stability; // is this right?
  };
  template <class Key, class T, class Eq, class Hash, class Alloc>
  struct container_traits< BOOST_STD_EXTENSION_NAMESPACE::hash_map<Key,T,Eq,Hash,Alloc> > {
    typedef map_tag category;
    typedef stable_tag iterator_stability; // is this right?
  };
#endif
  template <class Key, class Eq, class Hash, class Alloc>
  set_tag container_category(const BOOST_STD_EXTENSION_NAMESPACE::hash_set<Key,Eq,Hash,Alloc>&)
  { return set_tag(); }

  template <class Key, class T, class Eq, class Hash, class Alloc>
  map_tag container_category(const BOOST_STD_EXTENSION_NAMESPACE::hash_map<Key,T,Eq,Hash,Alloc>&)
  { return map_tag(); }

  template <class Key, class Eq, class Hash, class Alloc>
  stable_tag iterator_stability(const BOOST_STD_EXTENSION_NAMESPACE::hash_set<Key,Eq,Hash,Alloc>&)
  { return stable_tag(); }

  template <class Key, class T, class Eq, class Hash, class Alloc>
  stable_tag iterator_stability(const BOOST_STD_EXTENSION_NAMESPACE::hash_map<Key,T,Eq,Hash,Alloc>&)
  { return stable_tag(); }
#endif



  //===========================================================================
  // Generalized Container Functions


  // Erase
  template <class Sequence, class T>
  void erase_dispatch(Sequence& c, const T& x, 
                      sequence_tag)
  {
    c.erase(std::remove(c.begin(), c.end(), x), c.end());
  }

  template <class AssociativeContainer, class T>
  void erase_dispatch(AssociativeContainer& c, const T& x, 
                      associative_container_tag)
  {
    c.erase(x);
  }
  template <class Container, class T>
  void erase(Container& c, const T& x)
  {
    erase_dispatch(c, x, container_category(c));
  }

  // Erase If
  template <class Sequence, class Predicate, class IteratorStability>
  void erase_if_dispatch(Sequence& c, Predicate p,
                         sequence_tag, IteratorStability)
  {
#if 0
    c.erase(std::remove_if(c.begin(), c.end(), p), c.end());
#else
    if (! c.empty())
      c.erase(std::remove_if(c.begin(), c.end(), p), c.end());
#endif
  }
  template <class AssociativeContainer, class Predicate>
  void erase_if_dispatch(AssociativeContainer& c, Predicate p,
                         associative_container_tag, stable_tag)
  {
    typename AssociativeContainer::iterator i, next;
    for (i = next = c.begin(); next != c.end(); i = next) {
      ++next;
      if (p(*i))
        c.erase(i);
    }
  }
  template <class AssociativeContainer, class Predicate>
  void erase_if_dispatch(AssociativeContainer& c, Predicate p,
                         associative_container_tag, unstable_tag)
  {
    // This method is really slow, so hopefully we won't have any
    // associative containers with unstable iterators!
    // Is there a better way to do this?
    typename AssociativeContainer::iterator i;
    typename AssociativeContainer::size_type n = c.size();
    while (n--)
      for (i = c.begin(); i != c.end(); ++i)
        if (p(*i)) {
          c.erase(i);
          break;
        }
  }
  template <class Container, class Predicate>
  void erase_if(Container& c, Predicate p)
  {
    erase_if_dispatch(c, p, container_category(c), iterator_stability(c));
  }

  // Push
  template <class Container, class T>
  std::pair<typename Container::iterator, bool>
  push_dispatch(Container& c, const T& v, back_insertion_sequence_tag)
  {
    c.push_back(v);
    return std::make_pair(boost::prior(c.end()), true);
  }

  template <class Container, class T>
  std::pair<typename Container::iterator, bool>
  push_dispatch(Container& c, const T& v, front_insertion_sequence_tag)
  {
    c.push_front(v);
    return std::make_pair(c.begin(), true);
  }

  template <class AssociativeContainer, class T>
  std::pair<typename AssociativeContainer::iterator, bool>
  push_dispatch(AssociativeContainer& c, const T& v, 
                unique_associative_container_tag)
  {
    return c.insert(v);
  }

  template <class AssociativeContainer, class T>
  std::pair<typename AssociativeContainer::iterator, bool>
  push_dispatch(AssociativeContainer& c, const T& v,
                multiple_associative_container_tag)
  {
    return std::make_pair(c.insert(v), true);
  }

  template <class Container, class T>
  std::pair<typename Container::iterator,bool>
  push(Container& c, const T& v)
  {
    return push_dispatch(c, v, container_category(c));
  }

}} // namespace boost::graph_detail

#endif

#endif
