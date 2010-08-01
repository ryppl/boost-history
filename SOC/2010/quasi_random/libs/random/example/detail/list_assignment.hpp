// Copyright Justinas Vygintas Daugmaudis, 2010.
// Use, modification and distribution is subject to the
// Boost Software License, Version 1.0. (See accompanying
// file LICENSE-1.0 or http://www.boost.org/LICENSE-1.0)

#ifndef HEADER_DETAIL_LIST_ASSIGNMENT_HPP_INCLUDED
#define HEADER_DETAIL_LIST_ASSIGNMENT_HPP_INCLUDED

namespace detail {

// Forward declaration
template<class T, class ForwardIterator>
class scalar_or_list_assignment;


template<class T, typename ForwardIterator>
class list_assignment
{
private:
  typedef list_assignment<T, ForwardIterator>
      self_type;
  typedef scalar_or_list_assignment<T, ForwardIterator>
      switch_type;

  ForwardIterator m_iterator_;

public:
  explicit list_assignment(switch_type& rhs);
  explicit list_assignment(ForwardIterator pos)
    : m_iterator_(pos)
  {}

  inline self_type operator,(T value)
  {
      *m_iterator_ = value;
      return self_type(++m_iterator_);
  }
};


template<class T, class ForwardIterator>
class scalar_or_list_assignment
{
private:
  friend class list_assignment<T, ForwardIterator>;

  typedef scalar_or_list_assignment<T, ForwardIterator>
      self_t;
  typedef list_assignment<T, ForwardIterator>
      assign_from_list;

public:
  explicit scalar_or_list_assignment(ForwardIterator first, ForwardIterator last, T value)
      : m_first(first)
      , m_last(last)
      , m_value(value)
  {
      // Immediately initialize the first element
      // and move iterator position forward.
      *m_first = value; ++m_first;
  }

  scalar_or_list_assignment(const self_t& rhs)
      : m_first(rhs.m_first)
      , m_last(rhs.m_last)
      , m_value(rhs.m_value)
  {
      // This is, admitedly, not very pretty, but quite necessary
      // for optimization purposes; initialization would be O(n^2)
      // instead of O(n) otherwise.
      const_cast<self_t *>(&rhs)->disable();
  }

  ~scalar_or_list_assignment()
  {
      std::fill(m_first, m_last, m_value);
  }

  // Continues initialization of [first + 1, last)
  // by consecutive assignment of given scalar values
  // to the underlying data structure.
  assign_from_list operator,(T value)
  {
      *m_first = value; ++m_first;
      return assign_from_list(*this);
  }

private:
  void disable() {
      m_first = m_last;
  }

private:
  ForwardIterator m_first;
  ForwardIterator m_last;
  T m_value;
};


//=============================================================================
// list_assignment c-tor implementation

template<class T, typename Iterator>
inline list_assignment<T, Iterator>::list_assignment(scalar_or_list_assignment<T, Iterator>& rhs)
   : m_iterator_(rhs.m_first)
{
  rhs.disable();
}

} // namespace detail

#endif // HEADER_DETAIL_LIST_ASSIGNMENT_HPP_INCLUDED
