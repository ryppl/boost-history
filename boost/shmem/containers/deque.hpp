/*
 *
 * Copyright (c) 1994
 * Hewlett-Packard Company
 *
 * Permission to use, std::copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.  Hewlett-Packard Company makes no
 * representations about the suitability of this software for any
 * purpose.  It is provided "as is" without express or implied warranty.
 *
 *
 * Copyright (c) 1996
 * Silicon Graphics Computer Systems, Inc.
 *
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.  Silicon Graphics makes no
 * representations about the suitability of this software for any
 * purpose.  It is provided "as is" without express or implied warranty.
 *
 */
//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Ion Gaztañaga 2005. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/shmem for documentation.
//
//////////////////////////////////////////////////////////////////////////////
//
// This file comes from SGI's stl_deque.h and stl_uninitialized.h files. 
// Modified by Ion Gaztañaga 2005.
// Renaming, isolating and porting to generic algorithms. Pointer typedef 
// set to allocator::pointer to allow placing it in shared memory.
//
///////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_SHMEM_DEQUE_HPP
#define BOOST_SHMEM_DEQUE_HPP

#if (defined _MSC_VER) && (_MSC_VER >= 1200)
#  pragma once
#endif

#ifdef max
#undef max
#endif

#include <boost/shmem/detail/workaround.hpp>
#include <boost/shmem/detail/config_begin.hpp>
#include <boost/shmem/detail/utilities.hpp>
#include <boost/shmem/containers/container_fwd.hpp>
#include <boost/iterator.hpp>
#include <boost/iterator/reverse_iterator.hpp>
#include <cstddef>
#include <iterator>
#include <memory>
#include <algorithm>
#include <stdexcept>
#include <boost/detail/no_exceptions_support.hpp>


