/* The following code declares class array,
 * an STL container (as wrapper) for arrays of constant size, with
 * storage policies.
 *
 * (C) Copyright Herve Bronnimann, 2002.
 * (C) Copyright Nicolai M. Josuttis 2001.
 * Permission to copy, use, modify, sell and distribute this software
 * is granted provided this copyright notice appears in all copies.
 * This software is provided "as is" without express or implied
 * warranty, and with no claim as to its suitability for any purpose.
 *
 * 3  May 2002 - Allocators, destructor (Herve)
 * 30 Apr 2002 - Introduction of iterator policies (Herve)
 * 17 Apr 2002 - Introduction of checking policies (Herve)
 * 10 Apr 2002 - Introduction of a variable-size policy (Herve)
 * 10 Apr 2002 - Introduction of two fixed-size policies (Herve)
 * 10 Apr 2002 - Initial steal from Boost::array (Herve)
 */
#ifndef CS903_POLICY_VECTOR_HPP
#define CS903_POLICY_VECTOR_HPP

#include <cstddef>
#include <stdexcept>
#include <iterator>
#include <algorithm>
#include <memory>

// Global declaration of policy_vector

template < class T,
           class StoragePolicy,
           template <class S> class CheckingPolicy,
           template <class T2, class V2> class IteratorPolicy,
           class Alloc
	 > 
class policy_vector;

// Base class for ordinary allocators
// Unlike the SGI STL, we don't want to have specializations for
// instanceless allocators. Rather, we let the empty-object optimization
// do its job. That also takes care of whether Alloc::allocate is static
// or not.

template <class T, class Alloc>
class policy_vector_base {
public:
  typedef Alloc allocator_type;
  const allocator_type& get_allocator() const { return alloc; }
  policy_vector_base(const allocator_type& a) : alloc(a) {}
protected:
  T* allocate(std::size_t n) { return alloc.allocate(n); }
  void deallocate(T* ptr, std::size_t n) { if (ptr) alloc.deallocate(ptr,n); }
  void destroy(T* ptr) { alloc.destroy(ptr); }
  void destroy(T* first, T* last) {
    while (first!=last) { alloc.destroy(first); ++first; }
  }
private:
  Alloc alloc;
};

// A. STORAGE POLICIES:
// Provides all the functions related to storage, which do not depend on
// the type T of elements stored. For the functions which depend on T,
// they are part of a policy_vector_storage class. To define those
// functions, one must specialize policy_vector_storage

template < class T, class StoragePolicy, class Alloc >
struct policy_vector_storage;

// A1. Fixed aggregate array storage policy

template <std::size_t N>
struct fixed_aggregate_array {
  typedef std::size_t    size_type;
  // size is constant
  static size_type size() { return N; }
  static bool empty() { return false; }
  static size_type max_size() { return N; }
  enum { static_size = N };
};

template < class T, std::size_t N, class Alloc >
struct policy_vector_storage<T, fixed_aggregate_array<N>, Alloc >
  // : policy_vector_base<T,Alloc> // no reason to for fixed storage
  : fixed_aggregate_array<N>
{
private:
  typedef policy_vector_storage<T, fixed_aggregate_array<N>, Alloc > Self;
protected:
  T elems[N]; // don't use allocator for aggregate
public:
  // constructors and destructor
  explicit policy_vector_storage(const Alloc&) {}
  policy_vector_storage(std::size_t n, const T& value, const Alloc&)
    { std::fill_n(elems, std::min(n,N), value); }
  explicit policy_vector_storage(std::size_t)
    {}
  policy_vector_storage(Self const& rhs)
    { std::copy(rhs.elems, rhs.elems+N, elems); }
  template <class T2>
  policy_vector_storage(T2 rhs[N], const Alloc&)
    { std::copy(rhs, rhs+N, elems); }
  template <class Iterator>
  policy_vector_storage(Iterator first, Iterator last, const Alloc&)
    { std::copy(first, first+std::min(std::distance(first, last),N), elems); }
  // default destructor: destroys array
  // default copy constructor: copies array
  // assign value to all elements
  void assign (const T& value) {
    std::fill_n(elems, N, value);
  }
  // assign value to first n elements -- leave others unchanged if necessary
  void assign (std::size_t n, const T& value) {
    std::fill_n(elems, std::min(n,N), value);
  }
  // assign range -- leave other elements unchanged if necessary
  template <class RandomAccessIter>
  void assign(RandomAccessIter first, RandomAccessIter last) {
    std::copy(first, last+std::min(last-first, (std::ptrdiff_t)N), elems);
  }
  // swap (note: linear complexity)
  void swap (policy_vector_storage& y) {
      std::swap_ranges(elems, elems+N, y.elems);
  }
};

