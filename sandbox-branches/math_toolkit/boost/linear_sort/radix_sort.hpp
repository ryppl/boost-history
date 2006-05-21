/* (C) Copyright Hervé Brönnimann, Polytechnic University, 2002.
 * 
 * Permission to copy, use, modify, sell and distribute this software
 * and its documentation is granted provided this copyright notice
 * appears in all copies. This software and its documentation is
 * provided "as is" without express or implied warranty, and with no
 * claim as to its suitability for any purpose.
 */

#ifndef BOOST_RADIX_SORT_HPP
#define BOOST_RADIX_SORT_HPP

#ifndef BOOST_LINEAR_SORT_HPP
#include <boost/static_assert.hpp>
#include <boost/linear_sort/function_dispatch.hpp>
#include <boost/linear_sort/property_map.hpp>
#include <boost/linear_sort/counting_sort.hpp>
#endif 

// we need a special version for tuples because the components don't
// have a unique type...
#ifdef BOOST_TUPLE_HPP
#include <boost/linear_sort/radix_sort_tuple.hpp>
#endif

// For reasons of performance
#ifndef BOOST_LINEAR_SORT_HPP
#define indirect_recursive_sort  indirect_radix_sort
#define recursive_sort_copy  radix_sort_copy
#define recursive_sort  radix_sort
#else
#define indirect_recursive_sort  indirect_sort
#define recursive_sort_copy  sort_copy
#define recursive_sort  sort
#endif

namespace boost {
 
  namespace detail {

  //==============================
  // radix sort types
  //==============================
 
  template <class T>
  struct radix_sort_type { BOOST_STATIC_CONSTANT(bool, value = false); };

#define BOOST_DECLARE_RADIX_SORT_TYPE(type) \
  template <> struct radix_sort_type< type > \
  { BOOST_STATIC_CONSTANT(bool, value = true); }
#define BOOST_DECLARE_RADIX_SORT_TEMPLATE_1(tmpl, type) \
  template < tmpl > struct radix_sort_type< type > \
  { BOOST_STATIC_CONSTANT(bool, value = true); }
#define BOOST_DECLARE_RADIX_SORT_TEMPLATE_1_2(tmpl1, tmpl2, type) \
  template < tmpl1, tmpl2 > struct radix_sort_type< type > \
  { BOOST_STATIC_CONSTANT(bool, value = true); }
#define BOOST_DECLARE_RADIX_SORT_TEMPLATE_2(tmpl1, tmpl2, type1, type2) \
  template < tmpl1, tmpl2 > struct radix_sort_type< type1,type2 > \
  { BOOST_STATIC_CONSTANT(bool, value = true); }
#define BOOST_DECLARE_RADIX_SORT_TEMPLATE_3(tmpl1, tmpl2, tmpl3, type1, type2, type3) \
  template < tmpl1, tmpl2, tmpl3 > struct radix_sort_type< type1,type2,type3 > \
  { BOOST_STATIC_CONSTANT(bool, value = true); }

  BOOST_DECLARE_RADIX_SORT_TYPE( unsigned char );
  BOOST_DECLARE_RADIX_SORT_TYPE( signed char );
  BOOST_DECLARE_RADIX_SORT_TYPE( unsigned short );
  BOOST_DECLARE_RADIX_SORT_TYPE( signed short );
  BOOST_DECLARE_RADIX_SORT_TYPE( unsigned int );
  BOOST_DECLARE_RADIX_SORT_TYPE( signed int );
  BOOST_DECLARE_RADIX_SORT_TYPE( unsigned long );
  BOOST_DECLARE_RADIX_SORT_TYPE( signed long );
  BOOST_DECLARE_RADIX_SORT_TYPE( float );
  BOOST_DECLARE_RADIX_SORT_TYPE( double );
  BOOST_DECLARE_RADIX_SORT_TYPE( long double );

  BOOST_DECLARE_RADIX_SORT_TEMPLATE_1_2( class T, std::size_t N , T[N] );
  
#ifdef BOOST_ARRAY_HPP
  BOOST_DECLARE_RADIX_SORT_TEMPLATE_2( class T, std::size_t N , ::boost::array<T,N> );
#endif

