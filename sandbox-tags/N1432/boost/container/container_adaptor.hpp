//=======================================================================
//  (C) Copyright Jeremy Siek 2002. Permission to copy, use,
//  modify, sell and distribute this software is granted provided this
//  copyright notice appears in all copies. This software is provided
//  "as is" without express or implied warranty, and with no claim as
//  to its suitability for any purpose.
//=======================================================================

#ifndef BOOST_CONTAINER_ADAPTOR_HPP
#define BOOST_CONTAINER_ADAPTOR_HPP

// STATUS: under construction (code incomplete and won't compile)

#include <cassert>
#include <utility>
#include <stdexcept>
#include <boost/iterator_adaptors.hpp>

namespace boost {

  template <typename Traits>
  class default_container_policies
  {
    typedef typename Traits::iterator iterator;
    typedef typename Traits::const_iterator const_iterator;
    typedef typename Traits::size_type size_type;
  public:
    template <typename C>
    void destruct(C&) { }

    template <typename C>
    iterator begin(C& c) { return c.c().begin(); }

    template <typename C>
    const_iterator begin(const C& c) const { return c.c().begin(); }
    
    template <typename C>
    iterator end(C& c) { return c.c().end(); }
    
    template <typename C>
    const_iterator end(const C& c) const { return c.c().end(); }

    template <typename C>
    void swap(C& a, C& b) const { return c.c().end(); }

    template <typename C>
    size_type size(const C& c) const { return c.c().size(); }

    template <typename C>
    iterator insert(C& c, const_iterator p, const value_type& t) {
      return c.c().insert(p, t);
    }
    template <typename C>
    iterator insert(C& c, const_iterator p, size_type n, const value_type& t) {
      return c.c().insert(p, n, t);
    }
    template <typename C, typename InIter>
    iterator insert(C& c, const_iterator p, InIter i, InIter j) {
      return c.c().insert(p, i, j);
    }
    template <typename C>
    iterator erase(const_iterator q) { return policies().erase(*this, q); }
    iterator erase(const_iterator q1, const_iterator q2) { 
      return policies().erase(*this, q1, q2);
    }
    void clear() { policies().clear(*this); }

    template <typename C>
    void resize(C& c, size_type n) const { c.c().resize(n); }
  };

  template <typename Base, 
            typename Traits, 
            typename Policies = default_container_policies<Traits> >
  class container_adaptor {
  public:
    container_adaptor(const Base& c, const Policies& p)
      : m_c(c), m_p(p) { }

    // Container requirements (Table 65)
    typedef typename Traits::value_type value_type;
    typedef typename Traits::reference reference;
    typedef typename Traits::const_reference const_reference;
    typedef typename Traits::iterator iterator;
    typedef typename Traits::const_iterator const_iterator;
    typedef typename Traits::difference_type difference_type;
    typedef typename Traits::size_type size_type;

    // pointer and const_pointer not listed in Table 65 of the Std. -JGS
    typedef typename Traits::pointer pointer; 
    typedef typename Traits::const_pointer const_pointer;

    container_adaptor() { }
    
    ~container_adaptor() { policies().destruct(*this); }
    
    iterator begin() { return policies().begin(*this); }
    iterator end() { return policies().end(*this); }
    const_iterator begin() const { return policies().begin(*this); }
    const_iterator end() const { return policies().end(*this); }

    void swap(container_adaptor& other) {
      policies().swap(*this, other);
    }

    size_type size() const { return policies().size(*this); }
    size_type max_size() const { return (size_type)-1; }
    bool empty() const { return policis().size(*this) == 0; }

    // Reversible container requirements (Table 66)
    typedef typename boost::reverse_iterator_generator<iterator>::type
      reverse_iterator;
    typedef typename boost::reverse_iterator_generator<const_iterator>::type
      const_reverse_iterator;

    reverse_iterator rbegin() 
      { return reverse_iterator(end()); }
    reverse_iterator rend() 
      { return reverse_iterator(begin()); }
    const_reverse_iterator rbegin() const 
      { return const_reverse_iterator(end()); }
    const_reverse_iterator rend() const 
      { return const_reverse_iterator(begin()); }

