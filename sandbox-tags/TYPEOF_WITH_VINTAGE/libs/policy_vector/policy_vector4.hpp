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

// A. STORAGE POLICIES:

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
  typedef policy_vector_storage<T, fixed_aggregate_array<N> > Self;
  T elems[N];
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
  typedef policy_vector_storage<T, fixed_allocated_array<N> > Self;
  T* elems;
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
   struct double_resize {
     typedef std::size_t size_type;
     bool resize_before_insert(size_type size, size_type capacity,
                               size_type n, size_type &new_size)
     {
       new_size = capacity ? capacity : 1;
       while (size+n >= new_size) new_size *= 2;
       return new_size != capacity;
     }
     bool resize_after_erase(size_type, size_type, size_type&)
       { return false; }
   };
   //Doubles when out of space, shrinks when less than a quarter is used
   struct halfdouble_resize {
     typedef std::size_t size_type;
     bool resize_before_insert(size_type size, size_type capacity,
                               size_type n, size_type &new_size)
     {
       new_size = capacity ? capacity : 1;
       while (size+n >= new_size) new_size *= 2;
       return new_size != capacity;
     }
     bool resize_after_erase(size_type size, size_type capacity,
                              size_type &new_size)
     {
       new_size = capacity;
       while (size < new_size/4) new_size /= 2;
       return new_size != capacity;
     }
   };
   //Any other variant should come here

template < class ResizePolicy = double_resize, std::size_t initN = 0 >
struct variable_allocated_array : public ResizePolicy {
  typedef std::size_t    size_type;
  enum { static_init_size = initN };
};

template < class T, std::size_t initN,  class ResizePolicy >
struct policy_vector_storage<T, variable_allocated_array<ResizePolicy, initN> >
{
  typedef policy_vector_storage<T, variable_allocated_array<ResizePolicy, initN> > Self;
  typedef std::size_t    size_type;
  size_type nelems, maxelems;
  T* elems;
  // constructors and destructor
  policy_vector_storage()
    : nelems(0), maxelems(initN), elems(initN ? new T[initN] : 0) {}
  policy_vector_storage(Self const& rhs)
    : nelems(rhs.nelems), maxelems(rhs.maxelems)
    , elems(rhs.maxelems ? new T[rhs.maxelems] : 0)
    { std::copy(rhs.elems, rhs.elems+nelems, elems); }
  policy_vector_storage(size_type n)
    : nelems(n), maxelems(n), elems(new T[maxelems])
    {}
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
  Self& operator= (Self const& rhs) {
    nelems = rhs.nelems; maxelems = rhs.maxelems;
    elems = rhs.maxelems ? new T[rhs.maxelems] : 0;
    std::copy(rhs.elems, rhs.elems+nelems, elems);
    return *this;
  }
  // size is not constant any more, so we return a reference
  size_type& size() { return nelems; }
  size_type size() const { return nelems; }
  bool empty() const { return nelems == 0; }
  static size_type max_size() { return size_type(-1) / sizeof(T); }
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
};




// Z. POLICY VECTOR:

