//=======================================================================
// Copyright 2001 University of Notre Dame.
// Authors: Jeremy G. Siek, Andrew Lumsdaine
//
// This file is part of the Matrix Template Library
//
// You should have received a copy of the License Agreement for the
// Matrix Template Library along with the software; see the file LICENSE.
// If not, contact Office of Research, University of Notre Dame, Notre
// Dame, IN 46556.
//
// Permission to modify the code and to distribute modified code is
// granted, provided the text of this NOTICE is retained, a notice that
// the code was modified is included with the above COPYRIGHT NOTICE and
// with the COPYRIGHT NOTICE in the LICENSE file, and that the LICENSE
// file is distributed with the modified code.
//
// LICENSOR MAKES NO REPRESENTATIONS OR WARRANTIES, EXPRESS OR IMPLIED.
// By way of example, but not limitation, Licensor MAKES NO
// REPRESENTATIONS OR WARRANTIES OF MERCHANTABILITY OR FITNESS FOR ANY
// PARTICULAR PURPOSE OR THAT THE USE OF THE LICENSED SOFTWARE COMPONENTS
// OR DOCUMENTATION WILL NOT INFRINGE ANY PATENTS, COPYRIGHTS, TRADEMARKS
// OR OTHER RIGHTS.
//=======================================================================

#ifndef CONTAINER_TRAITS_HPP
#define CONTAINER_TRAITS_HPP

#include <utility>

template <class C>
class container_traits {
public:
  typedef typename C::size_type size_type;
  typedef typename C::iterator iterator;
  typedef typename C::const_iterator const_iterator;
  typedef typename C::reverse_iterator reverse_iterator;
  typedef typename C::const_reverse_iterator const_reverse_iterator;
  typedef typename C::value_type value_type;
  typedef typename C::reference reference;
};

template <class C>
typename container_traits<C>::size_type
size(const C& c) { return c.size(); }

template <class C>
typename container_traits<C>::const_iterator
begin(const C& c) { return c.begin(); }

template <class C>
typename container_traits<C>::iterator
begin(C& c) { return c.begin(); }

template <class C>
typename container_traits<C>::const_iterator
end(const C& c) { return c.end(); }

template <class C>
typename container_traits<C>::iterator
end(C& c) { return c.end(); }

template <class C>
typename container_traits<C>::const_reverse_iterator
rbegin(const C& c) { return c.rbegin(); }

template <class C>
typename container_traits<C>::reverse_iterator
rbegin(C& c) { return c.rbegin(); }

template <class C>
typename container_traits<C>::const_reverse_iterator
rend(const C& c) { return c.rend(); }

template <class C>
typename container_traits<C>::reverse_iterator
rend(C& c) { return c.rend(); }

template <class C>
std::pair<typename container_traits<C>::const_iterator,
          typename container_traits<C>::const_iterator>
range(const C& c) { return std::make_pair(begin(c), end(c)); }

template <class C>
std::pair<typename container_traits<C>::iterator,
          typename container_traits<C>::iterator>
range(C& c) { return std::make_pair(begin(c), end(c)); }

template <class C>
std::pair<typename container_traits<C>::const_reverse_iterator,
          typename container_traits<C>::const_reverse_iterator>
reverse_range(const C& c) { return std::make_pair(rbegin(c), rend(c)); }

template <class C>
std::pair<typename container_traits<C>::reverse_iterator,
          typename container_traits<C>::reverse_iterator>
reverse_range(C& c) { return std::make_pair(rbegin(c), rend(c)); }


template <class Cin, class Cout>
void copy(const Cin& c1, Cout& c2)
{
  std::copy(begin(c1), end(c1), begin(c2));
}


#endif // CONTAINER_TRAITS_HPP