// A2. Fixed allocated array storage policy

template <std::size_t N>
struct fixed_allocated_array {
  typedef std::size_t    size_type;
  // size is constant
  static size_type size() { return N; }
  static bool empty() { return false; }
  static size_type max_size() { return N; }
  enum { static_size = N };
};

template < class T, std::size_t N, class Alloc >
struct policy_vector_storage<T, fixed_allocated_array<N>, Alloc >
  : policy_vector_base<T,Alloc>
  , fixed_allocated_array<N>
{
private:
  typedef policy_vector_base<T,Alloc>                                Base;
  typedef policy_vector_storage<T, fixed_allocated_array<N>, Alloc > Self;
protected:
  T* elems;
public:
  // constructors
  explicit policy_vector_storage(const Alloc& a)
    : Base(a), elems(allocate(N))
    {}
  policy_vector_storage(std::size_t n, const T& value, const Alloc& a)
    : Base(a), elems(allocate(N))
    { std::uninitialized_fill_n(elems, std::min(n,N), value); }
  explicit policy_vector_storage(std::size_t)
    : Base(Alloc()), elems(allocate(N))
    { std::uninitialized_fill_n(elems, N, T()); }
  policy_vector_storage(const Self& rhs)
    : Base(rhs.get_allocator()), elems(allocate(N)) 
    { std::uninitialized_copy(rhs.elems, rhs.elems+N, elems); }
  template <class T2>
  policy_vector_storage(T2 rhs[N], const Alloc& a)
    : Base(a), elems(allocate(N))
    {
      T* end = std::uninitialized_copy(rhs, rhs+N, elems);
      std::uninitialized_fill(end, elems+N, T());
    }
  template <class Iterator>
  policy_vector_storage(Iterator first, Iterator last, const Alloc& a)
    : Base(a), elems(allocate(N))
    {
      if (std::distance(first, last) < N) {
        T* end = std::uninitialized_copy(first, last, elems),
        std::uninitialized_fill(end, elems+N, T());
      } else
        std::uninitialized_copy(first, first+N, elems);
    }
  // destructor
  ~policy_vector_storage()
    {
      destroy(elems, elems+N);
      deallocate(elems, N); elems = 0;
    }
  // copy constructor
  policy_vector_storage& operator=(Self const& rhs) {
    elems = allocate(N); 
    std::uninitialized_copy(rhs.elems, rhs.elems+N, elems);
    return *this;
  }
  // assign one value to all elements
  void assign (const T& value) {
    std::fill_n(elems, N, value);
  }
  // assign one value to first n elements -- leave others unchanged if necessary
  void assign (std::size_t n, const T& value) {
    std::fill_n(elems, std::min(n,N), value);
  }
  // assign range -- leave other elements unchanged if necessary
  template <class RandomAccessIter>
  void assign(RandomAccessIter first, RandomAccessIter last) {
    std::copy(first, first+std::min(last-first, (std::ptrdiff_t)N), elems);
  }
  // swap (note: linear complexity)
  void swap (policy_vector_storage& y) {
    std::swap_ranges(elems, elems+N, y.elems);
  }
};

// A3. Variable size allocated array storage policy
// Starts with an initial size, and grows from there
// The growth is controlled by the resizing policy

   // A31. Resizing policies for variable_allocated_array
   // resize_before_insert(size, capacity, n, new_size) returns true if
   // resizing may be needed after inserting n elements,
   // and new_size stores the new capacity after the resizing
  
   //Adds a constant number of elements
   template <std::size_t N>
   struct incremental_resize {
     typedef std::size_t size_type;
   protected:
     bool resize_before_insert(size_type size, size_type capacity,
                               size_type n, size_type &new_size)
     {
       new_size = capacity + ((size+n-capacity+N-1)/N) * N;
       return new_size != capacity;
     }
     bool resize_after_erase(size_type, size_type, size_type&)
       { return false; }
   };
   //Doubles when out of space, never shrinks
   template <std::size_t N>
   struct multiply_resize {
     typedef std::size_t size_type;
   protected:
     bool resize_before_insert(size_type size, size_type capacity,
                               size_type n, size_type &new_size)
     {
       new_size = capacity ? capacity : 1;
       while (size+n >= new_size) new_size *= N;
       return new_size != capacity;
     }
     bool resize_after_erase(size_type, size_type, size_type&)
       { return false; }
   };
   //Doubles when out of space, shrinks when less than a quarter is used
   template <std::size_t N>
   struct multiply_divide_resize {
     typedef std::size_t size_type;
   protected:
     bool resize_before_insert(size_type size, size_type capacity,
                               size_type n, size_type &new_size)
     {
       new_size = capacity ? capacity : 1;
       while (size+n >= new_size) new_size *= N;
       return new_size != capacity;
     }
     bool resize_after_erase(size_type size, size_type capacity,
                              size_type &new_size)
     {
       const int N2 = N*2;
       new_size = capacity;
       while (size < new_size/N2) new_size /= N;
       return new_size != capacity;
     }
   };
   //Any other variant should come here
   //Including: dynamically reconfigurable resize strategy

