//  boost/rank_list/detail/rl_begin_end.hpp

//  Copyright (c) Universidad de Alcala 2006
//  Use, modification, and distribution is subject to the Boost Software
//  License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See library home page at http://www.boost.org/libs/rank_list

/*
  detail/rl_begin_end.hpp
  -----------------------

  Methods that provide all kinds of iterators referring the
  beginning or the end of the sequence:

  begin(): get normal iterator referring the beginnig (O(1))
  begin()_const_: idem but const_iterator (O(1))
  end(): get normal iterator referring the ending (O(1))
  end()_const_: idem but const_iterator (O(1))
  rbegin(): reverse it. referring rev. beginnig (O(1))
  rbegin()_const_: idem but const_reverse_iterator (O(1))
  rend(): reverse it. referring rev. ending (O(1))
  rend()_const_: idem but const_reverse_iterator (O(1))
  
  See accompanying documentation for more details.

                                    November, 2006
                                    Martin Knoblauch Revuelta
                                    Universidad de Alcala (Spain)
                                    martin at aut.uah.es
*/

#ifndef _BOOST_RANK_LIST_BEGIN_END_HPP_
#define _BOOST_RANK_LIST_BEGIN_END_HPP_

#ifndef _BOOST_RANK_LIST_HPP_
#error "Don't include this file. Include rank_list.hpp instead."
#endif

namespace boost
{
  namespace rank_lists
  {
    using namespace detail;

//////////////////////////////////////////////////////////////////

// ---------------------- PUBLIC INTERFACE -----------------------

// begin(): return an iterator pointing to the beginnig
// of the sequence
//
// Complexity: O(1)

template<class T,class A,class W,class P>
inline
  typename rank_list<T,A,W,P>::iterator
  rank_list<T,A,W,P>::begin ()
{
  return iterator(node_t::m_next);
}

// begin()_const_: return an const_iterator (can be moved,
// but can't modify the referenced data) pointing to the
// beginnig of the sequence
//
// Complexity: O(1)

template<class T,class A,class W,class P>
inline
  typename rank_list<T,A,W,P>::const_iterator
  rank_list<T,A,W,P>::begin () const
{
  return const_iterator(node_t::m_next);
}

// end(): return an iterator pointing to the end of the
// sequence (a dummy position after the last element)
//
// Complexity: O(1)

template<class T,class A,class W,class P>
inline
  typename rank_list<T,A,W,P>::iterator
  rank_list<T,A,W,P>::end ()
{
  return iterator(dummy());
}

// end()_const_: return an const_iterator (can be moved,
// but can't modify the referenced data) pointing to the
// end of the sequence (a dummy position after the last
// element)
//
// Complexity: O(1)

template<class T,class A,class W,class P>
inline
  typename rank_list<T,A,W,P>::const_iterator
  rank_list<T,A,W,P>::end () const
{
  return const_iterator(dummy());
}

// rbegin(): return a reverse iterator pointing to the
// beginnig of the reverse sequence (the last element)
//
// Complexity: O(1)

template<class T,class A,class W,class P>
inline
  typename rank_list<T,A,W,P>::reverse_iterator
  rank_list<T,A,W,P>::rbegin ()
{
  return reverse_iterator(node_t::m_prev);
}

// rbegin()_const_: return a const reverse iterator
// pointing to the beginnig of the reverse sequence
// (the last element)
//
// Complexity: O(1)

template<class T,class A,class W,class P>
inline
  typename rank_list<T,A,W,P>::const_reverse_iterator
  rank_list<T,A,W,P>::rbegin () const
{
  return const_reverse_iterator(node_t::m_prev);
}

// rend(): return a reverse iterator pointing to the end of
// the reverse sequence (a dummy position before the first
// element)
//
// Complexity: O(1)

template<class T,class A,class W,class P>
inline
  typename rank_list<T,A,W,P>::reverse_iterator
  rank_list<T,A,W,P>::rend ()
{
  return reverse_iterator(dummy());
}

// rend()_const_: return a reverse iterator pointing to the
// end of the reverse sequence (a dummy position before the
// first element)
//
// Complexity: O(1)

template<class T,class A,class W,class P>
inline
  typename rank_list<T,A,W,P>::const_reverse_iterator
  rank_list<T,A,W,P>::rend () const
{
  return const_reverse_iterator(dummy());
}

//////////////////////////////////////////////////////////////////

  }  // namespace rank_lists
}  // namespace boost

#endif

