// Header file algo_selector.hpp
//
// Automatic selection of container algorithms (and traits) for known
// container types (basically, all STL container instances, as well as
// iterator_range instances).
//
// Copyright (c) 2003 Raoul M. Gough
//
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy
// at http://www.boost.org/LICENSE_1_0.txt)
//
// History
// =======
// 2003/ 9/11   rmg     File creation
//
// $Id$
//

#ifndef BOOST_PYTHON_INDEXING_ALGO_SELECTOR_HPP
#define BOOST_PYTHON_INDEXING_ALGO_SELECTOR_HPP

#include <boost/python/suite/indexing/container_proxy.hpp>
// Include this early to ensure the declaration of the get_pointer
// overload for element_proxy is in-scope for both name-lookup phases

#include <boost/python/suite/indexing/element_proxy_traits.hpp>
#include <boost/python/suite/indexing/container_traits.hpp>
#include <boost/python/suite/indexing/algorithms.hpp>

// Definitions of further supported types
#include <boost/python/suite/indexing/iterator_range.hpp>
#include <set>
#include <map>
#include <list>
#include <deque>
#include <vector>

namespace boost { namespace python { namespace indexing {
  /////////////////////////////////////////////////////////////////////////
  // Automated algorithm and trait selection
  /////////////////////////////////////////////////////////////////////////

  namespace detail {
    template<typename Container> class selector_impl;

    // selector_impl instances should include *two* publically
    // accessible typedefs, one for the non-const version of the
    // container, and one for the const version. This saves having to
    // have two specializations of selector_impl for every kind of
    // container.

    // std::set
    template <class Key, class Compare, class Allocator>
    class selector_impl<std::set<Key, Compare, Allocator> >
    {
      typedef std::set<Key, Compare, Allocator> Container;

      typedef set_traits<Container>       mutable_traits;
      typedef set_traits<Container const> const_traits;

    public:
      typedef set_algorithms<mutable_traits> mutable_algorithms;
      typedef set_algorithms<const_traits>   const_algorithms;
    };

    // std::multiset
    template <class Key, class Compare, class Allocator>
    class selector_impl<std::multiset<Key, Compare, Allocator> >
    {
      typedef std::multiset<Key, Compare, Allocator> Container;

      typedef set_traits<Container>       mutable_traits;
      typedef set_traits<Container const> const_traits;

    public:
      typedef set_algorithms<mutable_traits> mutable_algorithms;
      typedef set_algorithms<const_traits>   const_algorithms;
    };

    // std::map
    template <class Key, class T, class Compare, class Allocator>
    class selector_impl<std::map<Key, T, Compare, Allocator> >
    {
      typedef std::map<Key, T, Compare, Allocator> Container;

      typedef map_traits<Container>       mutable_traits;
      typedef map_traits<Container const> const_traits;

    public:
      typedef map_algorithms<mutable_traits> mutable_algorithms;
      typedef map_algorithms<const_traits>   const_algorithms;
    };

    // std::multimap
    template <class Key, class T, class Compare, class Allocator>
    class selector_impl<std::multimap<Key, T, Compare, Allocator> >
    {
      typedef std::multimap<Key, T, Compare, Allocator> Container;

      typedef map_traits<Container>       mutable_traits;
      typedef map_traits<Container const> const_traits;

    public:
      typedef map_algorithms<mutable_traits> mutable_algorithms;
      typedef map_algorithms<const_traits>   const_algorithms;
    };

    // std::vector
    template <class T, class Allocator>
    class selector_impl<std::vector<T, Allocator> >
    {
      typedef std::vector<T, Allocator> Container;

      typedef default_sequence_traits<Container>       mutable_traits;
      typedef default_sequence_traits<Container const> const_traits;

    public:
      typedef default_algorithms<mutable_traits> mutable_algorithms;
      typedef default_algorithms<const_traits>   const_algorithms;
    };

    // std::deque
    template <class T, class Allocator>
    class selector_impl<std::deque<T, Allocator> >
    {
      typedef std::deque<T, Allocator> Container;

      typedef default_sequence_traits<Container>       mutable_traits;
      typedef default_sequence_traits<Container const> const_traits;

    public:
      typedef default_algorithms<mutable_traits> mutable_algorithms;
      typedef default_algorithms<const_traits>   const_algorithms;
    };

    // std::list
    template <class T, class Allocator>
    class selector_impl<std::list<T, Allocator> >
    {
      typedef std::list<T, Allocator> Container;

      typedef default_sequence_traits<Container>       mutable_traits;
      typedef default_sequence_traits<Container const> const_traits;

    public:
      typedef list_algorithms<mutable_traits> mutable_algorithms;
      typedef list_algorithms<const_traits>   const_algorithms;
    };

    // Iterator ranges
    template <typename Iterator>
    class selector_impl<iterator_range<Iterator> >
    {
      typedef iterator_range<Iterator> Container;

      typedef iterator_range_traits<Container>       mutable_traits;
      typedef iterator_range_traits<Container const> const_traits; // ?

    public:
      typedef default_algorithms<mutable_traits> mutable_algorithms;
      typedef default_algorithms<const_traits>   const_algorithms;
    };

    // Container proxies
    template <typename RawContainer, typename Holder, typename Generator>
    class selector_impl<container_proxy<RawContainer, Holder, Generator> >
    {
      typedef container_proxy<RawContainer, Holder, Generator> Container;

      typedef container_proxy_traits<Container>       mutable_traits;
      typedef container_proxy_traits<Container const> const_traits;

    public:
      typedef default_algorithms<mutable_traits> mutable_algorithms;
      typedef default_algorithms<const_traits>   const_algorithms;
    };
  }

  // Select the right algorithms for each supported kind of container

  // Generic version (mutable containers)
  template<class Container>
  struct algo_selector
    : public detail::selector_impl<Container>::mutable_algorithms
  {
  };

  // Partial specialization for const containers
  template<class Container>
  struct algo_selector<Container const>
    : public detail::selector_impl<Container>::const_algorithms
  {
  };
} } }

#endif // BOOST_PYTHON_INDEXING_ALGO_SELECTOR_HPP
