/* Used in Boost.IndexedSet tests.
 *
 * Copyright Joaqu�n M L�pez Mu�oz 2003. Use, modification, and distribution
 * are subject to the Boost Software License, Version 1.0. (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/libs/indexed_set for library home page.
 */

#ifndef BOOST_INDEXED_SET_TEST_PAIR_OF_INTS_HPP
#define BOOST_INDEXED_SET_TEST_PAIR_OF_INTS_HPP

#include <boost/config.hpp> /* keep it first to prevent some nasty warnings in MSVC */
#include <utility>

typedef std::pair<int,int> pair_of_ints;

inline void increment_first(pair_of_ints& p)
{
  ++p.first;
}

inline void increment_second(pair_of_ints& p)
{
  ++p.second;
}

inline void increment_int(int& x)
{
  ++x;
}

#endif
