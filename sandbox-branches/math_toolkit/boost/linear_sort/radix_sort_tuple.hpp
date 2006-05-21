/* (C) Copyright Hervé Brönnimann, Polytechnic University, 2002.
 * 
 * Permission to copy, use, modify, sell and distribute this software
 * and its documentation is granted provided this copyright notice
 * appears in all copies. This software and its documentation is
 * provided "as is" without express or implied warranty, and with no
 * claim as to its suitability for any purpose.
 */

#ifndef BOOST_RADIX_SORT_TUPLE_HPP
#define BOOST_RADIX_SORT_TUPLE_HPP

#ifndef BOOST_RADIX_SORT_HPP
#include <boost/static_assert.hpp>
#include <boost/linear_sort/function_dispatch.hpp>
#include <boost/linear_sort/property_map.hpp>
#include <boost/linear_sort/counting_sort.hpp>
#endif 

// For reasons of performance
#ifdef BOOST_LINEAR_SORT_HPP
#define indirect_recursive_sort  indirect_sort
#define recursive_sort_copy  sort_copy
#define recursive_sort  sort
#else
#define indirect_recursive_sort  indirect_radix_sort
#define recursive_sort_copy  radix_sort_copy
#define recursive_sort  radix_sort
#endif

namespace boost {
 
  namespace detail {

  //==============================
  // indirect_radix_sort
  //==============================
  // (should properly be named indirect_radix_sort_copy)

    // indirect radix sort means that the input range is undisturbed and
    // that the output range's value type is an iterator in the input range
 
    template <class RandomAccessIter, class OutputRandomAccessIter,
              class RadixPropertyMap >
    void indirect_radix_sort(RandomAccessIter first, RandomAccessIter last,
                             OutputRandomAccessIter result,
                             RadixPropertyMap radix_map, std::size_t N)
      {
	using namespace std;

        // make proper IndirectRadixPropertyMap
        dereference_property_map<RandomAccessIter,RadixPropertyMap>
                indirect_radix_map(radix_map);

        // populate the output with iterators
        ptrdiff_t length = (last - first);
        OutputRandomAccessIter begin = result, end = result + length;
        for (OutputRandomAccessIter it = begin; it != end; ++it, ++first)
          *it = first;

        // get a temporary buffer to avoid copying back and forth
        pair<RandomAccessIter*, ptrdiff_t>
                q = get_temporary_buffer<RandomAccessIter>(length);
        
        if (q.second == length) {
          RandomAccessIter *dest = q.first, *dest_end = q.first+q.second;
          int i = N-1;
          // indirect radix sort (last digit comes first)
          while (i >= (int)1) {
            indirect_radix_map.property_map().set_radix(i);
	    recursive_sort_copy(begin, end, dest, indirect_radix_map);
            indirect_radix_map.property_map().set_radix(i-1);
	    recursive_sort_copy(dest, dest_end, begin, indirect_radix_map);
            i -= 2;
          }
          if (i == 0) {
            indirect_radix_map.property_map().set_radix(i);
	    recursive_sort_copy(begin, end, dest, indirect_radix_map);
            copy(dest, dest_end, begin);
          }
          // free buffer
          return_temporary_buffer(q.first);
        } else {
          // can't use buffer of smaller length, unfortunately...
          if (q.first)
            return_temporary_buffer(q.first);
          // indirect radix sort (last digit comes first) directly into result
          // [will probably use stable_sort instead of counting_sort]
          for (int i=N-1; i>=0; --i) {
            indirect_radix_map.property_map().set_radix(i);
            recursive_sort(begin, end, indirect_radix_map);
          }
        }
      }

    template <class RandomAccessIter, class OutputRandomAccessIter,
              class T, std::size_t N, class ProjectionPropertyMap>
    inline void
    indirect_radix_sort_dispatch(RandomAccessIter first, RandomAccessIter last,
      OutputRandomAccessIter result, ProjectionPropertyMap project,
      linear_sort_function_dispatch< array<T,N> > const&)
    {
      typedef array<T,N> Array;
      nth_element_property_map<Array,T,ProjectionPropertyMap> radix_map(-1,project);
      BOOST_STATIC_ASSERT( radix_sort_type<Array>::value );
      detail::indirect_radix_sort(first, last, result, radix_map, N);
    }