namespace boost {

namespace shmem {

template <class T, class Alloc>
class deque;

// Note: this function is simply a kludge to work around several compilers'
//  bugs in handling constant expressions.
inline size_t deque_buf_size(size_t size) 
   {  return size < 512 ? size_t(512 / size) : size_t(1);  }

// Deque base class.  It has two purposes.  First, its constructor
//  and destructor allocate (but don't initialize) storage.  This makes
//  exception safety easier.
template <class T, class Alloc>
class deque_base
   : public boost::detail::allocator::
               rebind_to<Alloc, T >::type,
     public boost::detail::allocator::
               rebind_to<Alloc, typename Alloc::pointer>::type
{
 public:
   typedef typename Alloc::value_type           val_alloc_val;
   typedef typename Alloc::pointer              val_alloc_ptr;
   typedef typename Alloc::const_pointer        val_alloc_cptr;
   typedef typename Alloc::reference            val_alloc_ref;
   typedef typename Alloc::const_reference      val_alloc_cref;
   typedef typename Alloc::value_type           val_alloc_diff;
   typedef typename boost::detail::allocator::
      rebind_to<Alloc, val_alloc_ptr>::type     ptr_alloc;
   typedef typename ptr_alloc::value_type       ptr_alloc_val;
   typedef typename ptr_alloc::pointer          ptr_alloc_ptr;
   typedef typename ptr_alloc::const_pointer    ptr_alloc_cptr;
   typedef typename ptr_alloc::reference        ptr_alloc_ref;
   typedef typename ptr_alloc::const_reference  ptr_alloc_cref;

   typedef typename boost::detail::allocator::
            rebind_to<Alloc, T >::type          allocator_type;
   
   allocator_type get_allocator() const 
      { return *this; }

 protected:
  typedef typename boost::detail::allocator::
         rebind_to<Alloc, typename Alloc::pointer>::type   map_allocator_type;

   val_alloc_ptr priv_allocate_node() 
      {  return this->allocator_type::allocate(deque_buf_size(sizeof(T)));  }

   void priv_deallocate_node(val_alloc_ptr p) 
      {  this->allocator_type::deallocate(p, deque_buf_size(sizeof(T)));  }

   ptr_alloc_ptr priv_allocate_map(size_t n) 
      { return this->map_allocator_type::allocate(n); }

   void priv_deallocate_map(ptr_alloc_ptr p, size_t n) 
      { this->map_allocator_type::deallocate(p, n); }

 public:
   // Class invariants:
   //  For any nonsingular iterator i:
   //    i.node is the address of an element in the map array.  The
   //      contents of i.node is a pointer to the beginning of a node.
   //    i.first == //(i.node) 
   //    i.last  == i.first + node_size
   //    i.cur is a pointer in the range [i.first, i.last).  NOTE:
   //      the implication of this is that i.cur is always a dereferenceable
   //      pointer, even if i is a past-the-end iterator.
   //  Start and Finish are always nonsingular iterators.  NOTE: this means
   //    that an empty deque must have one node, and that a deque
   //    with N elements, where N is the buffer size, must have two nodes.
   //  For every node other than start.node and finish.node, every element
   //    in the node is an initialized object.  If start.node == finish.node,
   //    then [start.cur, finish.cur) are initialized objects, and
   //    the elements outside that range are uninitialized storage.  Otherwise,
   //    [start.cur, start.last) and [finish.first, finish.cur) are initialized
   //    objects, and [start.first, start.cur) and [finish.cur, finish.last)
   //    are uninitialized storage.
   //  [map, map + map_size) is a valid, non-empty range.  
   //  [start.node, finish.node] is a valid range contained within 
   //    [map, map + map_size).  
   //  A pointer in the range [map, map + map_size) points to an allocated node
   //    if and only if the pointer is in the range [start.node, finish.node].
   class const_iterator 
      : public boost::iterator<std::random_access_iterator_tag, 
                              val_alloc_val,  val_alloc_diff, 
                              val_alloc_cptr, val_alloc_cref>
   {
      public:
      static size_t s_buffer_size() { return deque_buf_size(sizeof(T)); }

      typedef std::random_access_iterator_tag   iterator_category;
      typedef val_alloc_val                     value_type;
      typedef val_alloc_cptr                    pointer;
      typedef val_alloc_cref                    reference;
      typedef size_t                            size_type;
      typedef std::ptrdiff_t                    difference_type;

      typedef ptr_alloc_ptr                     index_pointer;
      typedef const_iterator                    self_t;

      friend class deque<T, Alloc>;
      friend class deque_base<T, Alloc>;

      protected: 
      val_alloc_ptr  m_cur;
      val_alloc_ptr  m_first;
      val_alloc_ptr  m_last;
      index_pointer  m_node;

      public: 
      const_iterator(val_alloc_ptr x, index_pointer y) 
         : m_cur(x), m_first(*y),
           m_last(*y + s_buffer_size()), m_node(y) {}

      const_iterator() : m_cur(0), m_first(0), m_last(0), m_node(0) {}

      const_iterator(const const_iterator& x)
         : m_cur(x.m_cur),   m_first(x.m_first), 
           m_last(x.m_last), m_node(x.m_node) {}

      reference operator*() const 
         { return *this->m_cur; }

      pointer operator->() const 
         { return this->m_cur; }

      difference_type operator-(const self_t& x) const 
      {
         return difference_type(this->s_buffer_size()) * (this->m_node - x.m_node - 1) +
            (this->m_cur - this->m_first) + (x.m_last - x.m_cur);
      }

      self_t& operator++() 
      {
         ++this->m_cur;
         if (this->m_cur == this->m_last) {
            this->priv_set_node(this->m_node + 1);
            this->m_cur = this->m_first;
         }
         return *this; 
      }

      self_t operator++(int)  
      {
         self_t tmp = *this;
         ++*this;
         return tmp;
      }

      self_t& operator--() 
      {
         if (this->m_cur == this->m_first) {
            this->priv_set_node(this->m_node - 1);
            this->m_cur = this->m_last;
         }
         --this->m_cur;
         return *this;
      }

      self_t operator--(int) 
      {
         self_t tmp = *this;
         --*this;
         return tmp;
      }

      self_t& operator+=(difference_type n)
      {
         difference_type offset = n + (this->m_cur - this->m_first);
         if (offset >= 0 && offset < difference_type(this->s_buffer_size()))
            this->m_cur += n;
         else {
            difference_type node_offset =
            offset > 0 ? offset / difference_type(this->s_buffer_size())
                        : -difference_type((-offset - 1) / this->s_buffer_size()) - 1;
            this->priv_set_node(this->m_node + node_offset);
            this->m_cur = this->m_first + 
            (offset - node_offset * difference_type(this->s_buffer_size()));
         }
         return *this;
      }

      self_t operator+(difference_type n) const
         {  self_t tmp = *this; return tmp += n;  }

      self_t& operator-=(difference_type n) 
         { return *this += -n; }
       
      self_t operator-(difference_type n) const 
         {  self_t tmp = *this; return tmp -= n;  }

      reference operator[](difference_type n) const 
         { return *(*this + n); }

      bool operator==(const self_t& x) const 
         { return this->m_cur == x.m_cur; }

      bool operator!=(const self_t& x) const 
         { return !(*this == x); }

      bool operator<(const self_t& x) const 
      {
         return (this->m_node == x.m_node) ? 
            (this->m_cur < x.m_cur) : (this->m_node < x.m_node);
      }

      bool operator>(const self_t& x) const  
         { return x < *this; }

      bool operator<=(const self_t& x) const 
         { return !(x < *this); }

      bool operator>=(const self_t& x) const 
         { return !(*this < x); }

      void priv_set_node(index_pointer new_node) 
      {
         this->m_node = new_node;
         this->m_first = *new_node;
         this->m_last = this->m_first + difference_type(this->s_buffer_size());
      }

      friend const_iterator operator+(std::ptrdiff_t n, const const_iterator& x)
         {  return x + n;  }
   };

   //Vector iterator
	class iterator : public const_iterator
	{
	   public:
      typedef std::random_access_iterator_tag   iterator_category;
      typedef val_alloc_val                     value_type;
      typedef ptr_alloc_ptr                     pointer;
      typedef val_alloc_ref                     reference;
      typedef size_t                            size_type;
      typedef std::ptrdiff_t                    difference_type;
      typedef ptr_alloc_ptr                     index_pointer;
      typedef const_iterator                    self_t;

      friend class deque<T, Alloc>;
      friend class deque_base<T, Alloc>;

      private:
      explicit iterator(const const_iterator& x) : const_iterator(x){}

      public:
      //Constructors
      iterator(val_alloc_ptr x, index_pointer y) : const_iterator(x, y){}
      iterator() : const_iterator(){}
      //iterator(const const_iterator &cit) : const_iterator(cit){}
      iterator(const iterator& x) : const_iterator(x){}

      //Pointer like operators
      reference operator*() const { return *this->m_cur; }
      pointer operator->() const { return this->m_cur; }

      reference operator[](difference_type n) const { return *(*this + n); }

      //Increment / Decrement
		iterator& operator++()  
         { this->const_iterator::operator++(); return *this;  }

		iterator operator++(int)
         { iterator tmp = *this; ++*this; return tmp; }
		
      iterator& operator--()
   		{  this->const_iterator::operator--(); return *this;  }

		iterator operator--(int)
	      {  iterator tmp = *this; --*this; return tmp; }

      // Arithmetic
		iterator& operator+=(difference_type off)
		   {  this->const_iterator::operator+=(off); return *this;  }

		iterator operator+(difference_type off) const
			{  return iterator(this->const_iterator::operator+(off));  }

		friend iterator operator+(difference_type off, const iterator& right)
         {  return iterator(off+static_cast<const const_iterator &>(right)); }

		iterator& operator-=(difference_type off)
			{  this->const_iterator::operator-=(off); return *this;   }

		iterator operator-(difference_type off) const
			{  return iterator(this->const_iterator::operator-(off));  }

		difference_type operator-(const const_iterator& right) const
			{  return static_cast<const const_iterator&>(*this) - right;   }
	};

   deque_base(const allocator_type& a, size_t num_elements)
      : allocator_type(a), map_allocator_type(a),
        m_map(0), m_map_size(0),
        m_start(), m_finish()
      { this->priv_initialize_map(num_elements); }

   deque_base(const allocator_type& a) 
      : allocator_type(a), map_allocator_type(a),
        m_map(0), m_map_size(0), m_start(), m_finish() {}

   ~deque_base()
   {
      if (this->m_map) {
         this->priv_destroy_nodes(this->m_start.m_node, this->m_finish.m_node + 1);
         this->priv_deallocate_map(this->m_map, this->m_map_size);
      }
   }
  
 protected:
   void priv_initialize_map(size_t num_elements)
   {
      size_t num_nodes = num_elements / deque_buf_size(sizeof(T)) + 1;

      this->m_map_size = std::max((size_t) InitialMapSize, num_nodes + 2);
      this->m_map = this->priv_allocate_map(this->m_map_size);

      ptr_alloc_ptr nstart = this->m_map + (this->m_map_size - num_nodes) / 2;
      ptr_alloc_ptr nfinish = nstart + num_nodes;
          
      BOOST_TRY {
         this->priv_create_nodes(nstart, nfinish);
      }
      BOOST_CATCH(...){
         this->priv_deallocate_map(this->m_map, this->m_map_size);
         this->m_map = 0;
         this->m_map_size = 0;
         BOOST_RETHROW
      }
      BOOST_CATCH_END

      this->m_start.priv_set_node(nstart);
      this->m_finish.priv_set_node(nfinish - 1);
      this->m_start.m_cur = this->m_start.m_first;
      this->m_finish.m_cur = this->m_finish.m_first +
                     num_elements % deque_buf_size(sizeof(T));

   }

   void priv_create_nodes(ptr_alloc_ptr nstart, ptr_alloc_ptr nfinish)
   {
      ptr_alloc_ptr cur;
      BOOST_TRY {
         for (cur = nstart; cur < nfinish; ++cur)
            *cur = this->priv_allocate_node();
      }
      BOOST_CATCH(...){
         this->priv_destroy_nodes(nstart, cur);
         BOOST_RETHROW
      }
      BOOST_CATCH_END
   }

   void priv_destroy_nodes(ptr_alloc_ptr nstart, ptr_alloc_ptr nfinish)
   {
      for (ptr_alloc_ptr n = nstart; n < nfinish; ++n)
         this->priv_deallocate_node(*n);
   }

   enum { InitialMapSize = 8 };

 protected:
  ptr_alloc_ptr   m_map;
  size_t          m_map_size;
  iterator        m_start;
  iterator        m_finish;
};

template <class T, class Alloc>
class deque : protected deque_base<T, Alloc>
{

