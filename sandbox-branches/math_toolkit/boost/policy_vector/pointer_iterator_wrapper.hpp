/* The following code declares class wrapper for pointer iterator
 *
 * (C) Copyright Herve Bronnimann, 2002.
 * Permission to copy, use, modify, sell and distribute this software
 * is granted provided this copyright notice appears in all copies.
 * This software is provided "as is" without express or implied
 * warranty, and with no claim as to its suitability for any purpose.
 *
 * 5  May 2002 - Separate file (Herve)
 */

#ifndef BOOST_POLICY_VECTOR_POINTER_ITERATOR_WRAPPER_HPP
#define BOOST_POLICY_VECTOR_POINTER_ITERATOR_WRAPPER_HPP

template <class Pointer, class V, bool Check>
struct pointer_iterator_wrapper_base
{
  pointer_iterator_wrapper_base(V*) {}
  Pointer base() const { return Pointer(); }
  Pointer check(Pointer ptr) const {}
};

template <class Pointer, class V>
struct pointer_iterator_wrapper_base<Pointer, V, true>
{
  pointer_iterator_wrapper_base(V* v) : m_v(v) {}
  Pointer base() const { return Pointer(&m_v->front()); }
  Pointer check(Pointer ptr) const {
    m_v->range_check(ptr-base(), m_v->size());
    return ptr;
  }
private:
  V* m_v; 
};

template <class V, class T, class Pointer, class Reference, bool Check>
struct pointer_iterator_wrapper : pointer_iterator_wrapper_base<Pointer,V,Check>
{
  typedef T                                      value_type;
  typedef Pointer                                pointer;
  typedef Reference                              reference;
  typedef std::ptrdiff_t                         difference_type;
  typedef std::random_access_iterator_tag        iterator_category;
protected:
  typedef pointer_iterator_wrapper_base<Pointer,V,Check>  base_type;
  typedef pointer_iterator_wrapper<V,T,Pointer,Reference,Check> self_type;
  typedef difference_type                        index_t;
public:
  pointer_iterator_wrapper() : base_type(0), m_ptr(0) {}
  pointer_iterator_wrapper(V* v, index_t n) : base_type(v), m_ptr(base()+n) {}
  pointer_iterator_wrapper(V* v, Pointer ptr) : base_type(v), m_ptr(ptr) {}
  pointer_iterator_wrapper(self_type const& w) : base_type(w), m_ptr(w.m_ptr) {}
  // operators
  Reference operator*()  const {
    return *base_type::check(m_ptr);
  }
  Pointer   operator->() const {
    return base_type::check(m_ptr);
  }
  Reference operator[](std::size_t n) const {
    return base_type::check(m_ptr[n]);
  }
  self_type& operator++() {
    ++m_ptr; return *this;
  }
  self_type operator++(int) {
    self_type old(*this); ++m_ptr; return *old;
  }
  self_type& operator--() {
    --m_ptr; return *this;
  }
  self_type& operator--(int) {
    self_type old(*this); --m_ptr; return *old;
  }
  self_type& operator+=(difference_type n) {
    m_ptr += n; return *this;
  }
  self_type& operator-=(difference_type n) {
    m_ptr -= n; return *this;
  }
#if defined(__GNUC__) && (__GNUC__) < 3
  // GCC 2.95 has a bug in its Koenig lookup
  self_type operator+(difference_type const& rhs) const {
    return self_type(*this) += rhs;
  }
  self_type operator-(difference_type const& rhs) const {
    return self_type(*this) -= rhs;
  }
  difference_type operator-(self_type const& rhs) const {
    // must belong to same vector: what do we do otherwise?
    return m_ptr - rhs.m_ptr;
  }
  bool operator==(self_type  const& rhs) const {
    // must belong to same vector: what do we do otherwise?
    return m_ptr == rhs.m_ptr;
  }
  bool operator!=(self_type const& rhs) const {
    // must belong to same vector: what do we do otherwise?
    return !(m_ptr == rhs.m_ptr);
  }
  bool operator<(self_type const& rhs) const {
    // must belong to same vector: what do we do otherwise?
    return m_ptr < rhs.m_ptr;
  }
  bool operator<=(self_type const& rhs) const {
    // must belong to same vector: what do we do otherwise?
    return !(rhs.m_ptr < m_ptr);
  }
  bool operator>(self_type const& rhs) const {
    // must belong to same vector: what do we do otherwise?
    return rhs.m_ptr < m_ptr;
  }
  bool operator>=(self_type const& rhs) const {
    // must belong to same vector: what do we do otherwise?
    return !(m_ptr < rhs.m_ptr);
  }
#else
  friend self_type operator+(self_type lhs, difference_type const& rhs) {
    return lhs += rhs;
  }
  friend self_type operator+(difference_type const& lhs, self_type rhs) {
    return rhs += lhs;
  }
  friend self_type operator-(self_type lhs, difference_type const& rhs) {
    return lhs -= rhs;
  }
  friend difference_type operator-(self_type const& lhs, self_type const& rhs) {
    // must belong to same vector: what do we do otherwise?
    return lhs.m_ptr - rhs.m_ptr;
  }
  friend bool operator==(self_type const& lhs, self_type  const& rhs) {
    // must belong to same vector: what do we do otherwise?
    return lhs.m_ptr == rhs.m_ptr;
  }
  friend bool operator!=(self_type const& lhs, self_type const& rhs) {
    // must belong to same vector: what do we do otherwise?
    return !(lhs.m_ptr == rhs.m_ptr);
  }
  friend bool operator<(self_type const& lhs, self_type const& rhs) {
    // must belong to same vector: what do we do otherwise?
    return lhs.m_ptr < rhs.m_ptr;
  }
  friend bool operator<=(self_type const& lhs, self_type const& rhs) {
    // must belong to same vector: what do we do otherwise?
    return !(rhs.m_ptr < lhs.m_ptr);
  }
  friend bool operator>(self_type const& lhs, self_type const& rhs) {
    // must belong to same vector: what do we do otherwise?
    return rhs.m_ptr < lhs.m_ptr;
  }
  friend bool operator>=(self_type const& lhs, self_type const& rhs) {
    // must belong to same vector: what do we do otherwise?
    return !(lhs.m_ptr < rhs.m_ptr);
  }
#endif
private:
  Pointer m_ptr;
};

#endif /* BOOST_POLICY_VECTOR_POINTER_ITERATOR_WRAPPER_HPP */