  //==============================
  // radix_sort_copy
  //==============================

    // radix_sort_copy may reorder the input range, but in any case
    // the output range's value type is that same as that of the input range
 
    template <class RandomAccessIter, class OutputRandomAccessIter,
              class ProjectionPropertyMap>
    class radix_sort_tuple_copy {
      radix_sort_tuple_copy(RandomAccessIter first, RandomAccessIter last,
                            OutputRandomAccessIter result,
			    ProjectionPropertyMap project)

      {
        // radix sort (last digit comes first)
        // by pair, copy back and forth
        signed int i;
        OutputRandomAccessIter result_end = result + (last-first);
	if (is_odd) {
	  recursive_sort_copy(first, last, result, radix_map);
	} else {
	  recursive_sort_copy(result, result_end, first, radix_map);
        }
        if (i == 0) {
          // above loop missed set_radix(0)
          radix_map.set_radix(0);
	  recursive_sort_copy(first, last, result, radix_map);
        } else {
          std::copy(first, last, result);
        }
      }

    template <class RandomAccessIter, class OutputRandomAccessIter,
              class Head, class Tail, class ProjectionPropertyMap>
    inline void
    radix_sort_copy_dispatch(RandomAccessIter first, RandomAccessIter last,
      OutputRandomAccessIter result, ProjectionPropertyMap project,
      linear_sort_function_dispatch< boost::tuples::cons<Head,Tail> > const&)
    {
      typedef boost::tuples::cons<Head,Tail> Array;
      detail::radix_sort_tuple_copy<> r(first, last, result, radix_map);
    }


  //==============================
  // radix_sort
  //==============================

    template <class RandomAccessIter, class RadixPropertyMap, class Array>
    void radix_sort(RandomAccessIter first, RandomAccessIter last,
                    RadixPropertyMap radix_map,
                    linear_sort_function_dispatch<Array> const&)
      {
        using namespace std;
        ptrdiff_t length = last-first;
        pair<Array*, ptrdiff_t> p = get_temporary_buffer<Array>(length);
  
        if (p.second >= length) {
          // use radix sort copy 
          Array* p_begin = p.first;
	  Array* p_end = p_begin + length;
	  if (N % 2 == 0) {
	    assert( N > 0);
            radix_map.set_radix(N-1);
	    recursive_sort_copy(first, last, p_begin, radix_map);
	    detail::radix_sort_copy(p_begin, p_end, first, radix_map, N-1);
	  } else {
	    // compensate odd length by extra copy
	    copy(first, last, p_begin);
	    detail::radix_sort_copy(p_begin, p_end, first, radix_map, N);
	  }
	} else {
          // free temp buffer first, this may help for counting_sort,
          // although more likely than not, counting_sort will not have
	  // a temporary buffer and will be forced to call stable_sort
          if (p.first)
            return_temporary_buffer(p.first);
          // radix sort (last digit comes first)
          for (signed int i=N-1; i>=0; --i) {
            radix_map.set_radix(i);
	    recursive_sort(first, last, radix_map);
          }
	}
      }

    template <class RandomAccessIter, class Head, class Tail, class PropertyMap>
    inline void
    radix_sort_dispatch(RandomAccessIter first, RandomAccessIter last,
      PropertyMap project,
      linear_sort_function_dispatch< boost::tuples::cons<Head,Tail > const&)
    {
      typedef array<T,N> Array;
      BOOST_STATIC_ASSERT( radix_sort_type<Array>::value );
      detail::radix_sort(first, last, project,
                         linear_sort_function_dispatch<Array>());
    }

  } // namespace detail

} // namespace boost

#ifndef BOOST_RADIX_SORT_HPP
#include <boost/linear_sort/radix_sort.hpp>
#endif

#endif // BOOST_RADIX_SORT_TUPLE_HPP