  typedef deque_base<T, Alloc> Base;
 public:                         // Basic types
   typedef typename Alloc::value_type           val_alloc_val;
   typedef typename Alloc::pointer              val_alloc_ptr;
   typedef typename Alloc::const_pointer        val_alloc_cptr;
   typedef typename Alloc::reference            val_alloc_ref;
   typedef typename Alloc::const_reference      val_alloc_cref;
   typedef typename boost::detail::allocator::
      rebind_to<Alloc, val_alloc_ptr>::type     ptr_alloc;
   typedef typename ptr_alloc::value_type       ptr_alloc_val;
   typedef typename ptr_alloc::pointer          ptr_alloc_ptr;
   typedef typename ptr_alloc::const_pointer    ptr_alloc_cptr;
   typedef typename ptr_alloc::reference        ptr_alloc_ref;
   typedef typename ptr_alloc::const_reference  ptr_alloc_cref;

   typedef T                                    value_type;
   typedef val_alloc_ptr                        pointer;
   typedef val_alloc_cptr                       const_pointer;
   typedef val_alloc_ref                        reference;
   typedef val_alloc_cref                       const_reference;
   typedef size_t                               size_type;
   typedef std::ptrdiff_t                       difference_type;

   typedef typename Base::allocator_type allocator_type;
   allocator_type get_allocator() const { return Base::get_allocator(); }

   public:                         // Iterators
   typedef typename Base::iterator       iterator;
   typedef typename Base::const_iterator const_iterator;

   typedef reverse_iterator<const_iterator> const_reverse_iterator;
   typedef reverse_iterator<iterator> reverse_iterator;

 protected:                      // Internal typedefs
   typedef ptr_alloc_ptr index_pointer;
   static size_t s_buffer_size() 
         { return deque_buf_size(sizeof(T)); }

 protected:
   using Base::priv_initialize_map;
   using Base::priv_create_nodes;
   using Base::priv_destroy_nodes;
   using Base::priv_allocate_node;
   using Base::priv_deallocate_node;
   using Base::priv_allocate_map;
   using Base::priv_deallocate_map;

   using Base::m_map;
   using Base::m_map_size;
   using Base::m_start;
   using Base::m_finish;

 public:                         // Basic accessors
   iterator begin() 
      { return this->m_start; }

   iterator end() 
      { return this->m_finish; }

   const_iterator begin() const 
      { return this->m_start; }

   const_iterator end() const 
      { return this->m_finish; }

   reverse_iterator rbegin() 
      { return reverse_iterator(this->m_finish); }

   reverse_iterator rend() 
      { return reverse_iterator(this->m_start); }

   const_reverse_iterator rbegin() const 
      { return const_reverse_iterator(this->m_finish); }

   const_reverse_iterator rend() const 
      { return const_reverse_iterator(this->m_start); }

   reference operator[](size_type n)
      { return this->m_start[difference_type(n)]; }

   const_reference operator[](size_type n) const 
      { return this->m_start[difference_type(n)]; }

   void priv_range_check(size_type n) const 
      {  if (n >= this->size())  BOOST_RETHROW std::out_of_range("deque");   }

   reference at(size_type n)
      { this->priv_range_check(n); return (*this)[n]; }

   const_reference at(size_type n) const
      { this->priv_range_check(n); return (*this)[n]; }

   reference front() { return *this->m_start; }

   reference back() 
   {
      iterator tmp = this->m_finish;
      --tmp;
      return *tmp;
   }

   const_reference front() const 
      { return *this->m_start; }

   const_reference back() const 
      {  const_iterator tmp = this->m_finish;  --tmp;   return *tmp;  }

   size_type size() const 
      { return this->m_finish - this->m_start; }

   size_type max_size() const 
      { return size_type(-1); }

   bool empty() const 
      { return this->m_finish == this->m_start; }

 public:                         // Constructor, destructor.
   explicit deque(const allocator_type& a = allocator_type()) 
      : Base(a, 0) {}

   deque(const deque& x) : Base(x.get_allocator(), x.size()) 
      { std::uninitialized_copy(x.begin(), x.end(), this->m_start); }

   deque(size_type n, const value_type& value,
         const allocator_type& a = allocator_type()) : Base(a, n)
      { this->priv_fill_initialize(value); }

   explicit deque(size_type n) : Base(allocator_type(), n)
      { this->priv_fill_initialize(value_type()); }

