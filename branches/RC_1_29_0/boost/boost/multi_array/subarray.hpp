// Copyright (C) 2002 Ronald Garcia
//
// Permission to copy, use, sell and distribute this software is granted
// provided this copyright notice appears in all copies. 
// Permission to modify the code and to distribute modified code is granted
// provided this copyright notice appears in all copies, and a notice 
// that the code was modified is included with the copyright notice.
//
// This software is provided "as is" without express or implied warranty, 
// and with no claim as to its suitability for any purpose.
//

#ifndef SUBARRAY_RG071801_HPP
#define SUBARRAY_RG071801_HPP

//
// subarray.hpp - used to implement standard operator[] on
// multi_arrays
//

#include "boost/multi_array/base.hpp"
#include "boost/multi_array/concept_checks.hpp"
#include "boost/limits.hpp"
#include "boost/type.hpp"
#include <algorithm>
#include <cstddef>
#include <functional>

namespace boost {
namespace detail {
namespace multi_array {

//
// const_sub_array
//    multi_array's proxy class to allow multiple overloads of
//    operator[] in order to provide a clean multi-dimensional array 
//    interface.
template <typename T, std::size_t NumDims, typename TPtr>
class const_sub_array :
  public boost::detail::multi_array::multi_array_impl_base<T,NumDims>
{
  typedef boost::detail::multi_array::multi_array_impl_base<T,NumDims> super_type;
public: 
  typedef typename super_type::value_type value_type;
  typedef typename super_type::const_reference const_reference;
  typedef typename super_type::const_iterator const_iterator;
  typedef typename super_type::const_iter_base const_iter_base;
  typedef typename super_type::const_reverse_iterator const_reverse_iterator;
  typedef typename super_type::element element;
  typedef typename super_type::size_type size_type;
  typedef typename super_type::difference_type difference_type;
  typedef typename super_type::index index;
  typedef typename super_type::extent_range extent_range;

  // template typedefs
  template <std::size_t NDims>
  struct const_array_view {
    typedef boost::detail::multi_array::const_multi_array_view<T,NDims> type;
  };

  template <std::size_t NDims>
  struct array_view {
    typedef boost::detail::multi_array::multi_array_view<T,NDims> type;
  };

  // Allow default copy constructor as well.

  template <typename OPtr>
  const_sub_array (const const_sub_array<T,NumDims,OPtr>& rhs) :
    base_(rhs.base_), extents_(rhs.extents_), strides_(rhs.strides_),
    index_base_(rhs.index_base_) {
  }

  // const_sub_array always returns const types, regardless of its own
  // constness.
  const_reference operator[](index idx) const {
    return super_type::access(boost::type<const_reference>(),
                              idx,base_,shape(),strides(),index_bases());
  }
  
  template <typename IndexList>
  const element& operator()(const IndexList& indices) const {
    return super_type::access_element(boost::type<const element&>(),
                                      origin(),
                                      indices,strides());
  }

  // see generate_array_view in base.hpp
#if !defined(BOOST_MSVC) || BOOST_MSVC > 1300
  template <int NDims>
#else
  template <int NumDims, int NDims> // else ICE
#endif // BOOST_MSVC
  typename const_array_view<NDims>::type 
  operator[](const boost::detail::multi_array::
             index_gen<NumDims,NDims>& indices)
    const {
    typedef typename const_array_view<NDims>::type return_type;
    return
      super_type::generate_array_view(boost::type<return_type>(),
                                      indices,
                                      shape(),
                                      strides(),
                                      index_bases(),
                                      base_);
  }

  template <typename OPtr>
  bool operator<(const const_sub_array<T,NumDims,OPtr>& rhs) const {
    return std::lexicographical_compare(begin(),end(),rhs.begin(),rhs.end());
  }

  template <typename OPtr>
  bool operator==(const const_sub_array<T,NumDims,OPtr>& rhs) const {
    if(std::equal(shape(),shape()+num_dimensions(),rhs.shape()))
      return std::equal(begin(),end(),rhs.begin());
    else return false;
  }

  template <typename OPtr>
  bool operator!=(const const_sub_array<T,NumDims,OPtr>& rhs) const {
    return !(*this == rhs);
  }

  template <typename OPtr>
  bool operator>(const const_sub_array<T,NumDims,OPtr>& rhs) const {
    return rhs < *this;
  }

  template <typename OPtr>
  bool operator<=(const const_sub_array<T,NumDims,OPtr>& rhs) const {
    return !(*this > rhs);
  }

  template <typename OPtr>
  bool operator>=(const const_sub_array<T,NumDims,OPtr>& rhs) const {
    return !(*this < rhs);
  }

  const_iterator begin() const {
    return const_iterator(const_iter_base(*index_bases(),origin(),
                                   shape(),strides(),index_bases()));
  }

  const_iterator end() const {
    return const_iterator(const_iter_base(*index_bases()+*shape(),origin(),
                                   shape(),strides(),index_bases()));
  }

  const_reverse_iterator rbegin() const {
    return const_reverse_iterator(end());
  }

  const_reverse_iterator rend() const {
    return const_reverse_iterator(begin());
  }

  TPtr origin() const { return base_; }
  size_type size() const { return extents_[0]; }
  size_type max_size() const { return num_elements(); }
  bool empty() const { return size() == 0; }
  size_type num_dimensions() const { return NumDims; }
  const size_type*  shape() const { return extents_; }
  const index* strides() const { return strides_; }
  const index* index_bases() const { return index_base_; }

  size_type num_elements() const { 
    return std::accumulate(shape(),shape() + num_dimensions(),
                           size_type(1), std::multiplies<size_type>());
  }


#ifndef BOOST_NO_MEMBER_TEMPLATE_FRIENDS
protected:
  template <typename,std::size_t> friend class value_accessor_n;  
  template <typename,std::size_t,typename> friend class const_sub_array;
#else    
public:  // Should be protected
#endif

  const_sub_array (TPtr base,
                 const size_type* extents,
                 const index* strides,
                 const index* index_base) :
    base_(base), extents_(extents), strides_(strides),
    index_base_(index_base) {
  }

  TPtr base_;
  const size_type* extents_;
  const index* strides_;
  const index* index_base_;
private:
  // const_sub_array cannot be assigned to (no deep copies!)
  const_sub_array& operator=(const const_sub_array&);
};

//
// sub_array
//    multi_array's proxy class to allow multiple overloads of
//    operator[] in order to provide a clean multi-dimensional array 
//    interface.
template <typename T, std::size_t NumDims>
class sub_array : public const_sub_array<T,NumDims,T*>
{
  typedef const_sub_array<T,NumDims,T*> super_type;
public: 
  typedef typename super_type::element element;
  typedef typename super_type::reference reference;
  typedef typename super_type::index index;
  typedef typename super_type::size_type size_type;
  typedef typename super_type::iterator iterator;
  typedef typename super_type::reverse_iterator reverse_iterator;
  typedef typename super_type::iter_base iter_base;
  typedef typename super_type::const_reference const_reference;
  typedef typename super_type::const_iterator const_iterator;
  typedef typename super_type::const_reverse_iterator const_reverse_iterator;
  typedef typename super_type::const_iter_base const_iter_base;