template < class ResizePolicy = multiply_resize<2>, std::size_t initN = 0 >
struct variable_allocated_array : public ResizePolicy {
  typedef std::size_t    size_type;
  enum { static_init_size = initN };
};

template < class T, std::size_t initN,  class ResizePolicy, class Alloc >
struct policy_vector_storage<T,
         variable_allocated_array<ResizePolicy, initN>, Alloc >
  : public policy_vector_base<T,Alloc>
  , public variable_allocated_array<ResizePolicy, initN>
{
private:
  typedef policy_vector_base<T,Alloc>                    Base;
  typedef variable_allocated_array<ResizePolicy, initN>  storage_policy;
  typedef policy_vector_storage<T,storage_policy,Alloc>  Self;
  typedef std::size_t                                    size_type;
protected:
  size_type nelems, maxelems;
  T* elems;
public:
  // constructors
  explicit policy_vector_storage(const Alloc& a)
    : Base(a), nelems(0), maxelems(initN)
    , elems(maxelems ? allocate(maxelems) : 0)
    { }
  policy_vector_storage(size_type n, const T& value, const Alloc& a)
    : Base(a), nelems(n), maxelems(std::max(n,initN))
    , elems(maxelems ? allocate(maxelems) : 0)
    { std::uninitialized_fill_n(elems, nelems, value); }
  explicit policy_vector_storage(size_type n)
    : Base(Alloc()), nelems(n), maxelems(std::max(n,initN))
    , elems(maxelems ? allocate(maxelems) : 0)
    { std::uninitialized_fill_n(elems, nelems, T()); }
  policy_vector_storage(policy_vector_storage const& rhs)
    : Base(rhs.get_allocator())
    , nelems(rhs.nelems), maxelems(std::max(rhs.maxelems,initN))
    , elems(maxelems ? allocate(maxelems) : 0)
    { std::uninitialized_copy(rhs.elems, rhs.elems+nelems, elems); }
  template <class T2, std::size_t N>
  policy_vector_storage(T2 rhs[N], const Alloc& a)
    : Base(a), nelems(N), maxelems(std::max(initN,N))
    , elems(maxelems ? allocate(maxelems) : 0)
    { std::uninitialized_copy(rhs, rhs+N, elems); }
  template <class Iterator>
  policy_vector_storage(Iterator first, Iterator last, const Alloc& a)
    : Base(a)
    , nelems(std::distance(first, last)), maxelems(std::max(initN,nelems))
    , elems(maxelems ? allocate(maxelems) : 0)
    { std::uninitialized_copy(first, last, elems); }
  // destructor
  ~policy_vector_storage() {
    destroy(elems, elems+nelems); nelems = 0;
    deallocate(elems,maxelems); elems = 0;
  }
  // copy constructor
  policy_vector_storage& operator= (policy_vector_storage const& rhs) {
    nelems = rhs.nelems; maxelems = rhs.maxelems;
    elems = rhs.maxelems ? allocate(rhs.maxelems) : 0;
    std::copy(rhs.elems, rhs.elems+nelems, elems);
    return *this;
  }
  // size is not constant any more, so no more static
  size_type size() const { return nelems; }
  bool empty() const { return nelems == 0; }
  size_type max_size() const { return policy_vector_base<T,Alloc>::max_size(); }
protected:
  T* allocate_and_fill_n(size_type maxn, size_type n, const T& value) {
    T* result = allocate(maxn);
    try {
      std::uninitialized_fill_n(result, n, value);
      return result;
    }
    catch(...) {
      deallocate(result, maxn);
      throw;
    }
  }
  T* allocate_and_copy(size_type n, T* first, T* last) {
    T* result = allocate(n);
    try {
      std::uninitialized_copy(first, last, result);
      return result;
    }
    catch(...) {
      deallocate(result, n);
      throw;
    }
  }
public:
  // assign value to all elements
  void assign(const T& value) {
    std::fill_n(elems, nelems, value);
  }
  // assign value to all n elements -- changes size to n
  void assign(size_type n, const T& value) {
    if (n>nelems) {
      size_type maxn;
      if (resize_before_insert(nelems, maxelems, n-nelems, maxn)) {
        T* new_elems = allocate_and_fill_n(maxn, n, value);
        destroy(elems, elems+nelems);
        if (elems) deallocate(elems,maxelems);
        elems = new_elems;
        maxelems = maxn;
      } else {
        T* end = std::fill_n(elems, nelems, value);
        std::uninitialized_fill_n(end, n-nelems, value);
      }
      nelems = n;
    } else {
      T* new_end = std::fill_n(elems, n, value);
      destroy(new_end, elems+nelems);
      nelems = n;
    }
  }
  // assign range -- resize if necessary
  template <class RandomAccessIter>
  void assign(RandomAccessIter first, RandomAccessIter last) {
    size_type n = last-first;
    if (n>nelems) {
      size_type maxn;
      if (resize_before_insert(nelems, maxelems, n-nelems, maxn)) {
        T* new_elems = allocate_and_copy(maxn, &*first, &*last);
        destroy(elems, elems+nelems);
        if (elems) deallocate(elems,maxelems);
        elems = new_elems;
        maxelems = maxn;
      } else {
        T* end = std::copy(first, first+nelems, elems);
        std::uninitialized_copy(first + nelems, last, end);
      }
      nelems = n;
    } else {
      T* new_end = std::copy(first, last, elems);
      destroy(new_end, elems+nelems);
      nelems = n;
    }
  }
  // special members for variable size
  size_type capacity() const { return maxelems; }
  void reserve(size_type new_capacity) {
    // for obvious reasons, we refuse to resize to less than the actual
    // number of elements, yet we allow to reserve fewer elements than
    // current capacity, causing vector to shrink
    if (new_capacity != maxelems && new_capacity >= nelems) {
      T* new_elems = allocate_and_copy(new_capacity, elems, elems+nelems);
      destroy(elems, elems+nelems);
      if (elems) deallocate(elems,maxelems);
      elems = new_elems;
      maxelems = new_capacity;
    }
  }
  // swap (note: constant complexity)
  void swap (policy_vector_storage& y) {
    std::swap(elems, y.elems);
    std::swap(nelems, y.nelems);
    std::swap(maxelems, y.maxelems);
  }
};