   // Check whether it's an integral type.  If so, it's not an iterator.
   template <class InpIt>
   deque(InpIt first, InpIt last,
         const allocator_type& a = allocator_type()) : Base(a) 
   {
      //Dispatch depending on integer/iterator
      const bool aux_boolean = boost::is_integral<InpIt>::value;
      typedef boost::mpl::bool_<aux_boolean> Result;
      this->priv_initialize_dispatch(first, last, Result());
   }

   template <class Integer>
   void priv_initialize_dispatch(Integer n, Integer x, boost::mpl::true_) 
   {
      this->priv_initialize_map(n);
      this->priv_fill_initialize(x);
   }

   template <class InpIt>
   void priv_initialize_dispatch(InpIt first, InpIt last, boost::mpl::false_) 
   {
      typedef typename std::iterator_traits<InpIt>::iterator_category ItCat;
      this->priv_range_initialize(first, last, ItCat());
   }

   void priv_destroy_range(iterator p, iterator p2)
      { for(;p != p2; ++p) this->allocator_type::destroy(&*p); }

   void priv_destroy_range(pointer p, pointer p2)
      { for(;p != p2; ++p) this->allocator_type::destroy(p); }

  ~deque() 
      { priv_destroy_range(this->m_start, this->m_finish); }

   deque& operator= (const deque& x) 
   {
      const size_type len = size();
      if (&x != this) {
         if (len >= x.size())
            this->erase(std::copy(x.begin(), x.end(), this->m_start), this->m_finish);
         else {
            const_iterator mid = x.begin() + difference_type(len);
            std::copy(x.begin(), mid, this->m_start);
            this->insert(this->m_finish, mid, x.end());
         }
      }
      return *this;
   }        

   void swap(deque& x) {
      std::swap(this->m_start, x.m_start);
      std::swap(this->m_finish, x.m_finish);
      std::swap(this->m_map, x.m_map);
      std::swap(this->m_map_size, x.m_map_size);
   }

 public: 
  // assign(), a generalized assignment member function.  Two
  // versions: one that takes a count, and one that takes a range.
  // The range version is a member template, so we dispatch on whether
  // or not the type is an integer.
   void priv_fill_assign(size_type n, const T& val) {
      if (n > size()) {
         std::fill(begin(), end(), val);
         this->insert(end(), n - size(), val);
      }
      else {
         this->erase(begin() + n, end());
         std::fill(begin(), end(), val);
      }
   }

   void assign(size_type n, const T& val) {
      this->priv_fill_assign(n, val);
      }

   template <class InpIt>
   void assign(InpIt first, InpIt last) {
      //Dispatch depending on integer/iterator
      const bool aux_boolean = boost::is_integral<InpIt>::value;
      typedef boost::mpl::bool_<aux_boolean> Result;
      this->priv_assign_dispatch(first, last, Result());
   }

 private:                        // helper functions for assign() 

   template <class Integer>
   void priv_assign_dispatch(Integer n, Integer val, boost::mpl::true_)
      { this->priv_fill_assign((size_type) n, (T) val); }

   template <class InpIt>
   void priv_assign_dispatch(InpIt first, InpIt last, boost::mpl::false_) 
   {
      typedef typename std::iterator_traits<InpIt>::iterator_category ItCat;
      this->priv_assign_aux(first, last, ItCat());
   }

   template <class InpIt>
   void priv_assign_aux(InpIt first, InpIt last, std::input_iterator_tag)
   {
      iterator cur = begin();
      for ( ; first != last && cur != end(); ++cur, ++first)
         *cur = *first;
      if (first == last)
         this->erase(cur, end());
      else
         this->insert(end(), first, last);
   }

   template <class FwdIt>
   void priv_assign_aux(FwdIt first, FwdIt last,
                        std::forward_iterator_tag) {
      size_type len = 0;
      std::distance(first, last, len);
      if (len > size()) {
         FwdIt mid = first;
         std::advance(mid, size());
         std::copy(first, mid, begin());
         this->insert(end(), mid, last);
      }
      else
         this->erase(std::copy(first, last, begin()), end());
   }

 public:                         // push_* and pop_*
     
   void push_back(const value_type& t) 
   {
      if (this->m_finish.m_cur != this->m_finish.m_last - 1) {
         this->allocator_type::construct(this->m_finish.m_cur, t);
         ++this->m_finish.m_cur;
      }
      else
         this->priv_push_back_aux(t);
   }

   void push_back() 
   {
      if (this->m_finish.m_cur != this->m_finish.m_last - 1) {
         this->allocator_type::construct(this->m_finish.m_cur, value_type());
         ++this->m_finish.m_cur;
      }
      else
         this->priv_push_back_aux();
   }

   void push_front(const value_type& t) {
      if (this->m_start.m_cur != this->m_start.m_first) {
         this->allocator_type::construct(this->m_start.m_cur - 1, t);
         --this->m_start.m_cur;
      }
      else
         this->priv_push_front_aux(t);
   }

   void push_front() 
   {
      if (this->m_start.m_cur != this->m_start.m_first) {
         this->allocator_type::construct(this->m_start.m_cur - 1, value_type());
         --this->m_start.m_cur;
      }
      else
         this->priv_push_front_aux();
   }


   void pop_back() 
   {
      if (this->m_finish.m_cur != this->m_finish.m_first) {
         --this->m_finish.m_cur;
         this->allocator_type::destroy(this->m_finish.m_cur);
      }
      else
         this->priv_pop_back_aux();
   }

   void pop_front() 
   {
      if (this->m_start.m_cur != this->m_start.m_last - 1) {
         this->allocator_type::destroy(this->m_start.m_cur);
         ++this->m_start.m_cur;
      }
      else 
         this->priv_pop_front_aux();
   }

 public:                         // Insert

   iterator insert(iterator position, const value_type& x) 
   {
      if (position.m_cur == this->m_start.m_cur) {
         this->push_front(x);
         return this->m_start;
      }
      else if (position.m_cur == this->m_finish.m_cur) {
         this->push_back(x);
         iterator tmp = this->m_finish;
         --tmp;
         return tmp;
      }
      else {
         return this->priv_insert_aux(position, x);
      }
   }

   iterator insert(iterator position)
      { return insert(position, value_type()); }