  // template typedefs
  template <std::size_t NDims>
  struct const_array_view {
    typedef boost::detail::multi_array::const_multi_array_view<T,NDims> type;
  };

  template <std::size_t NDims>
  struct array_view {
    typedef boost::detail::multi_array::multi_array_view<T,NDims> type;
  };

  // Assignment from other ConstMultiArray types.
  template <typename ConstMultiArray>
  sub_array& operator=(const ConstMultiArray& other) {
    function_requires< boost::detail::multi_array::ConstMultiArrayConcept< 
        ConstMultiArray, NumDims> >();

    // make sure the dimensions agree
    assert(other.num_dimensions() == num_dimensions());
    assert(std::equal(other.shape(),other.shape()+num_dimensions(),
                      shape()));
    // iterator-based copy
    std::copy(other.begin(),other.end(),begin());
    return *this;
  }


  sub_array& operator=(const sub_array& other) {
    if (&other != this) {
      // make sure the dimensions agree
      assert(other.num_dimensions() == num_dimensions());
      assert(std::equal(other.shape(),other.shape()+num_dimensions(),
                        shape()));
      // iterator-based copy
      std::copy(other.begin(),other.end(),begin());
    }
    return *this;
  }

  T* origin() { return base_; }
  const T* origin() const { return base_; }