  // Use BOOST_DECLARE_RADIX_SORT_TYPE and
  // BOOST_DECLARE_RADIX_SORT_TEMPLATE to authorize radix sort
  // with a certain type (for instance your_struct_type)

  //==============================
  // radix_sort_copy
  //==============================

    // radix_sort_copy may reorder the input range, but in any case
    // the output range's value type is that same as that of the input range
 
    template <class RandomAccessIter, class OutputRandomAccessIter,
              class RadixPropertyMap, class MSBRadixPropertyMap>
    void radix_sort_copy(RandomAccessIter first, RandomAccessIter last,
                         OutputRandomAccessIter result, std::size_t N,
                         RadixPropertyMap radix_map,
			 MSBRadixPropertyMap msb_radix_map)
      {
        // radix sort (last digit comes first)
        // by pair, copy back and forth
        signed int i;
        OutputRandomAccessIter result_end = result + (last-first);
	// typedef typename std::iterator_traits<OutputRandomAccessIter>::value_type T;
        for (i = (signed int)N-1; i >= (signed int)1; i -= 2) {
	  // std::cerr << "One pass of radix_sort_copy: \n";
          radix_map.set_radix(i);
	  recursive_sort_copy(first, last, result, radix_map);
	  // std::copy( result, result_end, std::ostream_iterator<T>(std::cerr," "));
	  // std::cerr << std::endl;
          if (i != 1) {
	    // std::cerr << "One pass of radix_sort_copy: \n";
	    radix_map.set_radix(i-1);
	    recursive_sort_copy(result, result_end, first, radix_map);
	    // std::copy( result, result_end, std::ostream_iterator<T>(std::cerr," "));
	    // std::cerr << std::endl;
	  } else {
	    // std::cerr << "Final pass of radix_sort_copy: \n";
            msb_radix_map.set_radix(0);
	    recursive_sort_copy(result, result_end, first, msb_radix_map);
	    // std::copy( result, result_end, std::ostream_iterator<T>(std::cerr," "));
	    // std::cerr << std::endl;
	  }
        }
        if (i == 0) {
          // above loop missed set_radix(0)
	  // std::cerr << "Final pass of radix_sort_copy: \n";
          msb_radix_map.set_radix(0);
	  recursive_sort_copy(first, last, result, msb_radix_map);
	  // std::copy( result, result_end, std::ostream_iterator<T>(std::cerr," "));
	  // std::cerr << std::endl;
        } else {
          // i==-1, and set_radix(0) has been done
          std::copy(first, last, result);
        }
      }

#define BOOST_RADIX_SORT_COPY_DISPATCH(T) \
    template <class RandomAccessIter, class OutputRandomAccessIter, \
	      class ProjectionPropertyMap> \
    inline void \
    radix_sort_copy_dispatch(RandomAccessIter first, RandomAccessIter last, \
      OutputRandomAccessIter result, ProjectionPropertyMap project, \
      linear_sort_function_dispatch<T> const&) \
    { \
      counting_sort_copy(first, last, result, project); \
    }
    BOOST_RADIX_SORT_COPY_DISPATCH( char )
    BOOST_RADIX_SORT_COPY_DISPATCH( signed char )
    BOOST_RADIX_SORT_COPY_DISPATCH( unsigned char )
    // BOOST_RADIX_SORT_COPY_DISPATCH( signed short )
    // BOOST_RADIX_SORT_COPY_DISPATCH( unsigned short )
#undef BOOST_RADIX_SORT_COPY_DISPATCH

#define BOOST_RADIX_SORT_COPY_DISPATCH(T,S,CT,is_float) \
    template <class RandomAccessIter, class OutputRandomAccessIter, \
              class ProjectionPropertyMap> \
    inline void \
    radix_sort_copy_dispatch(RandomAccessIter first, RandomAccessIter last, \
      OutputRandomAccessIter result, ProjectionPropertyMap project, \
      linear_sort_function_dispatch<T> const&) \
    { \
      nth_radix_property_map<T,unsigned CT,ProjectionPropertyMap> \
        radix_map(-1,project); \
      nth_radix_property_map<T,S CT,ProjectionPropertyMap> \
        msb_radix_map(0,project); \
      msb_radix_map.set_float_hack(is_float); \
      detail::radix_sort_copy(first, last, result, sizeof(T)/sizeof(CT), \
	                      radix_map, msb_radix_map); \
    }
    BOOST_RADIX_SORT_COPY_DISPATCH( signed short,  signed,   char, false )
    BOOST_RADIX_SORT_COPY_DISPATCH( unsigned short,unsigned, char, false )
    BOOST_RADIX_SORT_COPY_DISPATCH( signed int,    signed,   char, false )
    BOOST_RADIX_SORT_COPY_DISPATCH( unsigned int,  unsigned, char, false )
    BOOST_RADIX_SORT_COPY_DISPATCH( signed long,   signed,   char, false )
    BOOST_RADIX_SORT_COPY_DISPATCH( unsigned long, unsigned, char, false )
    BOOST_RADIX_SORT_COPY_DISPATCH( float,         signed,   char, true )
    BOOST_RADIX_SORT_COPY_DISPATCH( double,        signed,   char, true )
    BOOST_RADIX_SORT_COPY_DISPATCH( long double,   signed,   char, true )
#undef BOOST_RADIX_SORT_COPY_DISPATCH

