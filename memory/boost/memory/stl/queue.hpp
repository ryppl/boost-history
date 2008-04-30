//
//  stl/queue.hpp (*)
//
//  Copyright (c) 2004 - 2008 xushiwei (xushiweizh@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
//  See http://www.boost.org/libs/memory/index.htm for documentation.
//
#ifndef _BOOST_MEMORY_STL_QUEUE_HPP_
#define _BOOST_MEMORY_STL_QUEUE_HPP_

#if !defined(_DEQUE_) && !defined(_DEQUE)
#include <deque> // std::deque
#endif

#if !defined(_FUNCTIONAL_) && !defined(_FUNCTIONAL)
#include <functional> // std::less
#endif

#if !defined(_ALGORITHM_) && !defined(_ALGORITHM)
#include <algorithm> // std::make_heap, etc
#endif

#ifndef _NS_BOOST_BEGIN
#define _NS_BOOST_BEGIN	namespace boost {
#define _NS_BOOST_END	}
#endif

_NS_BOOST_BEGIN

// -------------------------------------------------------------------------

template <class _Tp, 
          class _Sequence = std::deque<_Tp>,
          class _Pred = std::less<_Tp> >
class priority_queue
{
public:
  typedef typename _Sequence::value_type      value_type;
  typedef typename _Sequence::size_type       size_type;
  typedef          _Sequence                  container_type;

  typedef typename _Sequence::reference       reference;
  typedef typename _Sequence::const_reference const_reference;

protected:
  _Sequence m_coll;
  _Pred m_pred;

public:
  priority_queue() {}
  explicit priority_queue(const _Pred& __x) :  m_coll(), m_pred(__x) {}
  priority_queue(const _Pred& __x, const _Sequence& __s) 
    : m_coll(__s), m_pred(__x) 
    { std::make_heap(m_coll.begin(), m_coll.end(), m_pred); }

  template <class _InputIterator>
  priority_queue(_InputIterator __first, _InputIterator __last) 
    : m_coll(__first, __last) { std::make_heap(m_coll.begin(), m_coll.end(), m_pred); }

  template <class _InputIterator>
  priority_queue(_InputIterator __first, 
                 _InputIterator __last, const _Pred& __x)
    : m_coll(__first, __last), m_pred(__x) 
    { std::make_heap(m_coll.begin(), m_coll.end(), m_pred); }

  template <class _InputIterator>
  priority_queue(_InputIterator __first, _InputIterator __last,
                 const _Pred& __x, const _Sequence& __s)
  : m_coll(__s), m_pred(__x)
  {
    m_coll.insert(m_coll.end(), __first, __last);
    std::make_heap(m_coll.begin(), m_coll.end(), m_pred);
  }

  bool empty() const { return m_coll.empty(); }
  size_type size() const { return m_coll.size(); }
  const_reference top() const { return m_coll.front(); }
  void push(const value_type& __x) {
    m_coll.push_back(__x); 
	std::push_heap(m_coll.begin(), m_coll.end(), m_pred);
  }
  void pop() {
    std::pop_heap(m_coll.begin(), m_coll.end(), m_pred);
    m_coll.pop_back();
  }
  void clear() {
	m_coll.clear();
  }
  void swap(priority_queue& o) {
	m_coll.swap(o.m_coll);
	std::swap(m_pred, o.m_pred);
  }
};

// -------------------------------------------------------------------------
// $Log: $

_NS_BOOST_END

#endif /* _BOOST_MEMORY_STL_QUEUE_HPP_ */