   void insert(iterator pos, size_type n, const value_type& x)
      { this->priv_fill_insert(pos, n, x); }

   // Check whether it's an integral type.  If so, it's not an iterator.
   template <class InpIt>
   void insert(iterator pos, InpIt first, InpIt last) 
   {
      //Dispatch depending on integer/iterator
      const bool aux_boolean = boost::is_integral<InpIt>::value;
      typedef boost::mpl::bool_<aux_boolean> Result;
      this->priv_insert_dispatch(pos, first, last, Result());
   }

   template <class Integer>
   void priv_insert_dispatch(iterator pos, Integer n, Integer x,
                           boost::mpl::true_) 
   {
      this->priv_fill_insert(pos, (size_type) n, (value_type) x);
   }

   template <class InpIt>
   void priv_insert_dispatch(iterator pos,
                           InpIt first, InpIt last,
                           boost::mpl::false_) 
   {
      typedef typename std::iterator_traits<InpIt>::iterator_category ItCat;
      this->insert(pos, first, last, ItCat());
   }

   void resize(size_type new_size, const value_type& x) 
   {
      const size_type len = size();
      if (new_size < len) 
         this->erase(this->m_start + new_size, this->m_finish);
      else
         this->insert(this->m_finish, new_size - len, x);
   }

   void resize(size_type new_size) 
      { resize(new_size, value_type()); }

   template <class InpIt>
   void insert(iterator pos, InpIt first, InpIt last, std::input_iterator_tag)
      {  std::copy(first, last, std::inserter(*this, pos));  }

   template <class FwdIt>
   void insert(iterator pos, FwdIt first, FwdIt last, std::forward_iterator_tag) 
   {
      size_type n = 0;
      n = std::distance(first, last);

      if (pos.m_cur == this->m_start.m_cur) {
         iterator new_start = this->priv_reserve_elements_at_front(n);
         BOOST_TRY{
            std::uninitialized_copy(first, last, new_start);
            this->m_start = new_start;
         }
         BOOST_CATCH(...){
            this->priv_destroy_nodes(new_start.m_node, this->m_start.m_node);
            BOOST_RETHROW
         }
         BOOST_CATCH_END
      }
      else if (pos.m_cur == this->m_finish.m_cur) {
         iterator new_finish = this->priv_reserve_elements_at_back(n);
         BOOST_TRY{
            std::uninitialized_copy(first, last, this->m_finish);
            this->m_finish = new_finish;
         }
         BOOST_CATCH(...){
            this->priv_destroy_nodes(this->m_finish.m_node + 1, new_finish.m_node + 1);
            BOOST_RETHROW
         }
         BOOST_CATCH_END
      }
      else
         this->priv_insert_aux(pos, first, last, n);
   }

 public:                         // Erase
   iterator erase(iterator pos) 
   {
      iterator next = pos;
      ++next;
      difference_type index = pos - this->m_start;
      if (size_type(index) < (this->size() >> 1)) {
         std::copy_backward(this->m_start, pos, next);
         pop_front();
      }
      else {
         std::copy(next, this->m_finish, pos);
         pop_back();
      }
      return this->m_start + index;
   }

 private:                        // Internal insert functions

   iterator priv_insert_aux(iterator pos, const value_type& x)
   {
      difference_type index = pos - this->m_start;
      value_type x_copy = x;
      if (size_type(index) < this->size() / 2) {
         this->push_front(this->front());
         iterator front1 = this->m_start;
         ++front1;
         iterator front2 = front1;
         ++front2;
         pos = this->m_start + index;
         iterator pos1 = pos;
         ++pos1;
         std::copy(front2, pos1, front1);
      }
      else {
         this->push_back(back());
         iterator back1 = this->m_finish;
         --back1;
         iterator back2 = back1;
         --back2;
         pos = this->m_start + index;
         std::copy_backward(pos, back2, back1);
      }
      *pos = x_copy;
      return pos;
   }

   iterator priv_insert_aux(iterator pos)
   {
      difference_type index = pos - this->m_start;
      if (index < static_cast<difference_type>(size() / 2)) {
         this->push_front(this->front());
         iterator front1 = this->m_start;
         ++front1;
         iterator front2 = front1;
         ++front2;
         pos = this->m_start + index;
         iterator pos1 = pos;
         ++pos1;
         std::copy(front2, pos1, front1);
      }
      else {
         this->push_back(back());
         iterator back1 = this->m_finish;
         --back1;
         iterator back2 = back1;
         --back2;
         pos = this->m_start + index;
         std::copy_backward(pos, back2, back1);
      }
      *pos = value_type();
      return pos;
   }

   void priv_insert_aux(iterator pos, size_type n, const value_type& x)
   {
      const difference_type elems_before = pos - this->m_start;
      size_type length = this->size();
      value_type x_copy = x;
      if (elems_before < difference_type(length / 2)) {
         iterator new_start = this->priv_reserve_elements_at_front(n);
         iterator old_start = this->m_start;
         pos = this->m_start + elems_before;
         BOOST_TRY {
            if (elems_before >= difference_type(n)) {
               iterator start_n = this->m_start + difference_type(n);
               std::uninitialized_copy(this->m_start, start_n, new_start);
               this->m_start = new_start;
               std::copy(start_n, pos, old_start);
               std::fill(pos - difference_type(n), pos, x_copy);
            }
            else {
               this->priv_uninitialized_copy_fill(this->m_start, pos, new_start, 
                                             this->m_start, x_copy);
               this->m_start = new_start;
               std::fill(old_start, pos, x_copy);
            }
         }
         BOOST_CATCH(...){
            this->priv_destroy_nodes(new_start.m_node, this->m_start.m_node);
            BOOST_RETHROW
         }
         BOOST_CATCH_END
      }
      else {
         iterator new_finish = this->priv_reserve_elements_at_back(n);
         iterator old_finish = this->m_finish;
         const difference_type elems_after = 
            difference_type(length) - elems_before;
         pos = this->m_finish - elems_after;
         BOOST_TRY {
            if (elems_after > difference_type(n)) {
               iterator finish_n = this->m_finish - difference_type(n);
               std::uninitialized_copy(finish_n, this->m_finish, this->m_finish);
               this->m_finish = new_finish;
               std::copy_backward(pos, finish_n, old_finish);
               std::fill(pos, pos + difference_type(n), x_copy);
            }
            else {
               this->priv_uninitialized_fill_copy(this->m_finish, pos + difference_type(n),
                                          x_copy, pos, this->m_finish);
               this->m_finish = new_finish;
               std::fill(pos, old_finish, x_copy);
            }
         }
         BOOST_CATCH(...){
            this->priv_destroy_nodes(this->m_finish.m_node + 1, new_finish.m_node + 1);
            BOOST_RETHROW
         }
         BOOST_CATCH_END
      }
   }

