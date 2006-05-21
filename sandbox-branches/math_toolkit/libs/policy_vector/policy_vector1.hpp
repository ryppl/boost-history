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

template <class T, std::size_t N>
class fixed_aggregate_array {
  public:
    typedef std::size_t    size_type;
    T elems[N];    // fixed-size policy_vector of elements of type T
    // constructors and destructor
    fixed_aggregate_array () {}
    fixed_aggregate_array (T rhs[N]) { std::copy(rhs, rhs+N, elems); }
    // size is constant
    static size_type size() { return N; }
    static bool empty() { return false; }
    static size_type max_size() { return N; }
    enum { static_size = N };
};

template <class T, std::size_t N>
class fixed_allocated_array {
  public:
    typedef std::size_t    size_type;
    T* elems;    // fixed-size policy_vector of elements of type T
    // constructors and destructor
    fixed_allocated_array () : elems( new T[N] ) {}
    fixed_allocated_array (T rhs[N]) : elems( new T[N] ) {
      std::copy(rhs, rhs+N, elems);
    }
    ~fixed_allocated_array () { delete[] elems; }
    // size is constant
    static size_type size() { return N; }
    static bool empty() { return false; }
    static size_type max_size() { return N; }
    enum { static_size = N };
};

template < class T, std::size_t N,
	   template <class T2,std::size_t N> class StoragePolicy >
class policy_vector : public StoragePolicy<T,N> {
  public:
    // type definitions
    typedef T              value_type;
    typedef T*             iterator;
    typedef const T*       const_iterator;
    typedef T&             reference;
    typedef const T&       const_reference;
    typedef std::ptrdiff_t difference_type;

    typedef StoragePolicy<T,N> storage_policy;
    typedef typename storage_policy::size_type size_type;

    // constructors
    policy_vector() : storage_policy() {}
    policy_vector(T rhs[N]) : storage_policy(rhs) {}

    // iterator support
    iterator begin() { return elems; }
    const_iterator begin() const { return elems; }
    iterator end() { return elems+static_size; }
    const_iterator end() const { return elems+static_size; }

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
    void swap (policy_vector<T,N,StoragePolicy>& y) {
        std::swap_ranges(begin(),end(),y.begin());
    }

    // direct access to data
    const T* data() const { return elems; }

    // assignment with type conversion
    template <typename T2>
    policy_vector<T,N,StoragePolicy>&
    operator= (const policy_vector<T2,N,StoragePolicy>& rhs) {
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
template<class T, std::size_t N,
	 template <class T2,std::size_t N> class StoragePolicy >
bool operator== (const policy_vector<T,N,StoragePolicy>& x,
                 const policy_vector<T,N,StoragePolicy>& y) {
    return std::equal(x.begin(), x.end(), y.begin());
}
template<class T, std::size_t N,
	 template <class T2,std::size_t N> class StoragePolicy >
bool operator< (const policy_vector<T,N,StoragePolicy>& x,
                const policy_vector<T,N,StoragePolicy>& y) {
    return std::lexicographical_compare(x.begin(),x.end(),y.begin(),y.end());
}
template<class T, std::size_t N,
	 template <class T2,std::size_t N> class StoragePolicy >
bool operator!= (const policy_vector<T,N,StoragePolicy>& x,
                const policy_vector<T,N,StoragePolicy>& y) {
    return !(x==y);
}
template<class T, std::size_t N,
	 template <class T2,std::size_t N> class StoragePolicy >
bool operator> (const policy_vector<T,N,StoragePolicy>& x,
                const policy_vector<T,N,StoragePolicy>& y) {
    return y<x;
}
template<class T, std::size_t N,
	 template <class T2,std::size_t N> class StoragePolicy >
bool operator<= (const policy_vector<T,N,StoragePolicy>& x,
                 const policy_vector<T,N,StoragePolicy>& y) {
    return !(y<x);
}
template<class T, std::size_t N,
	 template <class T2,std::size_t N> class StoragePolicy >
bool operator>= (const policy_vector<T,N,StoragePolicy>& x,
                 const policy_vector<T,N,StoragePolicy>& y) {
    return !(x<y);
}

// global swap()
template<class T, std::size_t N,
	 template <class T2,std::size_t N> class StoragePolicy >
inline void swap (policy_vector<T,N,StoragePolicy>& x,
                  policy_vector<T,N,StoragePolicy>& y) {
    x.swap(y);
}

#endif /*CS903_POLICY_VECTOR_HPP*/