    template <class RandomAccessIter, class OutputRandomAccessIter,
              class T, std::size_t N, class ProjectionPropertyMap>
    inline void
    radix_sort_copy_dispatch(RandomAccessIter first, RandomAccessIter last,
      OutputRandomAccessIter result, ProjectionPropertyMap project,
      linear_sort_function_dispatch< T[N] > const&)
    {
      typedef T Array[N];
      nth_element_property_map<Array,T,ProjectionPropertyMap> radix_map(-1,project);
      BOOST_STATIC_ASSERT( radix_sort_type<Array>::value );
      detail::radix_sort_copy(first, last, result, N, radix_map, radix_map);
    }

#ifdef BOOST_ARRAY_HPP
    template <class RandomAccessIter, class OutputRandomAccessIter,
              class T, std::size_t N, class ProjectionPropertyMap>
    inline void
    radix_sort_copy_dispatch(RandomAccessIter first, RandomAccessIter last,
      OutputRandomAccessIter result, ProjectionPropertyMap project,
      linear_sort_function_dispatch< boost::array<T,N> > const&)
    {
      typedef boost::array<T,N> Array;
      nth_element_property_map<Array,T,ProjectionPropertyMap> radix_map(-1,project);
      BOOST_STATIC_ASSERT( radix_sort_type<Array>::value );
      detail::radix_sort_copy(first, last, result, N, radix_map, radix_map);
    }
#endif


  //==============================
  // radix_sort
  //==============================