  reference operator[](index idx) {
    return super_type::access(boost::type<reference>(),
                              idx,base_,shape(),strides(),index_bases());
  }

  // see generate_array_view in base.hpp
#if !defined(BOOST_MSVC) || BOOST_MSVC > 1300
  template <int NDims>
#else
  template <int NumDims, int NDims> // else ICE
#endif // BOOST_MSVC
  typename array_view<NDims>::type 
  operator[](const boost::detail::multi_array::
             index_gen<NumDims,NDims>& indices) {
    typedef typename array_view<NDims>::type return_type;
    return
      super_type::generate_array_view(boost::type<return_type>(),
                                      indices,
                                      shape(),
                                      strides(),
                                      index_bases(),
                                      origin());
  }

  template <class IndexList>
  element& operator()(const IndexList& indices) {
    return super_type::access_element(boost::type<element&>(),
                                      origin(),
                                      indices,strides());
  }

  iterator begin() {
    return iterator(iter_base(*index_bases(),origin(),
                                   shape(),strides(),index_bases()));
  }

  iterator end() {
    return iterator(iter_base(*index_bases()+*shape(),origin(),
                                   shape(),strides(),index_bases()));
  }

  // RG - rbegin() and rend() written naively to thwart MSVC ICE.
  reverse_iterator rbegin() {
    reverse_iterator ri(end());
    return ri;
  }

  reverse_iterator rend() {
    reverse_iterator ri(begin());
    return ri;
  }

  //
  // proxies
  //

  template <class IndexList>
  const element& operator()(const IndexList& indices) const {
    return super_type::operator()(indices);
  }

  const_reference operator[](index idx) const {
    return super_type::operator[](idx);
  }

  // see generate_array_view in base.hpp
#if !defined(BOOST_MSVC) || BOOST_MSVC > 1300
  template <int NDims>
#else
  template <int NumDims, int NDims> // else ICE
#endif // BOOST_MSVC
  typename const_array_view<NDims>::type 
  operator[](const boost::detail::multi_array::
             index_gen<NumDims,NDims>& indices)
    const {
    return super_type::operator[](indices);
  }

  const_iterator begin() const {
    return super_type::begin();
  }
  
  const_iterator end() const {
    return super_type::end();
  }

  const_reverse_iterator rbegin() const {
    return super_type::rbegin();
  }

  const_reverse_iterator rend() const {
    return super_type::rend();
  }

#ifndef BOOST_NO_MEMBER_TEMPLATE_FRIENDS
private:
  template <typename,std::size_t> friend class value_accessor_n;
#else
public: // should be private
#endif

  sub_array (T* base,
            const size_type* extents,
            const index* strides,
            const index* index_base) :
    super_type(base,extents,strides,index_base) {
  }

};

} // namespace multi_array
} // namespace detail
//
// traits classes to get sub_array types
//
template <typename Array, int N>
class subarray_gen {
  typedef typename Array::element element;
public:
  typedef boost::detail::multi_array::sub_array<element,N> type;
};

template <typename Array, int N>
class const_subarray_gen {
  typedef typename Array::element element;
public:
  typedef boost::detail::multi_array::const_sub_array<element,N> type;  
};
} // namespace boost
  
#endif // SUBARRAY_RG071801_HPP
