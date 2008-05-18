// Copyright (c) 2003, 2008 Raoul M. Gough
//
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy
// at http://www.boost.org/LICENSE_1_0.txt)
//
// Header file deque.hpp
//
// Indexing algorithms support for std::deque instances
//
// History
// =======
// 2003-10-28   rmg     File creation from algo_selector.hpp
// 2008-05-18   rmg     Rename indexing subdirectory to indexing_v2
//
// $Id$
//

#ifndef BOOST_PYTHON_INDEXING_DEQUE_HPP
#define BOOST_PYTHON_INDEXING_DEQUE_HPP

#include <boost/python/suite/indexing_v2/container_traits.hpp>
#include <boost/python/suite/indexing_v2/container_suite.hpp>
#include <boost/python/suite/indexing_v2/algorithms.hpp>
#include <deque>

namespace boost { namespace python { namespace indexing_v2 {
#if !defined(BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION)
  namespace detail {
    ///////////////////////////////////////////////////////////////////////
    // algorithms support for std::deque instances
    ///////////////////////////////////////////////////////////////////////

    template <class T, class Allocator>
    class algorithms_selector<std::deque<T, Allocator> >
    {
      typedef std::deque<T, Allocator> Container;

      typedef random_access_sequence_traits<Container>       mutable_traits;
      typedef random_access_sequence_traits<Container const> const_traits;

    public:
      typedef default_algorithms<mutable_traits> mutable_algorithms;
      typedef default_algorithms<const_traits>   const_algorithms;
    };
  }
#endif

  template<
    class Container,
    method_set_type MethodMask = all_methods,
    class Traits = random_access_sequence_traits<Container>
  >
  struct deque_suite
    : container_suite<Container, MethodMask, default_algorithms<Traits> >
  {
  };

} } }

#endif // BOOST_PYTHON_INDEXING_DEQUE_HPP
