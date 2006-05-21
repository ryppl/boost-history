/* (C) Copyright Hervé Brönnimann, Polytechnic University, 2002.
 * 
 * Permission to copy, use, modify, sell and distribute this software
 * and its documentation is granted provided this copyright notice
 * appears in all copies. This software and its documentation is
 * provided "as is" without express or implied warranty, and with no
 * claim as to its suitability for any purpose.
 */

#ifndef BOOST_BLOCK_SORT_HPP
#define BOOST_BLOCK_SORT_HPP

#ifndef BOOST_LINEAR_SORT_HPP
#include <boost/static_assert.hpp>
#include <boost/linear_sort/function_dispatch.hpp>
#include <boost/linear_sort/property_map.hpp>
#include <boost/linear_sort/counting_sort.hpp>
#endif

namespace boost {
 
  namespace detail {

  //==============================
  // indirect_block_sort
  //==============================
  // works for variable-size containers, also for std::string
  // RandomAccessIter's value type is a model of Container
  // OutputRandomAccessIter's value type is RandomAccessIter

  template <class RandomAccessIter, class OutputRandomAccessIter>
  void indirect_block_sort(RandomAccessIter first, RandomAccessIter last,
                              OutputRandomAccessIter result)
  {
  }

  //==============================
  // block_sort_copy
  //==============================
  // works for variable-size containers, also for std::string
  // RandomAccessIter's value type is a model of Container
  // OutputRandomAccessIter's value type is RandomAccessIter

  template <class RandomAccessIter, class OutputRandomAccessIter>
  void block_sort_copy(RandomAccessIter first, RandomAccessIter last,
                          OutputRandomAccessIter result)
  {
  }

  //==============================
  // block_sort
  //==============================
  // works for variable-size containers, also for std::string
  // RandomAccessIter's value type is a model of Container

  template <class RandomAccessIter>
  void block_sort(RandomAccessIter first, RandomAccessIter last)
  {
  }
  
  } // namespace detail


} // namespace boost

#endif // BOOST_BLOCK_SORT_HPP