    // Sequence requirements (Table 67)
    container_adaptor(size_type n, const value_type& t)
      : m_c(n, t) { }

    template <typename InIter>
    container_adaptor(InIter i, InIter j)
      : m_c(i, j) { }

    iterator insert(const_iterator p, const value_type& t) {
      return policies().insert(*this, p, t);
    }
    iterator insert(const_iterator p, size_type n, const value_type& t) {
      return policies().insert(*this, p, n, t);
    }
    template <typename InIter>
    iterator insert(const_iterator p, InIter i, InIter j) {
      return policies().insert(*this, p, i, j);
    }
    iterator erase(const_iterator q) { return policies().erase(*this, q); }
    iterator erase(const_iterator q1, const_iterator q2) { 
      return policies().erase(*this, q1, q2);
    }
    void clear() { policies().clear(*this); }

    // Optional sequence operations (Table 68)
    reference front() { return *begin(); }
    const_reference front() const { return *begin(); }

    reference back() { return *rbegin(); }
    const_reference back() const { return *rbegin(); }

    void push_front(const value_type& x) { insert(begin(), x); }
    void push_back(const value_type& x) { insert(end(), x); }
    void pop_front() { erase(begin()); }
    void pop_back() { erase(--end()); }

    const_reference operator[] (size_type i) const { 
      assert(i < this->size());
      return begin()[i]; 
    }
    reference operator[] (size_type i) { 
      assert(i < this->size());      
      return begin()[i]; 
    }
    const_reference at(size_type i) const { 
      if (i >= this->size())
	throw std::out_of_range();
      return begin()[i]; 
    }
    reference at(size_type i) { 
      if (i >= this->size())
	throw std::out_of_range();
      return begin()[i]; 
    }
    
    // Not in any table?
    void resize(size_type n) { policies().resize(*this, n); }
  protected:
    Base& c() { return m_c; }
    const Base& c() const { return m_c; }
    
    Policies& policies() { return m_p; }
    const Policies& policies() const { return m_p; }

    Base m_c;
    Policies m_p;
  };


  // Container requirements (Table 65)  
  template <typename B, typename Tr, typename P>
  bool operator==(const container_adaptor<B, Tr, P>& a,
                  const container_adaptor<B, Tr, P>& b)
  {
    return a.size() == b.size() && std::equal(a.begin(), a.end(), b.begin());
  }

  template <typename B, typename Tr, typename P>
  bool operator!=(const container_adaptor<B, Tr, P>& a,
                  const container_adaptor<B, Tr, P>& b)
  {
    return !(a == b);
  }
  
  template <typename B, typename Tr, typename P>
  bool operator<(const container_adaptor<B, Tr, P>& a,
                 const container_adaptor<B, Tr, P>& b)
  {
    return std::lexicographical_compare(a.begin(), a.end(),
                                        b.begin(), b.end());
  }
  
  template <typename B, typename Tr, typename P>
  bool operator>(const container_adaptor<B, Tr, P>& a,
                 const container_adaptor<B, Tr, P>& b)
  {
    return b < a;
  }

  template <typename B, typename Tr, typename P>
  bool operator<=(const container_adaptor<B, Tr, P>& a,
                  const container_adaptor<B, Tr, P>& b)
  {
    return !(a > b);
  }

  template <typename B, typename Tr, typename P>
  bool operator>=(const container_adaptor<B, Tr, P>& a,
                  const container_adaptor<B, Tr, P>& b)
  {
    return !(a < b);
  }
  