   template <class FwdIt>
   void priv_insert_aux(iterator pos, FwdIt first, FwdIt last, size_type n)
   {
      const difference_type elemsbefore = pos - this->m_start;
      size_type length = size();
      if (elemsbefore < static_cast<difference_type>(length / 2)) {
         iterator new_start = this->priv_reserve_elements_at_front(n);
         iterator old_start = this->m_start;
         pos = this->m_start + elemsbefore;
         BOOST_TRY {
            if (elemsbefore >= difference_type(n)) {
               iterator start_n = this->m_start + difference_type(n); 
               std::uninitialized_copy(this->m_start, start_n, new_start);
               this->m_start = new_start;
               std::copy(start_n, pos, old_start);
               std::copy(first, last, pos - difference_type(n));
            }
            else {
               FwdIt mid = first;
               std::advance(mid, difference_type(n) - elemsbefore);
               this->priv_uninitialized_copy_copy(this->m_start, pos, first, mid, new_start);
               this->m_start = new_start;
               std::copy(mid, last, old_start);
            }
         }
         BOOST_CATCH(...){
            this->priv_destroy_nodes(new_start.m_node, this->m_start.m_node);
            BOOST_RETHROW
         }
         BOOST_CATCH_END
      }
      else {
         iterator new_finish = this->priv_reserve_elements_at_back(n);
         iterator old_finish = this->m_finish;
         const difference_type elemsafter = 
            difference_type(length) - elemsbefore;
         pos = this->m_finish - elemsafter;
         BOOST_TRY {
            if (elemsafter > difference_type(n)) {
               iterator finish_n = this->m_finish - difference_type(n);
               std::uninitialized_copy(finish_n, this->m_finish, this->m_finish);
               this->m_finish = new_finish;
               std::copy_backward(pos, finish_n, old_finish);
               std::copy(first, last, pos);
            }
            else {
               FwdIt mid = first;
               std::advance(mid, elemsafter);
               this->priv_uninitialized_copy_copy(mid, last, pos, this->m_finish, this->m_finish);
               this->m_finish = new_finish;
               std::copy(first, mid, pos);
            }
         }
         BOOST_CATCH(...){
            this->priv_destroy_nodes(this->m_finish.m_node + 1, new_finish.m_node + 1);
            BOOST_RETHROW
         }
         BOOST_CATCH_END
      }
   }

   void priv_fill_insert(iterator pos, size_type n, const value_type& x)
   {
      if (pos.m_cur == this->m_start.m_cur) {
         iterator new_start = this->priv_reserve_elements_at_front(n);
         BOOST_TRY {
            std::uninitialized_fill(new_start, this->m_start, x);
            this->m_start = new_start;
         }
         BOOST_CATCH(...){
            this->priv_destroy_nodes(new_start.m_node, this->m_start.m_node);
            BOOST_RETHROW
         }
         BOOST_CATCH_END
      }
      else if (pos.m_cur == this->m_finish.m_cur) {
         iterator new_finish = this->priv_reserve_elements_at_back(n);
         BOOST_TRY {
            std::uninitialized_fill(this->m_finish, new_finish, x);
            this->m_finish = new_finish;
         }
         BOOST_CATCH(...){
            this->priv_destroy_nodes(this->m_finish.m_node + 1, new_finish.m_node + 1);
            BOOST_RETHROW
         }
         BOOST_CATCH_END
      }
      else 
         this->priv_insert_aux(pos, n, x);
   }

   iterator erase(iterator first, iterator last)
   {
      if (first == this->m_start && last == this->m_finish) {
         this->clear();
         return this->m_finish;
      }
      else {
         difference_type n = last - first;
         difference_type elems_before = first - this->m_start;
         if (elems_before < difference_type((this->size() - n) / 2)) {
            std::copy_backward(this->m_start, first, last);
            iterator new_start = this->m_start + n;
            this->priv_destroy_range(this->m_start, new_start);
            this->priv_destroy_nodes(new_start.m_node, this->m_start.m_node);
            this->m_start = new_start;
         }
         else {
            std::copy(last, this->m_finish, first);
            iterator new_finish = this->m_finish - n;
            this->priv_destroy_range(new_finish, this->m_finish);
            this->priv_destroy_nodes(new_finish.m_node + 1, this->m_finish.m_node + 1);
            this->m_finish = new_finish;
         }
         return this->m_start + elems_before;
      }
   }

   void clear()
   {
      for (index_pointer node = this->m_start.m_node + 1;
            node < this->m_finish.m_node;
            ++node) {
         this->priv_destroy_range(*node, *node + this->s_buffer_size());
         this->priv_deallocate_node(*node);
      }

      if (this->m_start.m_node != this->m_finish.m_node) {
         this->priv_destroy_range(this->m_start.m_cur, this->m_start.m_last);
         this->priv_destroy_range(this->m_finish.m_first, this->m_finish.m_cur);
         this->priv_deallocate_node(this->m_finish.m_first);
      }
      else
         this->priv_destroy_range(this->m_start.m_cur, this->m_finish.m_cur);

      this->m_finish = this->m_start;
   }

 private:                       // Internal construction/destruction

   // Precondition: this->m_start and this->m_finish have already been initialized,
   // but none of the deque's elements have yet been constructed.
   void priv_fill_initialize(const value_type& value) 
   {
      index_pointer cur;
      BOOST_TRY {
         for (cur = this->m_start.m_node; cur < this->m_finish.m_node; ++cur){
            std::uninitialized_fill(*cur, *cur + this->s_buffer_size(), value);
         }
         std::uninitialized_fill(this->m_finish.m_first, this->m_finish.m_cur, value);
      }
      BOOST_CATCH(...){
         this->priv_destroy_range(this->m_start, iterator(*cur, cur));
         BOOST_RETHROW
      }
      BOOST_CATCH_END
   }

