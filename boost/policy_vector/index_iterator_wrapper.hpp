/* The following code declares class wrapper for index iterator
 *
 * (C) Copyright Herve Bronnimann, 2002.
 * Permission to copy, use, modify, sell and distribute this software
 * is granted provided this copyright notice appears in all copies.
 * This software is provided "as is" without express or implied
 * warranty, and with no claim as to its suitability for any purpose.
 *
 * 5  May 2002 - Separate file (Herve)
 */

#ifndef BOOST_POLICY_VECTOR_INDEX_ITERATOR_WRAPPER_HPP
#define BOOST_POLICY_VECTOR_INDEX_ITERATOR_WRAPPER_HPP

template <class V, class T, class Pointer, class Reference, bool Check>
struct index_iterator_wrapper
{
  typedef T                                      value_type;
  typedef Pointer                                pointer;
  typedef Reference                              reference;
  typedef std::ptrdiff_t                         difference_type;
  typedef std::random_access_iterator_tag        iterator_category;
protected:
  typedef index_iterator_wrapper<V,T,Pointer,Reference,Check> self_type;
  typedef std::size_t                            size_type;
public:
  index_iterator_wrapper() : _M_v(0), _M_n(0) {}
  index_iterator_wrapper(V* v, size_type n) : _M_v(v), _M_n(n) {}
  index_iterator_wrapper(self_type const& w)
    : _M_v(w._M_v), _M_n(w._M_n) {}
  // operators
  Reference operator*()  const { check(); return *address(); }
  Pointer   operator->() const { check(); return address(); }
  Reference operator[](std::size_t n) const { check(n); return address()[n]; }
  self_type& operator++() {
    ++_M_n; return *this;
  }
  self_type operator++(int) {
    self_type old(*this); ++_M_n; return *old;
  }
  self_type& operator--() {
    --_M_n; return *this;
  }
  self_type& operator--(int) {
    self_type old(*this); --_M_n; return *old;
  }
  self_type& operator+=(difference_type n) {
    _M_n += n; return *this;
  }
  self_type& operator-=(difference_type n) {
    _M_n -= n; return *this;
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
    return _M_n - rhs._M_n;
  }
  bool operator==(self_type  const& rhs) const {
    // must belong to same vector: what do we do otherwise?
    return _M_n == rhs._M_n;
  }
  bool operator!=(self_type const& rhs) const {
    // must belong to same vector: what do we do otherwise?
    return !(_M_n == rhs._M_n);
  }
  bool operator<(self_type const& rhs) const {
    // must belong to same vector: what do we do otherwise?
    return _M_n < rhs._M_n;
  }
  bool operator<=(self_type const& rhs) const {
    // must belong to same vector: what do we do otherwise?
    return !(rhs._M_n < _M_n);
  }
  bool operator>(self_type const& rhs) const {
    // must belong to same vector: what do we do otherwise?
    return rhs._M_n < _M_n;
  }
  bool operator>=(self_type const& rhs) const {
    // must belong to same vector: what do we do otherwise?
    return !(_M_n < rhs._M_n);
  }
#else
  friend inline self_type
  operator+(self_type lhs, difference_type rhs) {
    return lhs += rhs;
  }
  friend inline self_type
  operator+(difference_type lhs, self_type rhs) {
    return rhs += lhs;
  }
  friend inline self_type
  operator-(self_type const& lhs, difference_type const& rhs) {
    return lhs -= rhs;
  }
  friend inline difference_type
  operator-(self_type const& lhs, self_type const& rhs) {
    // must belong to same vector: what do we do otherwise?
    return lhs._M_n - rhs._M_n;
  }
  friend inline bool 
  operator==(self_type const& lhs, self_type const& rhs) {
    // must belong to same vector: what do we do otherwise?
    return lhs._M_n == rhs._M_n;
  }
  friend inline bool 
  operator!=(self_type const& lhs, self_type const& rhs) {
    // must belong to same vector: what do we do otherwise?
    return !(lhs._M_n == rhs._M_n);
  }
  friend inline bool 
  operator<(self_type const& lhs, self_type const& rhs) {
    // must belong to same vector: what do we do otherwise?
    return lhs._M_n < rhs._M_n;
  }
  friend inline bool 
  operator<=(self_type const& lhs, self_type const& rhs) {
    // must belong to same vector: what do we do otherwise?
    return !(rhs._M_n < lhs._M_n);
  }
  friend inline bool 
  operator>(self_type const& lhs, self_type const& rhs) {
    // must belong to same vector: what do we do otherwise?
    return rhs._M_n < lhs._M_n;
  }
  friend inline bool 
  operator>=(self_type const& lhs, self_type const& rhs) {
    // must belong to same vector: what do we do otherwise?
    return !(lhs._M_n < rhs._M_n);
  }
#endif
protected:
  Pointer address() const { return (&_M_v->front()) + _M_n; }
  // Could use partial specialization here, but any decent compiler
  // should optimize the test since Check is known at compile time
  // Partial specialization would be mandated only if the statements did
  // not compile for Check=false; and an alternate implementation was
  // needed
  void check() const {
    if (Check) _M_v->range_check(_M_n, _M_v->size());
  }
  void check(difference_type n) const {
    if (Check) _M_v->range_check(_M_n+n, _M_v->size());
  }
private:
  V* _M_v; 
  size_type _M_n;
};

#endif /* BOOST_POLICY_VECTOR_INDEX_ITERATOR_WRAPPER_HPP */

