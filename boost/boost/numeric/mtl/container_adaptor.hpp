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

#ifndef MTL_CONTAINER_ADAPTOR_HPP
#define MTL_CONTAINER_ADAPTOR_HPP

#include <boost/iterator_adaptors.hpp>
#include <mtl/range.hpp>
#include <mtl/assert.hpp>
#include <mtl/pair.hpp>

namespace mtl {

  template <class Traits>
  class default_container_policies
  {
    typedef typename Traits::iterator iterator;
    typedef typename Traits::const_iterator const_iterator;
    typedef typename Traits::size_type size_type;
  public:
    template <class C>
    iterator begin(C& c) { return c.begin(); }

    template <class C>
    const_iterator begin(const C& c) const { return c.begin(); }
    
    template <class C>
    iterator end(C& c) { return c.end(); }
    
    template <class C>
    const_iterator end(const C& c) const { return c.end(); }

    template <class C>
    size_type size(const C& c) const { return c.size(); }

    template <class C>
    void resize(C& c, size_type n) const { c.resize(n); }
  };

  template <class C, 
            class Traits, 
            class Policies = default_container_policies<Traits> >
  class container_adaptor {
  public:
    typedef typename Traits::value_type value_type;
    typedef typename Traits::size_type size_type;
    typedef typename Traits::reference reference;
    typedef typename Traits::const_reference const_reference;
    typedef typename Traits::difference_type difference_type;
    typedef typename Traits::pointer pointer; 
    typedef typename Traits::const_pointer const_pointer;
    typedef typename Traits::iterator iterator;
    typedef typename Traits::const_iterator const_iterator;
  public:
    typedef typename boost::reverse_iterator_generator<iterator>::type
      reverse_iterator;
    typedef typename boost::reverse_iterator_generator<const_iterator>::type
      const_reverse_iterator;

    container_adaptor() { }

    container_adaptor(const C& c, const Policies& p)
      : m_c(c), m_p(p) { }
    
    template <class X, class Y>
    container_adaptor(const pair<X,Y>& args)
      : m_c(args) { }

    iterator begin() { return policies().begin(c()); }
    iterator end() { return policies().end(c()); }
    const_iterator begin() const { return policies().begin(c()); }
    const_iterator end() const { return policies().end(c()); }

    reverse_iterator rbegin() 
      { return reverse_iterator(end()); }
    reverse_iterator rend() 
      { return reverse_iterator(begin()); }
    const_reverse_iterator rbegin() const 
      { return const_reverse_iterator(end()); }
    const_reverse_iterator rend() const 
      { return const_reverse_iterator(begin()); }

    reference front() { return *begin(); }
    const_reference front() const { return *begin(); }

    reference back() { return *rbegin(); }
    const_reference back() const { return *rbegin(); }

    const_reference operator[] (size_type i) const { 
      MTL_ASSERT_RANGE(i < this->size());
      return begin()[i]; 
    }
    reference operator[] (size_type i) { 
      MTL_ASSERT_RANGE(i < this->size());      
      return begin()[i]; 
    }

    void resize(size_type n) { policies().resize(c(), n); }
    size_type size() const { return policies().size(c()); }

  protected:
    C& c() { return m_c; }
    const C& c() const { return m_c; }
    
    Policies& policies() { return m_p; }
    const Policies& policies() const { return m_p; }

    C m_c;
    Policies m_p;
  };

} // namespace mtl

#endif // MTL_CONTAINER_ADAPTOR_HPP
