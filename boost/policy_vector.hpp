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
 * 20 May 2002 - External storage policies + external_iterator_adaptor (Herve)
 * 19 May 2002 - Policy initialization in constructors (Herve)
 * 5  May 2002 - STL-ization and namespace boost (Herve)
 * 3  May 2002 - Allocators, destructor (Herve)
 * 30 Apr 2002 - Introduction of iterator policies (Herve)
 * 17 Apr 2002 - Introduction of checking policies (Herve)
 * 10 Apr 2002 - Introduction of a variable-size policy (Herve)
 * 10 Apr 2002 - Introduction of two fixed-size policies (Herve)
 * 10 Apr 2002 - Initial steal from Boost::array (Herve)
 */

#ifndef BOOST_POLICY_VECTOR_HPP
#define BOOST_POLICY_VECTOR_HPP

#include <cstddef>
#include <stdexcept>
#include <iterator>
#include <algorithm>
#include <memory>

#include <boost/iterator_adaptors.hpp>

namespace boost {

// Global declaration of policy_vector

template < class T,
           class Alloc,
           class StoragePolicy,
           template <class S> class CheckingPolicy,
           template <class T2, class V2> class IteratorPolicy
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
  void construct(T* ptr, T const& x) { alloc.construct(ptr, x); }
  void destroy(T* ptr) { alloc.destroy(ptr); }
  void destroy(T* first, T* last) {
    while (first!=last) { alloc.destroy(first); ++first; }
  }
private:
  Alloc alloc;
};

// PRELUDE. ALLOCATOR POLICIES: usually Alloc should be just a
// standard-compliant allocator, but sometimes it is not even justified.
// For this, use instead

typedef int no_allocator_policy;

// A. STORAGE POLICIES:
// Provides all the functions related to storage, which do not depend on
// the type T of elements stored. For the functions which depend on T,
// they are part of a policy_vector_storage class. To define those
// functions, one must specialize policy_vector_storage

template < class T, class Alloc, class StoragePolicy >
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