    template <class RandomAccessIter, class Value,
              class RadixPropertyMap, class MSBRadixPropertyMap>
    void radix_sort(RandomAccessIter first, RandomAccessIter last, std::size_t N,
                    RadixPropertyMap radix_map, MSBRadixPropertyMap msb_radix_map,
                    linear_sort_function_dispatch<Value> const&)
      {
        using namespace std;
        ptrdiff_t length = last-first;
        pair<Value*, ptrdiff_t> p = get_temporary_buffer<Value>(length);
  
        if (p.second >= length) {
          // use radix sort copy 
          Value* p_begin = p.first;
	  Value* p_end = p_begin + length;
	  if (N % 2 == 0) {
	    assert( N > 0);
            radix_map.set_radix(N-1);
	    recursive_sort_copy(first, last, p_begin, radix_map);
	    detail::radix_sort_copy(p_begin, p_end, first, N-1,
	                            radix_map, msb_radix_map);
	  } else {
	    // compensate odd length by extra copy
	    copy(first, last, p_begin);
	    detail::radix_sort_copy(p_begin, p_end, first, N,
	                            radix_map, msb_radix_map);
	  }
	} else {
          // free temp buffer first, this may help for counting_sort,
          // although more likely than not, counting_sort will not have
	  // a temporary buffer and will be forced to call stable_sort
          if (p.first)
            return_temporary_buffer(p.first);
          // radix sort (last digit comes first)
          for (signed int i=N-1; i>0; --i) {
            radix_map.set_radix(i);
	    recursive_sort(first, last, radix_map);
          }
          msb_radix_map.set_radix(0);
          recursive_sort(first, last, msb_radix_map);
	}
      }

#define BOOST_RADIX_SORT_DISPATCH(T) \
    template <class RandomAccessIter, class Value, class ProjectionPropertyMap> \
    inline void \
    radix_sort_dispatch(RandomAccessIter first, RandomAccessIter last, \
      ProjectionPropertyMap project, linear_sort_function_dispatch<T> const&, \
      linear_sort_function_dispatch<Value> const&) \
    { \
      counting_sort(first, last, project); \
    }
    BOOST_RADIX_SORT_DISPATCH( char )
    BOOST_RADIX_SORT_DISPATCH( signed char )
    BOOST_RADIX_SORT_DISPATCH( unsigned char )
    // BOOST_RADIX_SORT_DISPATCH( signed short )
    // BOOST_RADIX_SORT_DISPATCH( unsigned short )
#undef BOOST_RADIX_SORT_DISPATCH

#define BOOST_RADIX_SORT_DISPATCH(T,S,CT,is_float) \
    template <class RandomAccessIter, class Value, class ProjectionPropertyMap> \
    inline void \
    radix_sort_dispatch(RandomAccessIter first, RandomAccessIter last, \
      ProjectionPropertyMap project, linear_sort_function_dispatch<T> const&, \
      linear_sort_function_dispatch<Value> const&t) \
    { \
      nth_radix_property_map<T,unsigned CT,ProjectionPropertyMap> \
        radix_map(-1,project); \
      nth_radix_property_map<T,S CT,ProjectionPropertyMap> \
        msb_radix_map(0,project); \
      msb_radix_map.set_float_hack(is_float); \
      detail::radix_sort(first, last, sizeof(T)/sizeof(CT), \
	                 radix_map, msb_radix_map, t); \
    }
    BOOST_RADIX_SORT_DISPATCH( signed short,   signed,   char,   false )
    BOOST_RADIX_SORT_DISPATCH( unsigned short, unsigned, char,   false )
    BOOST_RADIX_SORT_DISPATCH( signed int,     signed,   char,   false )
    BOOST_RADIX_SORT_DISPATCH( unsigned int,   unsigned, char,   false )
    BOOST_RADIX_SORT_DISPATCH( signed long,    signed,   char,   false )
    BOOST_RADIX_SORT_DISPATCH( unsigned long,  unsigned, char,   false )
    BOOST_RADIX_SORT_DISPATCH( float,          signed,   char,   true )
    BOOST_RADIX_SORT_DISPATCH( double,         signed,   char,   true )
    BOOST_RADIX_SORT_DISPATCH( long double,    signed,   char,   true )
#undef BOOST_RADIX_SORT_DISPATCH

    template <class RandomAccessIter, class Value, class T, std::size_t N,
              class PropertyMap>
    inline void
    radix_sort_dispatch(RandomAccessIter first, RandomAccessIter last,
      PropertyMap project, linear_sort_function_dispatch< T[N] > const&,
      linear_sort_function_dispatch<Value> const&t)
    {
      typedef T Array[N];
      nth_element_property_map<Array,T,PropertyMap> radix_map(-1,project);
      BOOST_STATIC_ASSERT( radix_sort_type<Array>::value );
      detail::radix_sort(first, last, N, radix_map, radix_map, t);
    }


#ifdef BOOST_ARRAY_HPP
    template <class RandomAccessIter, class Value, class T, std::size_t N,
              class PropertyMap>
    inline void
    radix_sort_dispatch(RandomAccessIter first, RandomAccessIter last,
      PropertyMap project, linear_sort_function_dispatch< array<T,N> > const&,
      linear_sort_function_dispatch<Value> const&t)
    {
      typedef array<T,N> Array;
      nth_element_property_map<Array,T,PropertyMap> radix_map(-1,project);
      BOOST_STATIC_ASSERT( radix_sort_type<Array>::value );
      detail::radix_sort(first, last, N, radix_map, radix_map, t);
    }
#endif

  //==============================
  // indirect_radix_sort
  //==============================
  // (should properly be named indirect_radix_sort_copy)

    // indirect radix sort means that the input range is undisturbed and
    // that the output range's value type is an iterator in the input range
 
