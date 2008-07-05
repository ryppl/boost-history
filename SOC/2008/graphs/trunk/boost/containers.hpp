// (C) Copyright 2004 Jeremy Siek
//  (C) Copyright 2008 Andrew Sutton
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#ifndef CONTAINERS_HPP
#define CONTAINERS_HPP

#include <boost/type_traits.hpp>

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
struct forward_container_tag : virtual container_tag { };
struct reversible_container_tag : virtual forward_container_tag { };
struct random_access_container_tag : virtual reversible_container_tag { };

// Sequential containers
struct sequence_tag : virtual forward_container_tag { };
struct front_insertion_sequence_tag : virtual sequence_tag { };
struct back_insertion_sequence_tag : virtual sequence_tag { };

// Associative containers
struct associative_container_tag : virtual forward_container_tag { };
struct sorted_associative_container_tag : virtual associative_container_tag, virtual reversible_container_tag { };
struct unique_associative_container_tag : virtual associative_container_tag { };
struct multiple_associative_container_tag : virtual associative_container_tag { };
struct simple_associative_container_tag : virtual associative_container_tag { };
struct pair_associative_container_tag : virtual associative_container_tag { };

// These aren't real concepts, just combinations of others.
struct set_container_tag : virtual simple_associative_container_tag, virtual unique_associative_container_tag { };
struct map_container_tag : virtual pair_associative_container_tag, virtual unique_associative_container_tag { };
struct multiset_container_tag : virtual simple_associative_container_tag, virtual multiple_associative_container_tag { };
struct multimap_container_tag : virtual pair_associative_container_tag, virtual multiple_associative_container_tag { };

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

// Trait accessors

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

// Metafunctions

/**
 * Evaluates to true if the container contains unique elements.
 */
template <typename Container>
struct contains_unique_elements
{
    static bool const value =
        boost::is_convertible<
            typename container_traits<Container>::category,
            unique_associative_container_tag
        >::value;
};

/**
 * Evaluates to true if the container's elements are mapped to a key. This is
 * only true for pair associative containers, not sets (which are a little
 * different).
 */
template <typename Container>
struct contains_mapped_elements
{
    static bool const value =
        boost::is_convertible<
            typename container_traits<Container>::category,
            pair_associative_container_tag
        >::value;
};

// Specializations

// Vector
struct vector_tag :
    virtual random_access_container_tag,
    virtual back_insertion_sequence_tag
{ };

template <class T, class Alloc>
struct container_traits<std::vector<T,Alloc>>
{
    typedef vector_tag category;
    typedef unstable_iterator_tag iterator_stability;
};

// List
struct list_tag :
    virtual reversible_container_tag,
    virtual back_insertion_sequence_tag,
    virtual front_insertion_sequence_tag
{ };

template <class T, class Alloc>
struct container_traits<std::list<T,Alloc>>
{
    typedef list_tag category;
    typedef stable_iterator_tag iterator_stability;
};

// TODO: Dequeue

// Set
struct set_tag :
    virtual sorted_associative_container_tag,
    virtual simple_associative_container_tag,
    virtual unique_associative_container_tag
{ };


template <class Key, class Cmp, class Alloc>
struct container_traits<std::set<Key,Cmp,Alloc>>
{
    typedef set_tag category;
    typedef stable_iterator_tag iterator_stability;
};

// Multiset
struct multiset_tag :
    virtual sorted_associative_container_tag,
    virtual simple_associative_container_tag,
    virtual multiple_associative_container_tag
{ };

template <class T, class Compare, class Alloc>
struct container_traits<std::multiset<T, Compare, Alloc>>
{
    typedef multiset_tag category;
    typedef stable_iterator_tag iterator_stability;
};

// Map
struct map_tag :
    virtual sorted_associative_container_tag,
    virtual pair_associative_container_tag,
    virtual unique_associative_container_tag
{ };

template <class Key, class T, class Compare, class Alloc>
struct container_traits<std::map<Key, T, Compare, Alloc>>
{
    typedef map_tag category;
    typedef stable_iterator_tag iterator_stability;
};

// Multimap
struct multimap_tag :
    virtual sorted_associative_container_tag,
    virtual pair_associative_container_tag,
    virtual multiple_associative_container_tag
{ };

template <class Key, class T, class Compare, class Alloc>
struct container_traits<std::multimap<Key, T, Compare, Alloc>>
{
    typedef multimap_tag category;
    typedef stable_iterator_tag iterator_stability;
};

// TODO: Unordered Set
// TODO: Unordered Multiset
// TODO: Unordered Map
// TODO: Unordered Multimap

// Generic insert/remove functions

namespace dispatch
{
    template <typename Container, typename Value>
    inline typename Container::iterator
    insert(Container& c, Value const& x, sequence_tag)
    { return c.insert(c.end(), x); }

    template <typename Container, typename Value>
    inline typename Container::iterator
    insert(Container& c, Value const& x, unique_associative_container_tag)
    { return c.insert(x).first; }

    template <typename Container, typename Value>
    inline typename Container::iterator
    insert(Container& c, Value const& x, multiple_associative_container_tag)
    { return c.insert(x); }


    template <typename Container>
    inline typename Container::iterator
    erase(Container& c, typename Container::iterator i, sequence_tag)
    { return c.erase(i); }

    template <typename Container>
    inline typename Container::iterator
    erase(Container& c, typename Container::iterator i, associative_container_tag)
    {
        typename Container::iterator j = ++i;
        c.erase(i);
        return j;
    }
}

template <typename Container, typename T>
inline typename Container::iterator
insert(Container& c, T const& x)
{ return dispatch::insert(c, x, container_category(c)); }

template <typename Container, typename T>
inline typename Container::iterator
erase(Container& c, typename Container::iterator i)
{ return dispatch::erase(c, i, container_category(c)); }

#if 0

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