// FIXME remove allocators and replace by 
// template < class T, std::size_t N >
// struct policy_vector_storage<T, no_allocator_policy, fixed_aggregate_array<N> >
template < class T, class Null, std::size_t N >
struct policy_vector_storage<T, Null, fixed_aggregate_array<N> >
  : public fixed_aggregate_array<N>
{
private:
  // FIXME replace by: typedef no_allocator_policy Alloc;
  // it's just a problem with the test suite, is all...
  typedef Null                                            Alloc;
  typedef fixed_aggregate_array<N>                        storage_policy;
  typedef policy_vector_storage<T, Alloc, storage_policy> self_type;
protected:
  T elems[N];
public:
  // constructors and destructor
  policy_vector_storage(const Alloc&, const storage_policy&)
    {}
  policy_vector_storage(std::size_t n, const T& value,
                        const Alloc&, const storage_policy&)
    { std::fill_n(elems, std::min(n,N), value); }
  policy_vector_storage(std::size_t, const Alloc&, const storage_policy&)
    {}
  policy_vector_storage(self_type const& rhs)
    { std::copy(rhs.elems, rhs.elems+N, elems); }
  template <class T2>
  policy_vector_storage(T2 rhs[N], const Alloc&, const storage_policy&)
    { std::copy(rhs, rhs+N, elems); }
  template <class Iterator>
  policy_vector_storage(Iterator first, Iterator last, const Alloc&,
                        const storage_policy&)
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
    std::copy(first, first+std::min(last-first, (std::ptrdiff_t)N), elems);
  }
  // swap (note: linear complexity)
  void swap (self_type& y) {
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

template < class T, class Alloc, std::size_t N >
struct policy_vector_storage<T, Alloc, fixed_allocated_array<N> >
  : public policy_vector_base<T,Alloc>
  , public fixed_allocated_array<N>
{
private:
  typedef policy_vector_base<T,Alloc>                     base_type;
  typedef fixed_allocated_array<N>                        storage_policy;
  typedef policy_vector_storage<T, Alloc, storage_policy> self_type;
protected:
  T* elems;
public:
  // constructors
  policy_vector_storage(const Alloc& a, const storage_policy&)
    : base_type(a), elems(allocate(N))
    {}
  policy_vector_storage(std::size_t n, const T& value,
                        const Alloc& a, const storage_policy&)
    : base_type(a), elems(allocate(N))
    { std::uninitialized_fill_n(elems, std::min(n,N), value); }
  policy_vector_storage(std::size_t, const Alloc& a, const storage_policy&)
    : base_type(a), elems(allocate(N))
    { std::uninitialized_fill_n(elems, N, T()); }
  policy_vector_storage(const self_type& rhs)
    : base_type(rhs.get_allocator()), elems(allocate(N)) 
    { std::uninitialized_copy(rhs.elems, rhs.elems+N, elems); }
  template <class T2>
  policy_vector_storage(T2 rhs[N], const Alloc& a, const storage_policy&)
    : base_type(a), elems(allocate(N))
    {
      T* end = std::uninitialized_copy(rhs, rhs+N, elems);
      std::uninitialized_fill(end, elems+N, T());
    }
  template <class Iterator>
  policy_vector_storage(Iterator first, Iterator last,
                        const Alloc& a, const storage_policy&)
    : base_type(a), elems(allocate(N))
    {
      if (std::distance(first, last) < N) {
        T* end = std::uninitialized_copy(first, last, elems);
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
  self_type& operator=(self_type const& rhs) {
    if (&rhs == this) return *this;
    std::copy(rhs.elems, rhs.elems+N, elems);
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
  // swap (note: constant complexity)
  void swap (self_type& y) {
    T* tmp = y.elems; y.elems = elems; elems = tmp;
  }
};

// A3. Fixed static external array storage policy: suggested by Victor Wagner.
// Stores a pointer to an array which is not owned by the policy_vector
// but whose size is known at compile time

template <class T, std::size_t N>
struct fixed_external_static_array {
  typedef std::size_t size_type;
  // Need a default constructor (unsafe), and a range constructor (safe)
  fixed_external_static_array() : elems(0) {}
  fixed_external_static_array(T (&ptr)[N]) : elems(ptr) {}
  // size is constant
  static size_type size() { return N; }
  static bool empty() { return false; }
  static size_type max_size() { return N; }
  enum { static_size = N };
protected:
  T* elems;
};

template < class T, class Null, std::size_t N>
struct policy_vector_storage<T, Null,
                             fixed_external_static_array<T, N> >
  : public fixed_external_static_array<T, N>
{
private:
  // typedef no_allocator_policy                             Alloc;
  typedef Null                                            Alloc;
  typedef fixed_external_static_array<T, N>               storage_policy;
  typedef policy_vector_storage<T, Alloc, storage_policy> self_type;
public:
  // constructors
  policy_vector_storage(const Alloc&, const storage_policy& sp)
    : storage_policy(sp)
    {}
  policy_vector_storage(std::size_t n, const T& value,
                        const Alloc&, const storage_policy& sp)
    : storage_policy(sp)
    { std::fill_n(elems, std::min(n,N), value); }
  policy_vector_storage(std::size_t, const Alloc&, storage_policy& sp)
    : storage_policy(sp)
    { std::fill_n(elems, N, T()); }
  policy_vector_storage(const self_type& rhs)
    : storage_policy(rhs)
    {} // no need to copy since point to the same array!
  template <class T2>
  policy_vector_storage(T2 rhs[N], const Alloc&, const storage_policy& sp)
    : storage_policy(sp)
    { std::copy(rhs, rhs+N, elems); }
  template <class Iterator>
  policy_vector_storage(Iterator first, Iterator last,
                        const Alloc&, const storage_policy& sp)
    : storage_policy(sp)
    {
      if (std::distance(first, last) < N) {
        T* end = std::copy(first, last, elems);
        std::fill(end, elems+N, T());
      } else
        std::copy(first, first+N, elems);
    }
  // destructor
  ~policy_vector_storage() {}
  // copy constructor
  self_type& operator=(self_type const& rhs) {
    if (&rhs == this) return *this;
    elems = rhs.elems;
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
  // swap (note: constant complexity)
  void swap (self_type& y) {
    T* tmp = y.elems; y.elems = elems; elems = tmp;
  }
};

// A4. Fixed external range storage policy, an outgrowth of the previous
// Stores a a range which is not owned by the policy_vector
// Iterator needs to a be a model of RandomAccessIterator

template <class Iterator, class ConstIterator>
struct fixed_external_range {
  typedef std::size_t                                        size_type;
  typedef Iterator                                           iterator;
  typedef ConstIterator                                      const_iterator;
  // Need a default constructor (unsafe) and a range constructor (safe)
  fixed_external_range() : elems(0), N(0) {}
  fixed_external_range(Iterator first, Iterator last)
    : elems(first), N(last-first) {}
  fixed_external_range(fixed_external_range const& sp)
    : elems(sp.elems), N(sp.N) {}
  // size is constant, but not static
  size_type size() const { return N; }
  bool empty() const { return N != 0; }
  size_type max_size() const { return N; }
protected:
  Iterator elems;
  size_type N;
  // enum { static_size = INT_MAX };
  // FIXME dirty workaround for compilers which try to instanciate:
  //   template <class T2> policy_vector(T rhs[static_size], ...);
  // even though it shouldn't do that if the constructor isn't called.
  // I need to understand what happens better and make sure I didn't do
  // anything wrong...
};

template < class T, class Null, class Iterator, class ConstIterator>
struct policy_vector_storage<T, Null,
	                     fixed_external_range<Iterator,ConstIterator> >
  : public fixed_external_range<Iterator,ConstIterator>
{
private:
  // typedef no_allocator_policy                                Alloc;
  typedef Null                                               Alloc;
  typedef policy_vector_base<T,Alloc>                        base_type;
  typedef fixed_external_range<Iterator,ConstIterator>       storage_policy;
  typedef policy_vector_storage<T, Alloc, storage_policy >   self_type;
public:
  // constructors
  policy_vector_storage(const Alloc&, const storage_policy& sp)
    : storage_policy(sp)
    {}
  policy_vector_storage(std::size_t n, const T& value,
                        const Alloc&, const storage_policy& sp)
    : storage_policy(sp)
    { std::fill_n(elems, std::min(n,N), value); }
  policy_vector_storage(std::size_t, const storage_policy& sp)
    : storage_policy(sp)
    { std::fill_n(elems, N, T()); }
  policy_vector_storage(const self_type& rhs)
    : storage_policy(rhs)
    { /* std::copy(rhs.elems, rhs.elems+N, elems); */ }
  template <class Iterator2>
  policy_vector_storage(Iterator2 first, Iterator2 last,
                        const Alloc&, const storage_policy& sp)
    : storage_policy(sp)
    {
      if (size_t(0)+std::distance(first, last) < N) {
        Iterator end = std::copy(first, last, elems);
        std::fill(end, elems+N, T());
      } else
        std::copy(first, first+N, elems);
    }
  // destructor
  ~policy_vector_storage() {}
  // copy constructor
  self_type& operator=(self_type const& rhs) {
    if (&rhs == this) return *this;
    storage_policy() = rhs;
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
  template <class Iterator2>
  void assign(Iterator2 first, Iterator2 last) {
    std::copy(first, first+std::min(last-first, (std::ptrdiff_t)N), elems);
  }
  // swap (note: constant complexity)
  void swap (self_type& y) {
    Iterator tmp = y.elems; y.elems = elems; elems = tmp;
    typename storage_policy::size_type tmpN = y.N; y.N = N; N = tmpN;
  }
};

// A5. Variable size allocated array storage policy
// Starts with an initial size, and grows from there
// The growth is controlled by the resizing policy

   // A31. Resizing policies for variable_allocated_array
   // resize_before_insert(size, capacity, n, new_capacity) returns true if
   // resizing may be needed after inserting n elements,
   // and new_capacity stores the new capacity after the resizing
  
   //Adds a constant number of elements
   template <std::size_t N>
   struct incremental_resize {
     typedef std::size_t size_type;
     bool resize_before_insert(size_type size, size_type capacity,
                               size_type n, size_type &new_capacity)
     {
       new_capacity = capacity + ((size+n-capacity+N-1)/N) * N;
       return new_capacity != capacity;
     }
     bool resize_after_erase(size_type, size_type, size_type&)
       { return false; }
   };
   //Doubles when out of space, never shrinks
   template <std::size_t N>
   struct multiply_resize {
     typedef std::size_t size_type;
     bool resize_before_insert(size_type size, size_type capacity,
                               size_type n, size_type &new_capacity)
     {
       new_capacity = capacity ? capacity : 1;
       while (size+n > new_capacity) new_capacity *= N;
       return new_capacity != capacity;
     }
     bool resize_after_erase(size_type, size_type, size_type&)
       { return false; }
   };
   //Doubles when out of space, shrinks when less than a quarter is used
   template <std::size_t N>
   struct multiply_divide_resize {
     typedef std::size_t size_type;
     bool resize_before_insert(size_type size, size_type capacity,
                               size_type n, size_type &new_capacity)
     {
       new_capacity = capacity ? capacity : 1;
       while (size+n > new_capacity) new_capacity *= N;
       return new_capacity != capacity;
     }
     bool resize_after_erase(size_type size, size_type capacity,
                              size_type &new_capacity)
     {
       const size_type N2 = N*N;
       new_capacity = capacity;
       while (size < new_capacity/N2) new_capacity /= N;
       return new_capacity != capacity;
     }
   };
   //Any other variant should come here
   //Including: dynamically reconfigurable resize strategy

template < class ResizePolicy = multiply_resize<2>, std::size_t initN = 0 >
struct variable_allocated_array : protected ResizePolicy {
  enum { static_init_size = initN };
};

template < class T, class Alloc,  class ResizePolicy, std::size_t initN >
struct policy_vector_storage<T, Alloc,
         variable_allocated_array<ResizePolicy, initN> >
  : public policy_vector_base<T,Alloc>
  , public variable_allocated_array<ResizePolicy, initN>
{
private:
  typedef policy_vector_base<T,Alloc>                    base_type;
  typedef variable_allocated_array<ResizePolicy, initN>  storage_policy;
  typedef policy_vector_storage<T,Alloc,storage_policy>  self_type;
  typedef std::size_t                                    size_type;
protected:
  size_type nelems, maxelems;
  T* elems;
public:
  // constructors
  policy_vector_storage(const Alloc& a, const storage_policy& sp)
    : base_type(a), storage_policy(sp), nelems(0), maxelems(initN)
    , elems(maxelems ? allocate(maxelems) : 0)
    {}
  policy_vector_storage(size_type n, const T& value,
                        const Alloc& a, const storage_policy& sp)
    : base_type(a), storage_policy(sp), nelems(n), maxelems(std::max(n,initN))
    , elems(maxelems ? allocate(maxelems) : 0)
    { std::uninitialized_fill_n(elems, nelems, value); }
  policy_vector_storage(size_type n, const Alloc& a, const storage_policy& sp)
    : base_type(a), storage_policy(sp) , nelems(n), maxelems(std::max(n,initN))
    , elems(maxelems ? allocate(maxelems) : 0)
    { std::uninitialized_fill_n(elems, nelems, T()); }
  policy_vector_storage(self_type const& rhs)
    : base_type(rhs.get_allocator()), storage_policy(rhs)
    , nelems(rhs.nelems), maxelems(std::max(rhs.maxelems,initN))
    , elems(maxelems ? allocate(maxelems) : 0)
    { std::uninitialized_copy(rhs.elems, rhs.elems+nelems, elems); }
  template <class Iterator>
  policy_vector_storage(Iterator first, Iterator last,
                        const Alloc& a, const storage_policy& sp)
    : base_type(a), storage_policy(sp)
    , nelems(std::distance(first, last)), maxelems(std::max(initN,nelems))
    , elems(maxelems ? allocate(maxelems) : 0)
    { std::uninitialized_copy(first, last, elems); }
  // destructor
  ~policy_vector_storage() {
    destroy(elems, elems+nelems); nelems = 0;
    deallocate(elems,maxelems); elems = 0;
  }
  // copy constructor
  policy_vector_storage& operator= (self_type const& rhs) {
    if (&rhs == this) return *this;
    destroy(elems, elems+nelems);
    if (elems) deallocate(elems, maxelems);
    nelems = rhs.nelems; maxelems = rhs.maxelems;
    elems = rhs.maxelems ? allocate(rhs.maxelems) : 0;
    std::copy( rhs.elems, rhs.elems+nelems, elems );
    // what's wrong with using assign()? (testsuite says BROKEN)
    // assign<T*>(rhs.elems, rhs.elems+nelems);
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
  template <class InputIter>
  T* allocate_and_copy(size_type n, InputIter first, InputIter last) {
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
  template <class InputIter>
  void assign(InputIter first, InputIter last) {
    size_type n = std::distance(first, last);
    if (n>nelems) {
      size_type maxn;
      if (resize_before_insert(nelems, maxelems, n-nelems, maxn)) {
        T* new_elems = allocate_and_copy(maxn, first, last);
        destroy(elems, elems+nelems);
        if (elems) deallocate(elems,maxelems);
        elems = new_elems;
        maxelems = maxn;
      } else {
        // would be more efficient with copy_n here!
        InputIter middle = first;
	std::advance(middle, nelems);
        std::copy(first, middle, elems);
        std::uninitialized_copy(middle, last, elems+nelems);
      }
      nelems = n;
    } else {
      std::copy(first, last, elems);
      destroy(elems+n, elems+nelems);
      nelems = n;
      size_type new_capacity;
      if (resize_after_erase(nelems, maxelems, new_capacity))
        reserve(new_capacity);
    }
  }
  // special members for variable size
  size_type capacity() const { return maxelems; }
  void reserve(size_type new_capacity) {
    // for obvious reasons, we refuse to resize to less than the actual
    // number of elements, yet we allow to reserve fewer elements than
    // current capacity, causing vector to shrink
    if (new_capacity == 0) {
      destroy(elems, elems+nelems);
      if (elems) deallocate(elems,maxelems);
      elems = 0;
      maxelems = 0;
    } else if (new_capacity != maxelems && new_capacity >= nelems) {
      T* new_elems = allocate_and_copy(new_capacity, elems, elems+nelems);
      destroy(elems, elems+nelems);
      if (elems) deallocate(elems,maxelems);
      elems = new_elems;
      maxelems = new_capacity;
    }
  }
  // swap (note: constant complexity)
  void swap (self_type& y) {
    // incomprehensible: std::swap wil generate weird error messages
    // with GCC 2.95.2
    T* tmp = y.elems; y.elems = elems; elems = tmp;
    size_type ntmp = y.nelems; y.nelems = nelems; nelems = ntmp;
    size_type maxtmp = y.maxelems; y.maxelems = maxelems; maxelems = maxtmp;
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

// C.2 Wrapped pointer iterator: a class encapsulating a pointer,
// allows to check according to vector's checking policy

// in namespace detail?
#include <boost/policy_vector/pointer_iterator_wrapper.hpp>

// Since we use the superb library Boost.iterator_adaptor for reverse
// iterator below, might as well use an adapter instead of these 
// fat wrapper classes. Something like:
// template class T, class V>
// class pointer_iterator_policies {};

template <class T, class V>
class pointer_iterator {
protected:
  // type definitions
  // typedef typename V::value_type T; // does not compile
  typedef pointer_iterator_wrapper<V,T,T*,T&,false>     iterator;
  typedef pointer_iterator_wrapper<const V,T,const T*,const T&,false>
                                                        const_iterator;
  typedef std::size_t                                   size_type;
  typedef std::ptrdiff_t                                difference_type;
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

template <class T, class V>
class checked_pointer_iterator {
protected:
  // type definitions
  // typedef typename V::value_type T; // does not compile
  typedef pointer_iterator_wrapper<V,T,T*,T&,true>      iterator;
  typedef pointer_iterator_wrapper<const V,T,const T*,const T&,true>
                                                        const_iterator;
  typedef std::size_t                                   size_type;
  typedef std::ptrdiff_t                                difference_type;
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

// C.3 Index iterator: encapsulates an offset
// allows to check according to vector's checking policy

// in namespace detail?
#include <boost/policy_vector/index_iterator_wrapper.hpp>

// Same here, something like:
// template class T, class V>
// class index_iterator_policies {};

template <class T, class V>
class index_iterator {
protected:
  // type definitions
  // typedef typename V::value_type T; // does not compile
  // typedef typename V::allocator_type::pointer          pointer;
  // typedef typename V::allocator_type::const_pointer    const_pointer;
  typedef index_iterator_wrapper<V,T,T*,T&,false>      iterator;
  typedef index_iterator_wrapper<const V,T,const T*,const T&,false>
                                                       const_iterator;
  typedef std::size_t                                  size_type;
  typedef std::ptrdiff_t                               difference_type;
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

template <class T, class V>
class checked_index_iterator {
protected:
  // type definitions
  // typedef typename V::value_type T; // does not compile
  typedef index_iterator_wrapper<V,T,T*,T&,true>      iterator;
  typedef index_iterator_wrapper<const V,T,const T*,const T&,true>
                                                       const_iterator;
  typedef typename iterator::size_type                 size_type;
  typedef typename iterator::difference_type           difference_type;
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

// C.4 Iterator wrapper: for use with fixed_external_range policy
// allows to check according to vector's checking policy

template <class T, class V>
class external_iterator_adaptor;

// iterator_adaptor needs to access V::storage::iterator and
// V::storage::const_iterator types, but that does not compile since V
// is only instantiated after checked_iterator_adaptor, and does not yet
// have member V::storage.
//
// We resolve the dilemma by providing a partial specialization for
// checked_iterator_adaptor, just for the fixed_external_range storage
// policy, which gives us access to Iterator and ConstIterator as
// template parameters.

template <class T, class Alloc,
  class Iterator, class ConstIterator,
  template <class S> class CheckingPolicy,
  template <class T2, class V2> class IteratorPolicy >
class external_iterator_adaptor<T,
	policy_vector<T, Alloc,
	              fixed_external_range<Iterator,ConstIterator>,
	              CheckingPolicy,
		      IteratorPolicy>
        >
{
private:
  typedef fixed_external_range<Iterator,ConstIterator>       storage_policy;
  typedef policy_vector<T, Alloc,
	                fixed_external_range<Iterator,ConstIterator>,
	                CheckingPolicy,
		        IteratorPolicy>                      V;

protected:
  // type definitions
  typedef pointer_iterator_wrapper<V,T,Iterator,T&,false>     iterator;
  typedef pointer_iterator_wrapper<const V,T,ConstIterator,const T&,false>
                                                             const_iterator;
  typedef typename storage_policy::size_type                 size_type;
  typedef typename iterator::difference_type                 difference_type;
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

// Same for checked version

template <class T, class V>
class checked_external_iterator_adaptor;

template <class T, class Alloc,
  class Iterator, class ConstIterator,
  template <class S> class CheckingPolicy,
  template <class T2, class V2> class IteratorPolicy >
class checked_external_iterator_adaptor<T,
	policy_vector<T, Alloc,
	              fixed_external_range<Iterator,ConstIterator>,
	              CheckingPolicy,
		      IteratorPolicy>
        >
{
private:
  typedef fixed_external_range<Iterator,ConstIterator>       storage_policy;
  typedef policy_vector<T, Alloc,
	                fixed_external_range<Iterator,ConstIterator>,
	                CheckingPolicy,
		        IteratorPolicy>                      V;

protected:
  // type definitions
  typedef pointer_iterator_wrapper<V,T,Iterator,T&,true>     iterator;
  typedef pointer_iterator_wrapper<const V,T,ConstIterator,const T&,true>
                                                             const_iterator;
  typedef typename storage_policy::size_type                 size_type;
  typedef typename iterator::difference_type                 difference_type;
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
  class Alloc = std::allocator<T>,
  class StoragePolicy = variable_allocated_array<multiply_resize<2>, 0>,
  template <class S> class CheckingPolicy = no_check,
  template <class T2, class V2> class IteratorPolicy = raw_pointer_iterator
  > 
class policy_vector
  : public policy_vector_storage<T,Alloc,StoragePolicy>
  , public CheckingPolicy<std::size_t>
  , public IteratorPolicy<T, 
             policy_vector<T,Alloc,StoragePolicy,CheckingPolicy,IteratorPolicy>
           >
{
  typedef policy_vector_storage<T,Alloc,StoragePolicy>    storage;
  typedef policy_vector<T,Alloc,StoragePolicy,CheckingPolicy,IteratorPolicy>
                                                          self_type;
public:
 //policies
  typedef StoragePolicy                                   storage_policy;
  typedef CheckingPolicy<std::size_t>                     checking_policy;
  typedef IteratorPolicy<T,self_type>                     iterator_policy;
  // type definitions
  typedef Alloc                                           allocator_type;
  typedef T                                               value_type;
  typedef typename allocator_type::pointer                pointer;
  typedef typename allocator_type::const_pointer          const_pointer;
  typedef typename allocator_type::reference              reference;
  typedef typename allocator_type::const_reference        const_reference;
  typedef typename iterator_policy::iterator              iterator;
  typedef typename iterator_policy::const_iterator        const_iterator;
  typedef typename iterator_policy::size_type             size_type;
  typedef typename iterator_policy::difference_type       difference_type;

  // constructors
  explicit policy_vector(const allocator_type& a = allocator_type(),
                         const storage_policy& sp = storage_policy(),
                         const checking_policy& cp = checking_policy(),
                         const iterator_policy& ip = iterator_policy())
    : storage(a, sp), checking_policy(cp), iterator_policy(ip) {}
  policy_vector(size_type n, const T& value,
                         const allocator_type& a = allocator_type(),
                         const storage_policy& sp = storage_policy(),
                         const checking_policy& cp = checking_policy(),
                         const iterator_policy& ip = iterator_policy())
    : storage(n, value, a, sp), checking_policy(cp), iterator_policy(ip) {}
  explicit policy_vector(size_type n,
                         const allocator_type& a = allocator_type(),
                         const storage_policy& sp = storage_policy(),
                         const checking_policy& cp = checking_policy(),
                         const iterator_policy& ip = iterator_policy())
    : storage(n, a, sp), checking_policy(cp), iterator_policy(ip) {}
  template <class Iterator>
  policy_vector(Iterator first, Iterator last,
                         const allocator_type& a = allocator_type(),
                         const storage_policy& sp = storage_policy(),
                         const checking_policy& cp = checking_policy(),
                         const iterator_policy& ip = iterator_policy())
    : storage(first, last, a, sp), checking_policy(cp), iterator_policy(ip) {}

  // only for fixed_size array
  template <class T2>
  policy_vector(T2 rhs[storage_policy::static_size],
                         const allocator_type& a = allocator_type(),
                         const storage_policy& sp = storage_policy(),
                         const checking_policy& cp = checking_policy(),
                         const iterator_policy& ip = iterator_policy())
    : storage(rhs, a, sp), checking_policy(cp), iterator_policy(ip) {}

  // copy constructors, with or without conversion
  policy_vector(const policy_vector &rhs)
    : storage(rhs), checking_policy(rhs), iterator_policy(rhs)
    { assign(rhs.begin(), rhs.end()); }
  template < class T2,
	     class Alloc2,
             class StoragePolicy2,
             template <class S> class CheckingPolicy2,
             template <class T3, class V3> class IteratorPolicy2 >
  explicit policy_vector(
             policy_vector<T2,Alloc2,
                           StoragePolicy2,CheckingPolicy2,IteratorPolicy2>
	     const& rhs)
    : storage(rhs), checking_policy(rhs), iterator_policy(rhs)
    { assign(rhs.begin(), rhs.end()); }

  // assignment from array with type conversion, only for fixed-size arrays
  // which provide static_size (otherwise, won't compile)
  template <class T2>
  policy_vector& operator= (T2 rhs[storage_policy::static_size]) {
    assign(rhs, rhs + storage_policy::static_size);
    return *this;
  }

  // assignment from other policy_vector with type conversion
  template < class T2,
	     class Alloc2,
             class StoragePolicy2,
             template <class S> class CheckingPolicy2,
             template <class T3, class V3> class IteratorPolicy2 >
  policy_vector& operator=
    (const policy_vector<T2,Alloc2,
                         StoragePolicy2,CheckingPolicy2,IteratorPolicy2>& rhs)
  {
    storage() = rhs;
    checking_policy() = rhs;
    iterator_policy() = rhs;
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
  typedef typename boost::reverse_iterator_generator<iterator, T,
                     reference, pointer,
		     std::random_access_iterator_tag, difference_type>::type
	  reverse_iterator;
  typedef typename boost::reverse_iterator_generator<const_iterator, T,
                     const_reference, const_pointer,
		     std::random_access_iterator_tag, difference_type>::type
	  const_reverse_iterator;

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

#if defined(__GNUC__) && (__GNUC__) < 3
  // GCC 2.95 has a bug in its Koenig lookup
  bool operator==(const policy_vector& y) const {
    return (size() == y.size()) && std::equal(elems, elems+size(), y.elems);
  }
  bool operator!=(const policy_vector& y) const {
    return !(*this == y);
  }
  bool operator<(const policy_vector& y) const {
    return std::lexicographical_compare(elems, elems+size(),
                                        y.elems, y.elems+y.size());
  }
  bool operator>(const policy_vector& y) const {
    return y < *this;
  }
  bool operator<=(const policy_vector& y) const {
    return !(y < *this);
  }
  bool operator>=(const policy_vector& y) const {
      return !(*this < y);
  }
#else
  // comparisons -- global operators
  friend inline bool
  operator== (const policy_vector& x, const policy_vector& y) {
    return std::equal(x.elems, x.elems+x.size(), y.elems);
  }
  friend inline bool
  operator!=(const policy_vector& x, const policy_vector& y) {
    return !(x==y);
  }
  friend inline bool
  operator<(const policy_vector& x, const policy_vector& y) {
      return std::lexicographical_compare(x.elems, x.elems+x.size(),
                                          y.elems, y.elems+y.size());
  }
  friend inline bool
  operator<=(const policy_vector& x, const policy_vector& y) {
    return !(y<x);
  }
  friend inline bool
  operator>(const policy_vector& x, const policy_vector& y) {
    return y<x;
  }
  friend inline bool
  operator>=(const policy_vector& x, const policy_vector& y) {
    return !(x<y);
  }
#endif  

  // global swap()
  friend inline void swap (policy_vector& x, policy_vector& y) {
    x.swap(y);
  }

  // ONLY FOR VARIABLE-SIZE ARRAYS

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
    size_type n = last-first;
    if (n == 0) return first;
    T* start = &*first; // is always dereferenceable so no problem
    T* finish = start+n; // &*last would generate an exception if checking!
    T* new_end = std::copy(finish, elems+nelems, start);
    destroy(new_end, elems+nelems);
    nelems -= n;
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
    size_type new_capacity;
    if (resize_after_erase(size(), capacity(), new_capacity))
      reserve(new_capacity);
  }

  // assign() (from storage base class)

  // resizing, padding with T if necessary
  void resize(size_type n, const T& x) {
    if (n > size())
      insert(end(), n-size(), x);
    else
      erase(begin()+n, end());
  }
  void resize(size_type n) {
    if (n > size())
      insert(end(), n-size(), T());
    else
      erase(begin()+n, end());
  }

  // clear()
  void clear() {
    destroy(elems, elems+nelems);
    nelems = 0;
    size_type new_capacity;
    if (resize_after_erase(size(), capacity(), new_capacity))
      reserve(new_capacity);
  }


};

} // namespace boost

#endif /* BOOST_POLICY_VECTOR_HPP */