    template <class RandomAccessIter, class OutputRandomAccessIter, class Value,
              class RadixPropertyMap, class MSBRadixPropertyMap >
    void indirect_radix_sort(RandomAccessIter first, RandomAccessIter last,
                             OutputRandomAccessIter result, std::size_t N,
                             RadixPropertyMap radix_map,
			     MSBRadixPropertyMap msb_radix_map,
			     linear_sort_function_dispatch<Value> const&)
      {
	using namespace std;
        // make proper IndirectRadixPropertyMap
        dereference_property_map<RandomAccessIter,RadixPropertyMap>
                indirect_radix_map(radix_map);
        dereference_property_map<RandomAccessIter,MSBRadixPropertyMap>
                indirect_msb_radix_map(msb_radix_map);

        // populate the output with iterators
        ptrdiff_t length = (last - first);
        OutputRandomAccessIter begin = result, end = result + length;
        for (OutputRandomAccessIter it = begin; it != end; ++it, ++first)
          *it = first;

        // radix sort directly within result
	radix_sort(begin, end, N, indirect_radix_map, indirect_msb_radix_map,
	           linear_sort_function_dispatch<RandomAccessIter>());
      }

#define BOOST_INDIRECT_RADIX_SORT_DISPATCH(T) \
    template <class RandomAccessIter, class OutputRandomAccessIter, class Value, \
              class ProjectionPropertyMap> \
    inline void \
    indirect_radix_sort_dispatch(RandomAccessIter first, RandomAccessIter last, \
      OutputRandomAccessIter result, ProjectionPropertyMap project, \
      linear_sort_function_dispatch<T> const&, \
      linear_sort_function_dispatch<Value> const&t) \
    { \
      indirect_counting_sort(first, last, result, project, t); \
    }
    BOOST_INDIRECT_RADIX_SORT_DISPATCH( char )
    BOOST_INDIRECT_RADIX_SORT_DISPATCH( signed char )
    BOOST_INDIRECT_RADIX_SORT_DISPATCH( unsigned char )
#undef BOOST_INDIRECT_RADIX_SORT_DISPATCH

#define BOOST_INDIRECT_RADIX_SORT_DISPATCH(T,CT,is_float) \
    template <class RandomAccessIter, class OutputRandomAccessIter, class Value, \
              class ProjectionPropertyMap> \
    inline void \
    indirect_radix_sort_dispatch(RandomAccessIter first, RandomAccessIter last, \
      OutputRandomAccessIter result, ProjectionPropertyMap project, \
      linear_sort_function_dispatch<T> const&, \
      linear_sort_function_dispatch<Value> const&t) \
    { \
      nth_radix_property_map<T,unsigned char,ProjectionPropertyMap> \
        radix_map(-1,project); \
      nth_radix_property_map<T,CT,ProjectionPropertyMap> msb_radix_map(0,project); \
      msb_radix_map.set_float_hack(is_float); \
      detail::indirect_radix_sort(first, last, result, sizeof(T), \
	                          radix_map, msb_radix_map, t); \
    }
    BOOST_INDIRECT_RADIX_SORT_DISPATCH( signed short,   signed char,   false )
    BOOST_INDIRECT_RADIX_SORT_DISPATCH( unsigned short, unsigned char, false )
    BOOST_INDIRECT_RADIX_SORT_DISPATCH( signed int,     signed char,   false )
    BOOST_INDIRECT_RADIX_SORT_DISPATCH( unsigned int,   unsigned char, false )
    BOOST_INDIRECT_RADIX_SORT_DISPATCH( signed long,    signed char,   false )
    BOOST_INDIRECT_RADIX_SORT_DISPATCH( unsigned long,  unsigned char, false )
    BOOST_INDIRECT_RADIX_SORT_DISPATCH( float,          signed char,   true )
    BOOST_INDIRECT_RADIX_SORT_DISPATCH( double,         signed char,   true )
    BOOST_INDIRECT_RADIX_SORT_DISPATCH( long double,    signed char,   true )
#undef BOOST_INDIRECT_RADIX_SORT_DISPATCH