   template <class InpIt>
   void priv_range_initialize(InpIt first, InpIt last, std::input_iterator_tag)
   {
      this->priv_initialize_map(0);
      BOOST_TRY {
         for ( ; first != last; ++first)
            this->push_back(*first);
      }
      BOOST_CATCH(...){
         this->clear();
         BOOST_RETHROW
      }
      BOOST_CATCH_END
   }

   template <class FwdIt>
   void priv_range_initialize(FwdIt first, FwdIt last, std::forward_iterator_tag)
   {
      size_type n = 0;
      n = std::distance(first, last);
      this->priv_initialize_map(n);

      index_pointer cur_node;
      BOOST_TRY {
         for (cur_node = this->m_start.m_node; 
               cur_node < this->m_finish.m_node; 
               ++cur_node) {
            FwdIt mid = first;
            std::advance(mid, this->s_buffer_size());
            std::uninitialized_copy(first, mid, *cur_node);
            first = mid;
         }
         std::uninitialized_copy(first, last, this->m_finish.m_first);
      }
      BOOST_CATCH(...){
         this->priv_destroy_range(this->m_start, iterator(*cur_node, cur_node));
         BOOST_RETHROW
      }
      BOOST_CATCH_END
   }

   // Called only if this->m_finish.m_cur == this->m_finish.m_last - 1.
   void priv_push_back_aux(const value_type& t)
   {
      value_type t_copy = t;
      this->priv_reserve_map_at_back();
      *(this->m_finish.m_node + 1) = this->priv_allocate_node();
      BOOST_TRY {
         this->allocator_type::construct(this->m_finish.m_cur, t_copy);
         this->m_finish.priv_set_node(this->m_finish.m_node + 1);
         this->m_finish.m_cur = this->m_finish.m_first;
      }
      BOOST_CATCH(...){
         this->priv_deallocate_node(*(this->m_finish.m_node + 1));
         BOOST_RETHROW
      }
      BOOST_CATCH_END
   }

   // Called only if this->m_finish.m_cur == this->m_finish.m_last - 1.
   void priv_push_back_aux()
   {
      this->priv_reserve_map_at_back();
      *(this->m_finish.m_node + 1) = this->priv_allocate_node();
      BOOST_TRY {
         this->allocator_type::construct(this->m_finish.m_cur, value_type());
         this->m_finish.priv_set_node(this->m_finish.m_node + 1);
         this->m_finish.m_cur = this->m_finish.m_first;
      }
      BOOST_CATCH(...){
         this->priv_deallocate_node(*(this->m_finish.m_node + 1));
         BOOST_RETHROW
      }
      BOOST_CATCH_END
   }

   // Called only if this->m_start.m_cur == this->m_start.m_first.
   void priv_push_front_aux(const value_type& t)
   {
      value_type t_copy = t;
      this->priv_reserve_map_at_front();
      *(this->m_start.m_node - 1) = this->priv_allocate_node();
      BOOST_TRY {
         this->m_start.priv_set_node(this->m_start.m_node - 1);
         this->m_start.m_cur = this->m_start.m_last - 1;
         this->allocator_type::construct(this->m_start.m_cur, t_copy);
      }
      BOOST_CATCH(...){
         ++this->m_start;
         this->priv_deallocate_node(*(this->m_start.m_node - 1));
         BOOST_RETHROW
      }
      BOOST_CATCH_END
   } 

   // Called only if this->m_start.m_cur == this->m_start.m_first.
   void priv_push_front_aux()
   {
      this->priv_reserve_map_at_front();
      *(this->m_start.m_node - 1) = this->priv_allocate_node();
      BOOST_TRY {
         this->m_start.priv_set_node(this->m_start.m_node - 1);
         this->m_start.m_cur = this->m_start.m_last - 1;
         this->allocator_type::construct(this->m_start.m_cur, value_type());
      }
      BOOST_CATCH(...){
         ++this->m_start;
         this->priv_deallocate_node(*(this->m_start.m_node - 1));
         BOOST_RETHROW
      }
      BOOST_CATCH_END
   } 

   // Called only if this->m_finish.m_cur == this->m_finish.m_first.
   void priv_pop_back_aux()
   {
      this->priv_deallocate_node(this->m_finish.m_first);
      this->m_finish.priv_set_node(this->m_finish.m_node - 1);
      this->m_finish.m_cur = this->m_finish.m_last - 1;
      this->allocator_type::destroy(this->m_finish.m_cur);
   }

   // Called only if this->m_start.m_cur == this->m_start.m_last - 1.  Note that 
   // if the deque has at least one element (a precondition for this member 
   // function), and if this->m_start.m_cur == this->m_start.m_last, then the deque 
   // must have at least two nodes.
   void priv_pop_front_aux()
   {
      this->allocator_type::destroy(this->m_start.m_cur);
      this->priv_deallocate_node(this->m_start.m_first);
      this->m_start.priv_set_node(this->m_start.m_node + 1);
      this->m_start.m_cur = this->m_start.m_first;
   }      

   iterator priv_reserve_elements_at_front(size_type n) 
   {
      size_type vacancies = this->m_start.m_cur - this->m_start.m_first;
      if (n > vacancies) 
         this->priv_new_elements_at_front(n - vacancies);
      return this->m_start - difference_type(n);
   }

   iterator priv_reserve_elements_at_back(size_type n) 
   {
      size_type vacancies = (this->m_finish.m_last - this->m_finish.m_cur) - 1;
      if (n > vacancies)
         this->priv_new_elements_at_back(n - vacancies);
      return this->m_finish + difference_type(n);
   }

   void priv_new_elements_at_front(size_type new_elems)
   {
      size_type new_nodes = (new_elems + this->s_buffer_size() - 1) / 
                              this->s_buffer_size();
      this->priv_reserve_map_at_front(new_nodes);
      size_type i;
      BOOST_TRY {
         for (i = 1; i <= new_nodes; ++i)
            *(this->m_start.m_node - i) = this->priv_allocate_node();
      }
      BOOST_CATCH(...) {
         for (size_type j = 1; j < i; ++j)
            this->priv_deallocate_node(*(this->m_start.m_node - j));      
         BOOST_RETHROW
      }
      BOOST_CATCH_END
   }

