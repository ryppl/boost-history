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
  T elems[N];
  // constructors and destructor
  policy_vector_storage() {}
  policy_vector_storage(T rhs[N]) {
    std::copy(rhs, rhs+N, elems);
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
  T* elems;
  // constructors and destructor
  policy_vector_storage() : elems( new T[N] ) {}
  policy_vector_storage(T rhs[N]) : elems( new T[N] ) {
    std::copy(rhs, rhs+N, elems);
  }
  ~policy_vector_storage() { delete[] elems; }
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
    policy_vector(T rhs[static_size]) : storage(rhs) {}

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
    reference back() { return elems[static_size-1]; }
    const_reference back() const { return elems[static_size-1]; }

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