// B. CHECKING POLICY

template <class size_type>
struct no_check {
  static void range_check (size_type, size_type) {}
};

template <class size_type>
struct exception_check {
  static void range_check (size_type i, size_type size) {
    if (i >= size)
      throw std::range_error("policy_vector: range error in checking\n");
  }
};

template <class size_type>
struct assert_check {
  static void range_check (size_type i, size_type size) {
    assert(i < size);
  }
};


// C. ITERATOR POLICY

// C.1 Raw pointer iterator: default iterator type

template <class T, class V>
class raw_pointer_iterator {
protected:
  // type definitions
  // typedef typename V::value_type T;
  typedef T*                     iterator;
  typedef const T*               const_iterator;
  typedef std::size_t            size_type;
  typedef std::ptrdiff_t         difference_type;
  // operators
  static iterator
  make_iterator(V* v, size_type n) {
    return (&v->front())+n;
  }
  static const_iterator
  make_const_iterator(const V* v, size_type n) {
    return (&v->front())+n;
  }
};

// C.2 Wrapped pointer iterator: a real class encapsulating a pointer,
// allows to check according to vector's checking policy

// needs a wrapper class for pair<policy_vector*,T*> that will perform the check
template <class V, class T, class Pointer, class Reference>
struct pointer_iterator_wrapper
{
  typedef T                                      value_type;
  typedef Pointer                                pointer;
  typedef Reference                              reference;
  typedef std::ptrdiff_t                         difference_type;
  typedef std::random_access_iterator_tag        iterator_category;
protected:
  typedef std::size_t                            size_type;
public:
  pointer_iterator_wrapper() : _M_v(0), _M_ptr(0) {}
  pointer_iterator_wrapper(V* v, size_type n) : _M_v(v), _M_ptr(base()+n) {}
  pointer_iterator_wrapper(V* v, Pointer ptr) : _M_v(v), _M_ptr(ptr) {}
  pointer_iterator_wrapper(pointer_iterator_wrapper const& w)
    : _M_v(w._M_v), _M_ptr(w._M_ptr) {}
  // operators
  Reference operator*()  const { return *check(_M_ptr); }
  Pointer   operator->() const { return check(_M_ptr); }
  Reference operator[](std::size_t n) const { return check(_M_ptr[n]); }
  pointer_iterator_wrapper& operator++() {
    ++_M_ptr; return *this;
  }
  pointer_iterator_wrapper operator++(int) {
    pointer_iterator_wrapper old(*this); ++_M_ptr; return *old;
  }
  pointer_iterator_wrapper& operator--() {
    --_M_ptr; return *this;
  }
  pointer_iterator_wrapper& operator--(int) {
    pointer_iterator_wrapper old(*this); --_M_ptr; return *old;
  }
  pointer_iterator_wrapper& operator+=(difference_type n) {
    _M_ptr += n; return *this;
  }
  pointer_iterator_wrapper& operator-=(difference_type n) {
    _M_ptr -= n; return *this;
  }
  // GCC 2.95 has a bug in its Koenig lookup, put as friend
  friend pointer_iterator_wrapper
  operator+(pointer_iterator_wrapper lhs, difference_type const& rhs) {
    return lhs += rhs;
  }
  friend pointer_iterator_wrapper
  operator+(difference_type const& lhs, pointer_iterator_wrapper rhs) {
    return rhs += lhs;
  }
  friend pointer_iterator_wrapper
  operator-(pointer_iterator_wrapper lhs, difference_type const& rhs) {
    return lhs -= rhs;
  }
  friend difference_type
  operator-(pointer_iterator_wrapper const& lhs, pointer_iterator_wrapper const& rhs) {
    // must belong to same vector: what do we do otherwise?
    return lhs._M_ptr - rhs._M_ptr;
  }
  friend bool 
  operator==(pointer_iterator_wrapper const& lhs, pointer_iterator_wrapper  const& rhs) {
    // must belong to same vector: what do we do otherwise?
    return lhs._M_ptr == rhs._M_ptr;
  }
  friend bool 
  operator!=(pointer_iterator_wrapper const& lhs, pointer_iterator_wrapper const& rhs) {
    // must belong to same vector: what do we do otherwise?
    return lhs._M_ptr != rhs._M_ptr;
  }
  friend bool 
  operator<(pointer_iterator_wrapper const& lhs, pointer_iterator_wrapper const& rhs) {
    // must belong to same vector: what do we do otherwise?
    return lhs._M_ptr < rhs._M_ptr;
  }
  friend bool 
  operator<=(pointer_iterator_wrapper const& lhs, pointer_iterator_wrapper const& rhs) {
    // must belong to same vector: what do we do otherwise?
    return lhs._M_ptr <= rhs._M_ptr;
  }
  friend bool 
  operator>(pointer_iterator_wrapper const& lhs, pointer_iterator_wrapper const& rhs) {
    // must belong to same vector: what do we do otherwise?
    return lhs._M_ptr > rhs._M_ptr;
  }
  friend bool 
  operator>=(pointer_iterator_wrapper const& lhs, pointer_iterator_wrapper const& rhs) {
    // must belong to same vector: what do we do otherwise?
    return lhs._M_ptr >= rhs._M_ptr;
  }
protected:
  Pointer base() const { return Pointer(&_M_v->front()); }
  Pointer check(Pointer ptr) const {
    _M_v->range_check(ptr-base(), _M_v->size());
    return ptr;
  }
private:
  V* _M_v; 
  Pointer _M_ptr;
};