    template <class RandomAccessIter, class OutputRandomAccessIter, class Value,
              class T, std::size_t N, class ProjectionPropertyMap>
    inline void
    indirect_radix_sort_dispatch(RandomAccessIter first, RandomAccessIter last,
      OutputRandomAccessIter result, ProjectionPropertyMap project,
      linear_sort_function_dispatch< T[N] > const&,
      linear_sort_function_dispatch<Value> const&t) \
    {
      typedef T Array[N];
      nth_element_property_map<Array,T,ProjectionPropertyMap> radix_map(-1,project);
      BOOST_STATIC_ASSERT( radix_sort_type<Array>::value );
      detail::indirect_radix_sort(first, last, result, N, radix_map, radix_map, t);
    }

#ifdef BOOST_ARRAY_HPP
    template <class RandomAccessIter, class OutputRandomAccessIter, class Value,
              class T, std::size_t N, class ProjectionPropertyMap>
    inline void
    indirect_radix_sort_dispatch(RandomAccessIter first, RandomAccessIter last,
      OutputRandomAccessIter result, ProjectionPropertyMap project,
      linear_sort_function_dispatch< array<T,N> > const&,
      linear_sort_function_dispatch< Value > const&t)
    {
      typedef array<T,N> Array;
      nth_element_property_map<Array,T,ProjectionPropertyMap> radix_map(-1,project);
      BOOST_STATIC_ASSERT( radix_sort_type<Array>::value );
      detail::indirect_radix_sort(first, last, result, N, radix_map, radix_map, t);
    }
#endif

  } // namespace detail

#undef indirect_recursive_sort
#undef recursive_sort_copy
#undef recursive_sort

  //=============================================
  // dispatching mechanism: principal functions
  //=============================================
  // goal: to find the appropriate RadixPropertyMap for supported types
  // without using partial specialization (only function overloading)
  // important observation: we can always dereference first (or else,
  // range is empty!) so avoid using the iterator_traits altogether

  // variants with ProjectionPropertyMap

  template <class RandomAccessIter, class OutputRandomAccessIter,
            class ProjectionPropertyMap>
  void indirect_radix_sort(RandomAccessIter first, RandomAccessIter last,
                           OutputRandomAccessIter result,
                           ProjectionPropertyMap project)
  {
    if (first == last) return;
    detail::indirect_radix_sort_dispatch(first, last, result, project,
      detail::make_linear_sort_function_dispatch(project[*first]),
      detail::make_linear_sort_function_dispatch(*first));
  }

  template <class RandomAccessIter, class OutputRandomAccessIter,
            class ProjectionPropertyMap>
  void radix_sort_copy(RandomAccessIter first, RandomAccessIter last,
                       OutputRandomAccessIter result,
                       ProjectionPropertyMap project)
  {
    if (first == last) return;
    detail::radix_sort_copy_dispatch(first, last, result, project,
      detail::make_linear_sort_function_dispatch(project[*first]));
  }

  template <class RandomAccessIter, class ProjectionPropertyMap>
  void radix_sort(RandomAccessIter first, RandomAccessIter last,
                  ProjectionPropertyMap project)
  {
    if (first == last) return;
    detail::radix_sort_dispatch(first, last, project,
      detail::make_linear_sort_function_dispatch(project[*first]),
      detail::make_linear_sort_function_dispatch(*first));
  }

  // variants without ProjectionPropertyMap

  template <class RandomAccessIter, class OutputRandomAccessIter>
  void indirect_radix_sort(RandomAccessIter first, RandomAccessIter last,
                           OutputRandomAccessIter result)
  {
    if (first == last) return;
    detail::indirect_radix_sort_dispatch(first, last, result,
      make_linear_sort_identity_property_map(*first),
      detail::make_linear_sort_function_dispatch(*first),
      detail::make_linear_sort_function_dispatch(*first));
  }

  template <class RandomAccessIter, class OutputRandomAccessIter>
  void radix_sort_copy(RandomAccessIter first, RandomAccessIter last,
                       OutputRandomAccessIter result)
  {
    if (first == last) return;
    detail::radix_sort_copy_dispatch(first, last, result,
      make_linear_sort_identity_property_map(*first),
      detail::make_linear_sort_function_dispatch(*first));
  }

  template <class RandomAccessIter>
  void radix_sort(RandomAccessIter first, RandomAccessIter last)
  {
    if (first == last) return;
    detail::radix_sort_dispatch(first, last,
      make_linear_sort_identity_property_map(*first),
      detail::make_linear_sort_function_dispatch(*first),
      detail::make_linear_sort_function_dispatch(*first));
  }

} // namespace boost

#endif // BOOST_RADIX_SORT_HPP