template < class T, class StoragePolicy >
class policy_vector
  : public policy_vector_storage< T, StoragePolicy >
  , public StoragePolicy
{
  public:
    // type definitions
    typedef T              value_type;
    typedef T*             iterator;
    typedef const T*       const_iterator;
    typedef T&             reference;
    typedef const T&       const_reference;
    typedef std::size_t    size_type;
    typedef std::ptrdiff_t difference_type;

    typedef policy_vector_storage<T,StoragePolicy> storage;
    typedef StoragePolicy  storage_policy;

    // constructors
    policy_vector() : storage() {}
    policy_vector(const policy_vector<T,StoragePolicy> &rhs) : storage(rhs) {}
    policy_vector(size_type n) : storage() {}
    policy_vector(size_type n, const T& x) : storage() { assign(x); }
    policy_vector(T rhs[static_size]) : storage(rhs) {}
    template <class Iterator>
    policy_vector(Iterator first, Iterator last) : storage(first, last) {}

    // iterator support
    iterator begin() { return elems; }
    const_iterator begin() const { return elems; }
    iterator end() { return elems+size(); }
    const_iterator end() const { return elems+size(); }

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
    reference operator[](size_type i) { return elems[i]; }
    const_reference operator[](size_type i) const { return elems[i]; }

    // at() with range check
    reference at(size_type i) { rangecheck(i); return elems[i]; }
    const_reference at(size_type i) const { rangecheck(i); return elems[i]; }

    // front() and back()
    reference front() { return elems[0]; }
    const_reference front() const { return elems[0]; }
    reference back() { return elems[size()-1]; }
    const_reference back() const { return elems[size()-1]; }

    // insert() variants
    void insert(iterator pos, const T& x) {
      if (pos != end()) {
	size_type new_capacity;
        if (resize_before_insert(size(), capacity(), 1, new_capacity)) {
	  size_type index = pos - begin();
	  reserve(new_capacity); // note: this may invalidate pos
	  pos = begin() + index;
	}
	std::copy_backward( pos, end(), end()+1);
      }
      *pos = x;
      ++size();
    }
    template <class Iterator>
    void insert(iterator pos, Iterator first, Iterator last) {
      size_type n = std::distance(first, last);
      if (pos != end()) {
	size_type new_capacity;
        if (resize_before_insert(size(), capacity(), n, new_capacity)) {
	  size_type index = pos - begin();
	  reserve(new_capacity); // note: this may invalidate pos
	  pos = begin() + index;
	}
	std::copy_backward( pos, end(), end()+n);
      }
      std::copy(first, last, pos);
      size() += n;
    }
    void insert(iterator pos, size_type n, const T& x) {
      if (pos != end()) {
	size_type new_capacity;
        if (resize_before_insert(size(), capacity(), n, new_capacity)) {
	  size_type index = pos - begin();
	  reserve(new_capacity); // note: this may invalidate pos
	  pos = begin() + index;
	}
	std::copy_backward( pos, end(), end()+n);
      }
      std::fill_n(pos, n, x);
      size() += n;
    }
    
    // erase() variants
    void erase(iterator pos) {
      std::copy( pos+1, end(), pos);
      --size();
      size_type new_capacity;
      if (resize_after_erase(size(), capacity(), new_capacity))
	reserve(new_capacity);
    }
     void erase(iterator first, iterator last) {
      std::copy( last, end(), first);
      size() -= (last - first);
      size_type new_capacity;
      if (resize_after_erase(size(), capacity(), new_capacity))
	reserve(new_capacity);
    }
    
    
    // push_back() and pop_back()
    void push_back(T const& x) {
      size_type new_capacity;
      if (resize_before_insert(size(), capacity(), 1, new_capacity))
	reserve(new_capacity);
      elems[size()] = x;
      ++size();
    }
    void pop_back()  {
      size_type new_size;
      --size();
      if (resize_after_erase(size(), capacity(), new_size))
	reserve(new_size);
    }

    // swap (note: linear complexity)
    void swap (policy_vector<T,StoragePolicy>& y) {
        std::swap_ranges(begin(),end(),y.begin());
    }

    // direct access to data
    const T* data() const { return elems; }

    // assignment with type conversion
    template <typename T2>
    policy_vector<T,StoragePolicy>&
    operator= (const policy_vector<T2,StoragePolicy>& rhs) {
        std::copy(rhs.begin(),rhs.end(), begin());
        return *this;
    }

    // assign one value to all elements
    void assign (const T& value)
    {
        std::fill_n(begin(),size(),value);
    }

  private:
    // check range (may be private because it is static)
    static void rangecheck (size_type i) {
        if (i >= size()) { throw std::range_error("policy_vector"); }
    }

};

// comparisons
template<class T, class StoragePolicy >
bool operator== (const policy_vector<T,StoragePolicy>& x,
                 const policy_vector<T,StoragePolicy>& y) {
    return std::equal(x.begin(), x.end(), y.begin());
}
template<class T, class StoragePolicy >
bool operator< (const policy_vector<T,StoragePolicy>& x,
                const policy_vector<T,StoragePolicy>& y) {
    return std::lexicographical_compare(x.begin(),x.end(),y.begin(),y.end());
}
template<class T, class StoragePolicy >
bool operator!= (const policy_vector<T,StoragePolicy>& x,
                const policy_vector<T,StoragePolicy>& y) {
    return !(x==y);
}
template<class T, class StoragePolicy >
bool operator> (const policy_vector<T,StoragePolicy>& x,
                const policy_vector<T,StoragePolicy>& y) {
    return y<x;
}
template<class T, class StoragePolicy >
bool operator<= (const policy_vector<T,StoragePolicy>& x,
                 const policy_vector<T,StoragePolicy>& y) {
    return !(y<x);
}
template<class T, class StoragePolicy >
bool operator>= (const policy_vector<T,StoragePolicy>& x,
                 const policy_vector<T,StoragePolicy>& y) {
    return !(x<y);
}

// global swap()
template<class T, class StoragePolicy >
inline void swap (policy_vector<T,StoragePolicy>& x,
                  policy_vector<T,StoragePolicy>& y) {
    x.swap(y);
}

#endif /*CS903_POLICY_VECTOR_HPP*/