template <class T, class V>
class checked_pointer_iterator {
protected:
  // type definitions
  // typedef typename V::value_type T;
  typedef pointer_iterator_wrapper<V,T,T*,T&>                  iterator;
  typedef pointer_iterator_wrapper<const V,T,const T*, const T&> const_iterator;
  typedef std::size_t                                          size_type;
  typedef std::ptrdiff_t                                       difference_type;
  // operators
  static iterator
  make_iterator(V* v, size_type n) {
    return iterator(v,n);
  }
  static const_iterator
  make_const_iterator(const V* v, size_type n) {
    return const_iterator(v,n);
  }
};

// C.3 Index iterator:

// needs a wrapper class for pair<policy_vector*,size_type>
template <class V, class T, class Pointer, class Reference>
struct index_iterator_wrapper
{
  typedef T                                      value_type;
  typedef Pointer                                pointer;
  typedef Reference                              reference;
  typedef std::ptrdiff_t                         difference_type;
  typedef std::random_access_iterator_tag        iterator_category;
protected:
  typedef std::size_t                            size_type;
public:
  index_iterator_wrapper() : _M_v(0), _M_n(0) {}
  index_iterator_wrapper(V* v, size_type n) : _M_v(v), _M_n(n) {}
  index_iterator_wrapper(index_iterator_wrapper const& w)
    : _M_v(w._M_v), _M_n(w._M_n) {}
  // operators
  Reference operator*()  const { check(); return *address(); }
  Pointer   operator->() const { check(); return address(); }
  Reference operator[](std::size_t n) const { check(n); return address()[n]; }
  index_iterator_wrapper& operator++() {
    ++_M_n; return *this;
  }
  index_iterator_wrapper operator++(int) {
    index_iterator_wrapper old(*this); ++_M_n; return *old;
  }
  index_iterator_wrapper& operator--() {
    --_M_n; return *this;
  }
  index_iterator_wrapper& operator--(int) {
    index_iterator_wrapper old(*this); --_M_n; return *old;
  }
  index_iterator_wrapper& operator+=(difference_type n) {
    _M_n += n; return *this;
  }
  index_iterator_wrapper& operator-=(difference_type n) {
    _M_n -= n; return *this;
  }
  // GCC 2.95 has a bug in its Koenig lookup, put as friend
  friend index_iterator_wrapper
  operator+(index_iterator_wrapper lhs, difference_type rhs) {
    return lhs += rhs;
  }
  friend index_iterator_wrapper
  operator+(difference_type lhs, index_iterator_wrapper rhs) {
    return rhs += lhs;
  }
  friend index_iterator_wrapper
  operator-(index_iterator_wrapper const& lhs, difference_type const& rhs) {
    return lhs -= rhs;
  }
  friend inline difference_type
  operator-(index_iterator_wrapper const& lhs, index_iterator_wrapper const& rhs) {
    // must belong to same vector: what do we do otherwise?
    return lhs._M_n - rhs._M_n;
  }
  friend inline bool 
  operator==(index_iterator_wrapper const& lhs, index_iterator_wrapper const& rhs) {
    // must belong to same vector: what do we do otherwise?
    return lhs._M_n == rhs._M_n;
  }
  friend inline bool 
  operator!=(index_iterator_wrapper const& lhs, index_iterator_wrapper const& rhs) {
    // must belong to same vector: what do we do otherwise?
    return lhs._M_n != rhs._M_n;
  }
  friend inline bool 
  operator<(index_iterator_wrapper const& lhs, index_iterator_wrapper const& rhs) {
    // must belong to same vector: what do we do otherwise?
    return lhs._M_n < rhs._M_n;
  }
  friend inline bool 
  operator<=(index_iterator_wrapper const& lhs, index_iterator_wrapper const& rhs) {
    // must belong to same vector: what do we do otherwise?
    return lhs._M_n <= rhs._M_n;
  }
  friend inline bool 
  operator>(index_iterator_wrapper const& lhs, index_iterator_wrapper const& rhs) {
    // must belong to same vector: what do we do otherwise?
    return lhs._M_n > rhs._M_n;
  }
  friend inline bool 
  operator>=(index_iterator_wrapper const& lhs, index_iterator_wrapper const& rhs) {
    // must belong to same vector: what do we do otherwise?
    return lhs._M_n >= rhs._M_n;
  }
protected:
  Pointer address() const { return (&_M_v->front()) + _M_n; }
  void check() const {
    _M_v->range_check(_M_n, _M_v->size());
  }
  void check(difference_type n) const {
    _M_v->range_check(_M_n+n, _M_v->size());
  }
private:
  V* _M_v; 
  size_type _M_n;
};