  template <typename Traits>
  class default_assoc_container_policies
    : public default_container_policies<Traits>
  {
  public:
    
    template <typename C>
    std::pair<const_iterator, bool>
    insert(C& c, const value_type& t) {
      return c.c().insert(t);
    }
    template <typename C>    
    const_iterator insert(C& c, const value_type& t) {
      return c.c().insert(t);
    }
    template <typename C>
    const_iterator insert(C& c, const_iterator p, const value_type& t) {
      return c.c().insert(p, t);
    }
    template <typename C, typename InIter>
    void insert(C& c, InIter i, InIter j) { c.c().insert(i, j); } 

    template <typename C>
    size_type erase(C& c, key_type k) { return c.c().erase(k); }

    template <typename C>
    void erase(C& c, const_iterator q) { c.c().erase(q); }

    template <typename C>
    void erase(C& c, const_iterator q1, const_iterator q2) { 
      c.c().erase(q1, q2); }

    template <typename C>
    iterator find(C& c, key_type k) { return c.c().find(k); }

    template <typename C>
    const_iterator find(C& c, key_type k) const { return c.c().find(k); }

    template <typename C>
    size_type count(C& c, key_type k) const { return c.count(k); }

    template <typename C>
    iterator lower_bound(C& c, key_type k)
    { return c.c().lower_bound(k); }

    template <typename C>
    const_iterator lower_bound(C& c, key_type k) const
    { return c.c().lower_bound(k); }

    template <typename C>
    iterator upper_bound(C& c, key_type k) { return c.c().upper_bound(k); }

    template <typename C>
    const_iterator upper_bound(C& c, key_type k) const 
    { return c.c().upper_bound(k); }

    template <typename C>
    pair<iterator, iterator> equal_range(C& c, key_type k) 
    { return c.c().equal_range(k); }

    template <typename C>
    pair<const_iterator, const_iterator> equal_range(key_type k) const
    { return c.c().equal_range(k); }
  };

  template <typename Base, 
            typename Traits, 
            typename Policies = default_assoc_container_policies<Traits> >
  class associative_container_adaptor
    // Not sure this inheritance will work. -JGS
    : public container_adaptor<Base, Traits, Policies>
  {
    typedef container_adaptor<Base, Traits, Policies> Super;
  public:
    typedef typename Super::const_iterator const_iterator;
    typedef typename Super::size_type size_type;
    typedef typename Super::value_type value_type;

    associative_container_adaptor(const Base& c, const Policies& p)
      : Super(c, p) { }

    // Associative container requirements (Table 69)
    typedef typename Traits::key_type key_type;
    typedef typename Traits::key_compare key_compare;
    typedef typename Traits::value_compare value_compare;
    
    associative_container_adaptor() { }
    
    explicit associative_container_adaptor(const key_compare& c) { }

    template <typename InIter>
    associative_container_adaptor(InIter i, InIter j,
                                  const key_compare& c) { }    

    template <typename InIter>
    associative_container_adaptor(InIter i, InIter j) { }
    
    key_compare key_comp() { }
    value_compare value_comp() { }
    
    std::pair<const_iterator, bool> insert(const value_type& t) 
    { return policies().insert(*this, t); }

    const_iterator insert(const value_type& t)
    { return policies().insert(*this, t); }

    const_iterator insert(const_iterator p, const value_type& t)
    { return policies().insert(*this, p, t); }

    template <typename InIter>
    void insert(InIter i, InIter j)
    { policies().insert(*this, i, j); }

    size_type erase(key_type k) { return policies().erase(*this, k); }

    void erase(const_iterator q) { policies().erase(*this, q); }

    void erase(const_iterator q1, const_iterator q2)
    { policies().erase(*this, q1, q2); }

    iterator find(key_type k) { return policies().find(*this, k); }
    const_iterator find(key_type k) const { return policies().find(*this, k); }
    size_type count(key_type k) const { return policies().count(*this, k); }

    iterator lower_bound(key_type k)
    { return policies().lower_bound(*his, k); }
    const_iterator lower_bound(key_type k) const 
    { return policies().lower_bound(*his, k); }

    iterator upper_bound(key_type k)
    { return policies().upper_bound(*his, k); }
    const_iterator upper_bound(key_type k) const
    { return policies().upper_bound(*his, k); }

    pair<iterator, iterator> equal_range(key_type k)
    { return policies().equal_range(*his, k); }
    pair<const_iterator, const_iterator> equal_range(key_type k) const
    { return policies().equal_range(*his, k); }
  };
  
} // namespace boost

#endif // BOOST_CONTAINER_ADAPTOR_HPP

