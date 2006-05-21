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
#include <string>

// Global declaration of policy_vector

template < class T, class StoragePolicy,
           template <class S> class CheckingPolicy,
           template <class T2, class V2> class IteratorPolicy> 
class policy_vector;

// A. STORAGE POLICIES:
// Provides all the functions related to storage, which do not depend on
// the type T of elements stored. For the functions which depend on T,
// they are part of a policy_vector_storage class. To define those
// functions, one must specialize policy_vector_storage

template < class T, class StoragePolicy>
struct policy_vector_storage {};

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

template < class T, std::size_t N>
struct policy_vector_storage<T, fixed_aggregate_array<N> >
{
private:
  typedef policy_vector_storage<T, fixed_aggregate_array<N> > Self;
protected:
  T elems[N];
public:
  // constructors and destructor
  policy_vector_storage() {}
  policy_vector_storage(Self const& rhs)
    { std::copy(rhs.elems, rhs.elems+N, elems); }
  policy_vector_storage(std::size_t) {}
  policy_vector_storage(T rhs[N])
    { std::copy(rhs, rhs+N, elems); }
  template <class Iterator>
  policy_vector_storage(Iterator first, Iterator last)
    {
      if (std::distance(first, last) < N)
	std::copy(first, last, elems);
      else
	std::copy_n(first, N, elems);
    }
  // default copy constructor
  // assign a range -- leave others unchanged if necessary
  template <class RandomAccessIter>
  void assign(RandomAccessIter first, RandomAccessIter last) {
    std::copy_n(first, std::min(last-first, (std::ptrdiff_t)N), elems);
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

template < class T, std::size_t N>
struct policy_vector_storage<T, fixed_allocated_array<N> >
{
private:
  typedef policy_vector_storage<T, fixed_allocated_array<N> > Self;
protected:
  T* elems;
public:
  // constructors and destructor
  policy_vector_storage() : elems( new T[N] ) {}
  policy_vector_storage(Self const& rhs) : elems( new T[N] ) 
    { std::copy(rhs.elems, rhs.elems+N, elems); }
  policy_vector_storage(std::size_t) {}
  policy_vector_storage(T rhs[N]) : elems( new T[N] )
    { std::copy(rhs, rhs+N, elems); }
  template <class Iterator>
  policy_vector_storage(Iterator first, Iterator last) : elems(new T[N])
    {
      if (std::distance(first, last) < N)
	std::copy(first, last, elems);
      else
	std::copy_n(first, N, elems);
    }
  ~policy_vector_storage() { delete[] elems; }
  // copy constructor
  policy_vector_storage& operator=(Self const& rhs) {
    elems = new T[N]; 
    std::copy(rhs.elems, rhs.elems+N, elems);
    return *this;
  }
  // assign a range -- leave others unchanged if necessary
  template <class RandomAccessIter>
  void assign(RandomAccessIter first, RandomAccessIter last) {
    std::copy_n(first, std::min(last-first, (std::ptrdiff_t)N), elems);
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

template < class T, std::size_t initN,  class ResizePolicy >
struct policy_vector_storage<T, variable_allocated_array<ResizePolicy, initN> >
{
private:
  // typedef policy_vector_storage<T,variable_allocated_array<ResizePolicy, initN> > Self;
  typedef std::size_t    size_type;
protected:
  size_type nelems, maxelems;
  T* elems;
public:
  // constructors and destructor
  policy_vector_storage()
    : nelems(0), maxelems(initN)
    , elems(maxelems ? new T[maxelems] : 0) {}
  policy_vector_storage(policy_vector_storage const& rhs)
    : nelems(rhs.nelems), maxelems(std::max(rhs.maxelems,initN))
    , elems(maxelems ? new T[maxelems] : 0)
    { std::copy(rhs.elems, rhs.elems+nelems, elems); }
  policy_vector_storage(size_type n)
    : nelems(n), maxelems(std::max(n,initN))
    , elems(maxelems ? new T[maxelems] : 0) {}
  template <size_type N>
  policy_vector_storage(T rhs[N])
    : nelems(N), maxelems(std::max(initN,N))
    , elems(maxelems ? new T[maxelems] : 0)
    { std::copy(rhs, rhs+N, elems); }
  template <class Iterator>
  policy_vector_storage(Iterator first, Iterator last)
    : nelems(std::distance(first, last)), maxelems(std::max(initN,nelems))
    , elems(maxelems ? new T[maxelems] : 0)
    { std::copy(first, last, elems); }
  ~policy_vector_storage() { if (elems) delete[] elems; elems = 0; }
  // copy constructor
  policy_vector_storage& operator= (policy_vector_storage const& rhs) {
    nelems = rhs.nelems; maxelems = rhs.maxelems;
    elems = rhs.maxelems ? new T[rhs.maxelems] : 0;
    std::copy(rhs.elems, rhs.elems+nelems, elems);
    return *this;
  }
  // size is not constant any more, so we return a reference
  size_type size() const { return nelems; }
  bool empty() const { return nelems == 0; }
  static size_type max_size() { return size_type(-1) / sizeof(T); }
  // assign one value to all elements -- does not change size
  void assign (const T& value) {
    std::fill_n(elems, nelems, value);
  }
  // assign a range -- resize if necessary
  template <class RandomAccessIter>
  void assign(RandomAccessIter first, RandomAccessIter last) {
    reserve(last-first);
    std::copy(first, last, elems);
  }
  // special for variable size
  size_type capacity() const { return maxelems; }
  void reserve(size_type new_capacity) {
    // for obvious reasons, we refuse to resize to less than the actual
    // number of elements
    if (new_capacity != capacity() && new_capacity >= nelems) {
      T* new_elems = new T[new_capacity];
      maxelems = new_capacity;
      std::copy(elems, elems+nelems, new_elems);
      T* old_elems = elems;
      elems = new_elems;
      if (old_elems) delete[] old_elems;
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
    if (i >= size) {
      std::string msg = "policy_vector: range error in checking\n";
      throw std::range_error(msg);
    }
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

template < class T,
           class StoragePolicy,
           template <class S> class CheckingPolicy,
	   template <class T2, class V2> class IteratorPolicy> 
class policy_vector
  : public policy_vector_storage< T, StoragePolicy >
  , public StoragePolicy
  , public CheckingPolicy<std::size_t>
  , public IteratorPolicy<T, policy_vector<T,StoragePolicy,CheckingPolicy,IteratorPolicy> >
{
protected:
  typedef policy_vector<T,StoragePolicy,CheckingPolicy,IteratorPolicy> Self;
  typedef policy_vector_storage<T,StoragePolicy>    storage;
  typedef StoragePolicy                             storage_policy;
  typedef CheckingPolicy<std::size_t>               checking_policy;
  typedef IteratorPolicy<T,Self>                    iterator_policy;
public:
  // type definitions
  typedef T                                         value_type;
  typedef T&                                        reference;
  typedef const T&                                  const_reference;
  typedef typename iterator_policy::iterator        iterator;
  typedef typename iterator_policy::const_iterator  const_iterator;
  typedef std::size_t                               size_type;
  typedef std::ptrdiff_t                            difference_type;

  // constructors
  policy_vector() : storage() {}
  policy_vector(const policy_vector &rhs) : storage(rhs) {}
  policy_vector(size_type n) : storage(n) {}
  policy_vector(size_type n, const T& x) : storage(n) { assign(x); }
  template <class Iterator>
  policy_vector(Iterator first, Iterator last) : storage(first, last) {}

  // constructor with conversion?

  // should be part of the conversion policy?
  // only defined for fixed_size_array
  policy_vector(T rhs[static_size]) : storage(rhs) {}

  // iterator support: from iterator_policy
  iterator begin() { return make_iterator(this,0); }
  const_iterator begin() const { return make_const_iterator(this,0); }
  iterator end() { return make_iterator(this,size()); }
  const_iterator end() const { return make_const_iterator(this,size()); }

  // size

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
      if (resize_before_insert(size(), capacity(), n, new_capacity))
        reserve(new_capacity);
      T* last = elems+nelems;
      std::copy_backward( elems+index, last, last+n);
      pos = begin()+index;
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
  void erase(iterator pos) {
    std::copy( pos+1, end(), pos);
    --nelems;
    size_type new_capacity;
    if (resize_after_erase(size(), capacity(), new_capacity))
      reserve(new_capacity);
  }
   void erase(iterator first, iterator last) {
    std::copy( last, end(), first);
    nelems -= (last - first);
    size_type new_capacity;
    if (resize_after_erase(size(), capacity(), new_capacity))
      reserve(new_capacity);
  }
  
  
  // push_back() and pop_back()
  void push_back(T const& x) {
    size_type new_capacity;
    if (resize_before_insert(size(), capacity(), 1, new_capacity))
      reserve(new_capacity);
    elems[nelems] = x; // will not perform checking
    ++nelems;
  }
  void pop_back()  {
    size_type new_size;
    --nelems;
    if (resize_after_erase(size(), capacity(), new_size))
      reserve(new_size);
  }

  // assignment with type conversion
  template <typename T2, class StoragePolicy2,
	    template <class S> class CheckingPolicy2,
	    template <class T3, class V3> class IteratorPolicy2>
  policy_vector&
  operator= (const policy_vector<T2, StoragePolicy2, CheckingPolicy2, IteratorPolicy2>& rhs) {
    ::storage_policy  = rhs; // StoragePolicy2 to storage_policy conversion
    ::checking_policy = rhs; // CheckingPolicy2 to checking_policy conversion
    ::iterator_policy = rhs; // IteratorPolicy2 to iterator_policy conversion
    assign(rhs.begin(), rhs.end()); // Range assignment -- see storage_policy
    return *this;
  }

  // assign one value to all elements
  void assign (const T& value) {
      std::fill_n(begin(), size(), value);
  }

  // assign a range -- behavior depends on storage policy
  template <class RandomAccessIter>
  void assign(RandomAccessIter first, RandomAccessIter last) {
    storage::assign(first, last);
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

