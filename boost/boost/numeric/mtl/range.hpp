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

#ifndef RANGE_HPP
#define RANGE_HPP

#include <iterator>
#include <utility>

template <class A, class B>
class iter_range
{
public:
  iter_range(const A& a, const B& b) : first(a), second(b) { }
  iter_range& operator++() { ++first; return *this; }

  typename std::iterator_traits<A>::reference
  operator*() const { return *first; }

  typename std::iterator_traits<A>::reference
  operator[](std::ptrdiff_t i) const { return first[i]; }

//private:
  A first;
  B second;
};

template <class A, class B>
bool empty(const iter_range<A,B>& r) { return r.first == r.second; }

template <class Range>
struct range_traits {
  typedef typename Range::value_type value_type;
};

template <class A, class B>
struct range_traits< iter_range<A,B> > {
  typedef typename std::iterator_traits<A>::value_type value_type;
};

#if 0
template <class Container>
iter_range<typename Container::const_iterator,
           typename Container::const_iterator>
make_range(const Container& c)
{  
  typedef iter_range<typename Container::const_iterator,
    typename Container::const_iterator> Range;
 return Range(c.begin(), c.end());
}
#endif

#endif // RANGE_HPP