   void priv_new_elements_at_back(size_type new_elems)
   {
      size_type new_nodes = (new_elems + this->s_buffer_size() - 1) 
                              / this->s_buffer_size();
      this->priv_reserve_map_at_back(new_nodes);
      size_type i;
      BOOST_TRY {
         for (i = 1; i <= new_nodes; ++i)
            *(this->m_finish.m_node + i) = this->priv_allocate_node();
      }
      BOOST_CATCH(...) {
         for (size_type j = 1; j < i; ++j)
            this->priv_deallocate_node(*(this->m_finish.m_node + j));      
         BOOST_RETHROW
      }
      BOOST_CATCH_END
   }

 private:                      // Allocation of this->m_map and nodes

   // Makes sure the this->m_map has space for new nodes.  Does not actually
   //  add the nodes.  Can invalidate this->m_map pointers.  (And consequently, 
   //  deque iterators.)
   void priv_reserve_map_at_back (size_type nodes_to_add = 1) 
   {
      if (nodes_to_add + 1 > this->m_map_size - (this->m_finish.m_node - this->m_map))
         this->priv_reallocate_map(nodes_to_add, false);
   }

   void priv_reserve_map_at_front (size_type nodes_to_add = 1) 
   {
      if (nodes_to_add > size_type(this->m_start.m_node - this->m_map))
         this->priv_reallocate_map(nodes_to_add, true);
   }

   void priv_reallocate_map(size_type nodes_to_add, bool add_at_front)
   {
      size_type old_num_nodes = this->m_finish.m_node - this->m_start.m_node + 1;
      size_type new_num_nodes = old_num_nodes + nodes_to_add;

      index_pointer new_nstart;
      if (this->m_map_size > 2 * new_num_nodes) {
         new_nstart = this->m_map + (this->m_map_size - new_num_nodes) / 2 
                           + (add_at_front ? nodes_to_add : 0);
         if (new_nstart < this->m_start.m_node)
            std::copy(this->m_start.m_node, this->m_finish.m_node + 1, new_nstart);
         else
            std::copy_backward(this->m_start.m_node, this->m_finish.m_node + 1, 
                        new_nstart + old_num_nodes);
      }
      else {
         size_type new_map_size = 
            this->m_map_size + std::max(this->m_map_size, nodes_to_add) + 2;

         index_pointer new_map = this->priv_allocate_map(new_map_size);
         new_nstart = new_map + (new_map_size - new_num_nodes) / 2
                              + (add_at_front ? nodes_to_add : 0);
         std::copy(this->m_start.m_node, this->m_finish.m_node + 1, new_nstart);
         this->priv_deallocate_map(this->m_map, this->m_map_size);

         this->m_map = new_map;
         this->m_map_size = new_map_size;
      }

      this->m_start.priv_set_node(new_nstart);
      this->m_finish.priv_set_node(new_nstart + old_num_nodes - 1);
   }

 private:

   // this->priv_uninitialized_copy_fill
   // Copies [first1, last1) into [first2, first2 + (last1 - first1)), and
   //  fills [first2 + (last1 - first1), last2) with x.
   void priv_uninitialized_copy_fill(iterator first1, iterator last1,
                                   iterator first2, iterator last2,
                                   const T& x)
   {
      iterator mid2 = std::uninitialized_copy(first1, last1, first2);
      BOOST_TRY {
         std::uninitialized_fill(mid2, last2, x);
      }
      BOOST_CATCH(...){
         for(;first2 != mid2; ++first2){
            this->allocator_type::destroy(&*first2); 
         }
      }
      BOOST_CATCH_END
   }

   // this->priv_uninitialized_fill_copy
   // Fills [result, mid) with x, and copies [first, last) into
   //  [mid, mid + (last - first)).
   iterator priv_uninitialized_fill_copy(iterator result, iterator mid,
                                       const T& x,
                                       iterator first, iterator last)
   {
      std::uninitialized_fill(result, mid, x);
      BOOST_TRY {
         return std::uninitialized_copy(first, last, mid);
      }
      BOOST_CATCH(...){
         for(;result != mid; ++result){
            this->allocator_type::destroy(&*result); 
         }
         BOOST_RETHROW
      }
      BOOST_CATCH_END
   }

   // this->priv_uninitialized_copy_copy
   // Copies [first1, last1) into [result, result + (last1 - first1)), and
   //  copies [first2, last2) into
   //  [result, result + (last1 - first1) + (last2 - first2)).
   template <class InpIt1, class InpIt2, class FwdIt>
   FwdIt priv_uninitialized_copy_copy(InpIt1 first1, InpIt1 last1,
                                    InpIt2 first2, InpIt2 last2,
                                    FwdIt result)
   {
      FwdIt mid = std::uninitialized_copy(first1, last1, result);
      BOOST_TRY {
         return std::uninitialized_copy(first2, last2, mid);
      }
      BOOST_CATCH(...){
         for(;result != mid; ++result){
            this->allocator_type::destroy(&*result); 
         }
         BOOST_RETHROW
      }
      BOOST_CATCH_END
   }
};

// Nonmember functions.
template <class T, class Alloc>
inline bool operator==(const deque<T, Alloc>& x,
                       const deque<T, Alloc>& y)
{
   return x.size() == y.size() && equal(x.begin(), x.end(), y.begin());
}

template <class T, class Alloc>
inline bool operator<(const deque<T, Alloc>& x,
                      const deque<T, Alloc>& y) 
{
   return lexicographical_compare(x.begin(), x.end(), y.begin(), y.end());
}

template <class T, class Alloc>
inline bool operator!=(const deque<T, Alloc>& x,
                       const deque<T, Alloc>& y) 
   {  return !(x == y);   }

template <class T, class Alloc>
inline bool operator>(const deque<T, Alloc>& x,
                      const deque<T, Alloc>& y) 
   {  return y < x; }

template <class T, class Alloc>
inline bool operator<=(const deque<T, Alloc>& x,
                       const deque<T, Alloc>& y) 
   {  return !(y < x); }

template <class T, class Alloc>
inline bool operator>=(const deque<T, Alloc>& x,
                       const deque<T, Alloc>& y) 
   {  return !(x < y); }

template <class T, class Alloc>
inline void swap(deque<T,Alloc>& x, deque<T,Alloc>& y) 
   {  x.swap(y);  }

}  //namespace shmem {

}  //namespace boost {

#include <boost/shmem/detail/config_end.hpp>

#endif //   #ifndef  BOOST_SHMEM_DEQUE_HPP