template <class T, class V>
class index_iterator {
protected:
  // type definitions
  // typedef typename V::value_type T;
  typedef index_iterator_wrapper<V,T,T*,T&>                    iterator;
  typedef index_iterator_wrapper<const V,T,const T*, const T&> const_iterator;
  typedef std::size_t                                          size_type;
  typedef std::ptrdiff_t                                       difference_type;
  // operators
  static iterator
  make_iterator(V* v, size_type n) {
    return iterator(v,n);
  }
  static const_iterator
  make_const_iterator(const V* v, size_type n) {
    return const_iterator(v,n);
  }
};



// Z. POLICY VECTOR:

template <
  class T,
  class StoragePolicy = variable_allocated_array<multiply_resize<2>, 0>,
  template <class S> class CheckingPolicy = no_check,
  template <class T2, class V2> class IteratorPolicy = raw_pointer_iterator,
  class Alloc = std::allocator<T>
  > 
class policy_vector
  : public policy_vector_storage< T, StoragePolicy, Alloc >
  , public CheckingPolicy<std::size_t>
  , public IteratorPolicy<T, 
             policy_vector<T,StoragePolicy,CheckingPolicy,IteratorPolicy,Alloc>
           >
{
protected:
  typedef policy_vector<T,StoragePolicy,CheckingPolicy,IteratorPolicy,Alloc>
                                                          Self;
  typedef policy_vector_storage<T,StoragePolicy,Alloc>    storage;
  typedef StoragePolicy                                   storage_policy;
  typedef CheckingPolicy<std::size_t>                     checking_policy;
  typedef IteratorPolicy<T,Self>                          iterator_policy;
public:
  // type definitions
  typedef Alloc                                           allocator_type;
  typedef T                                               value_type;
  typedef T&                                              reference;
  typedef const T&                                        const_reference;
  typedef typename iterator_policy::iterator              iterator;
  typedef typename iterator_policy::const_iterator        const_iterator;
  typedef std::size_t                                     size_type;
  typedef std::ptrdiff_t                                  difference_type;

  // constructors
  explicit policy_vector(const allocator_type& a = allocator_type())
    : storage(a) {}
  policy_vector(size_type n, const T& value,
                const allocator_type& a = allocator_type())
    : storage(n,value,a) {}
  explicit policy_vector(size_type n) : storage(n) {}
  policy_vector(const policy_vector &rhs) : storage(rhs) {}
  template <class Iterator>
  policy_vector(Iterator first, Iterator last,
                const allocator_type& a = allocator_type())
    : storage(first, last, a) {}
  // only for fixed_size array
  template <class T2>
  policy_vector(T2 rhs[static_size], const allocator_type& a = allocator_type())
    : storage(rhs, a) {}

  // copy constructor with conversion
  template < class T2,
             class StoragePolicy2,
             template <class S> class CheckingPolicy2,
             template <class T3, class V3> class IteratorPolicy2,
	     class Alloc2 >
  policy_vector(policy_vector<T2,StoragePolicy2,CheckingPolicy2,IteratorPolicy2,Alloc2> const& rhs)
    : storage(rhs)
    , storage_policy(rhs), checking_policy(rhs), iterator_policy(rhs)
    { assign(rhs.begin(), rhs.end()); }

  // assignment from array with type conversion, only for fixed-size arrays
  template <class T2>
  policy_vector& operator= (T2 rhs[static_size]) {
    assign(rhs, rhs+static_size);
    return *this;
  }

  // assignment from other policy_vector with type conversion
  template < class T2,
             class StoragePolicy2,
             template <class S> class CheckingPolicy2,
             template <class T3, class V3> class IteratorPolicy2,
	     class Alloc2 >
  policy_vector& operator=
    (const policy_vector<T2,StoragePolicy2,CheckingPolicy2,IteratorPolicy2,
                         Alloc2>& rhs)
  {
    // would do better with assign, but also need to copy policies
    Self tmp( rhs );
    swap( tmp );
    return *this;
  }

  // iterator support: from iterator_policy
  iterator begin() { return make_iterator(this,0); }
  const_iterator begin() const { return make_const_iterator(this,0); }
  iterator end() { return make_iterator(this,size()); }
  const_iterator end() const { return make_const_iterator(this,size()); }

  // size, empty, maxsize: from policy_vector_storage
  // capacity, reserve:  from policy_vector_storage (only if variable size)

  // reverse iterator support
  typedef std::reverse_iterator<iterator> reverse_iterator;
  typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

  reverse_iterator rbegin() { return reverse_iterator(end()); }
  const_reverse_iterator rbegin() const {
      return const_reverse_iterator(end());
  }
  reverse_iterator rend() { return reverse_iterator(begin()); }
  const_reverse_iterator rend() const {
      return const_reverse_iterator(begin());
  }

  // operator[]
  reference operator[](size_type i) {
    range_check(i, size());
    return elems[i];
  }
  const_reference operator[](size_type i) const {
    range_check(i, size());
    return elems[i];
  }

  // at() with range check
  reference at(size_type i) {
    if (i >= size()) { throw std::range_error("policy_vector: range error in at()\n"); }
    return elems[i];
  }
  const_reference at(size_type i) const {
    if (i >= size()) { throw std::range_error("policy_vector: range error in at()\n"); }
    return elems[i];
  }

  // front() and back()
  reference front() { return elems[0]; }
  const_reference front() const { return elems[0]; }
  reference back() { return elems[size()-1]; }
  const_reference back() const { return elems[size()-1]; }

  // insert() variants
  void insert(iterator pos, const T& x) {
    size_type index = pos - begin();
    if (index != nelems) {
      size_type new_capacity;
      if (resize_before_insert(size(), capacity(), 1, new_capacity))
        reserve(new_capacity);
      T* last = elems+nelems;
      std::copy_backward( elems+index, last, last+1);
      pos = begin()+index;
    }
    ++nelems;
    *pos = x;
  }
  template <class Iterator>
  void insert(iterator pos, Iterator first, Iterator last) {
    size_type n = std::distance(first, last);
    size_type index = pos - begin();
    if (index != nelems) {
      size_type new_capacity;
      if (resize_before_insert(size(), capacity(), n, new_capacity)) {
        reserve(new_capacity);
        pos = begin()+index;
      }
      T* last = elems+nelems;
      std::copy_backward( elems+index, last, last+n);
    }
    nelems += n;
    std::copy(first, last, pos);
  }
  void insert(iterator pos, size_type n, const T& x) {
    size_type index = pos - begin();
    if (index != nelems) {
      size_type new_capacity;
      if (resize_before_insert(size(), capacity(), n, new_capacity))
        reserve(new_capacity);
      T* last = elems+nelems;
      std::copy_backward( elems+index, last, last+n);
      pos = begin()+index;
    }
    nelems += n;
    std::fill_n(pos, n, x);
  }
  
  // erase() variants
  iterator erase(iterator position) {
    T* pos = &*position;
    std::copy( pos+1, elems+nelems, pos);
    --nelems;
    destroy(elems+nelems);
    size_type new_capacity;
    if (resize_after_erase(size(), capacity(), new_capacity)) {
      size_type index = pos-elems;
      reserve(new_capacity);
      position = begin()+index;
    }
    return position;
  }
  iterator erase(iterator first, iterator last) {
    T* start = &*first;
    T* finish = &*last;
    T* new_end = std::copy(finish, elems+nelems, start);
    destroy(new_end, elems+nelems);
    nelems -= (finish-start);
    size_type new_capacity;
    if (resize_after_erase(size(), capacity(), new_capacity)) {
      size_type index = start-elems;
      reserve(new_capacity);
      first = begin()+index;
    }
    return first;
  }
  
  // push_back() and pop_back()
  void push_back(T const& x) {
    size_type new_capacity;
    if (resize_before_insert(size(), capacity(), 1, new_capacity))
      reserve(new_capacity);
    construct(elems+nelems, x);
    ++nelems;
  }
  void pop_back()  {
    --nelems;
    destroy(elems+nelems);
    size_type new_size;
    if (resize_after_erase(size(), capacity(), new_size))
      reserve(new_size);
  }

  // assign one value to all elements -- static version
  // (see fixed-size policies)

  // resizing, paading with T if necessary
  void resize(size_type n, const T& x) {
    if (n > size())
      insert(end(), n-size(), x);
    else
      erase(elems+n, end());
  }
  void resize(size_type n) {
    if (n > size())
      insert(end(), n-size(), T());
    else
      erase(elems+n, end());
  }

  // comparisons -- global operators
  friend  bool operator== (const policy_vector& x, const policy_vector& y) {
    return std::equal(x.begin(), x.end(), y.begin());
  }
  
  friend bool operator< (const policy_vector& x, const policy_vector& y) {
      return std::lexicographical_compare(x.begin(),x.end(),y.begin(),y.end());
  }
  friend inline bool operator!= (const policy_vector& x, const policy_vector& y) {
      return !(x==y);
  }
  friend inline bool operator> (const policy_vector& x, const policy_vector& y) {
      return y<x;
  }
  friend inline bool operator<= (const policy_vector& x, const policy_vector& y) {
    return !(y<x);
  }
  friend inline bool operator>= (const policy_vector& x, const policy_vector& y) {
      return !(x<y);
  }
  
  // global swap()
  friend inline void swap (policy_vector& x, policy_vector& y) {
    x.swap(y);
  }

};

#endif /*CS903_POLICY_VECTOR_HPP*/

