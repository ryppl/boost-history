/* (C) Copyright Hervé Brönnimann, Polytechnic University, 2002.
 * 
 * Permission to copy, use, modify, sell and distribute this software
 * and its documentation is granted provided this copyright notice
 * appears in all copies. This software and its documentation is
 * provided "as is" without express or implied warranty, and with no
 * claim as to its suitability for any purpose.
 */

#ifndef BOOST_COUNTING_SORT_HPP
#define BOOST_COUNTING_SORT_HPP

#ifndef BOOST_LINEAR_SORT_HPP
#include <memory>
#include <algorithm>
#include <functional>
#include <iterator>
#include <boost/static_assert.hpp>
#include <boost/integer_traits.hpp>
#include <boost/linear_sort/function_dispatch.hpp>
#include <boost/linear_sort/property_map.hpp>
#endif

namespace boost {
 
  //==============================
  // counting_sort_traits 
  //==============================
  // should be specialized by user for user-defined types T
  // which have a value small enough to us counting sort
  // (e.g. enums), in conjunction with
  // BOOST_DECLARE_COUNTING_SORT_TYPE(T);

  template <class T>
  struct counting_sort_traits : public boost::integer_traits<T>
  {
    BOOST_STATIC_CONSTANT(std::size_t,
      range_size = (const_min < 0 && const_max >= 0)
        ? std::size_t(const_max) + std::size_t(-const_min) + 1
        : std::size_t(const_max - const_min) + 1);
  };

  //==============================
  // counting sort types
  //==============================

  // check assumptions attached to counting sort algorithm

  template <class T>
  struct counting_sort_type { BOOST_STATIC_CONSTANT(bool, value = false); };

  #define BOOST_DECLARE_COUNTING_SORT_TYPE(type) \
    template<> struct counting_sort_type< type > \
    { BOOST_STATIC_CONSTANT(bool, value = true); }
  #define BOOST_DECLARE_COUNTING_SORT_TEMPLATE_1(tmpl, type) \
    template< tmpl > struct counting_sort_type< type > \
    { BOOST_STATIC_CONSTANT(bool, value = true); }
  #define BOOST_DECLARE_COUNTING_SORT_TEMPLATE_2(tmpl1, tmpl2, type) \
    template< tmpl1, tmpl2 > struct counting_sort_type< type > \
    { BOOST_STATIC_CONSTANT(bool, value = true); }
  #define BOOST_DECLARE_COUNTING_SORT_TEMPLATE_3(tmpl1, tmpl2, tmpl3, type) \
    template< tmpl1, tmpl2, tmpl3 > struct counting_sort_type< type > \
    { BOOST_STATIC_CONSTANT(bool, value = true); }

  BOOST_DECLARE_COUNTING_SORT_TYPE(bool);
  BOOST_DECLARE_COUNTING_SORT_TYPE(char);
  BOOST_DECLARE_COUNTING_SORT_TYPE(unsigned char);
  BOOST_DECLARE_COUNTING_SORT_TYPE(signed char);
  // BOOST_DECLARE_COUNTING_SORT_TYPE(short);
  BOOST_DECLARE_COUNTING_SORT_TYPE(unsigned short);
  BOOST_DECLARE_COUNTING_SORT_TYPE(signed short);

  // Use BOOST_DECLARE_COUNTING_SORT_TYPE and
  // BOOST_DECLARE_COUNTING_SORT_TEMPLATE to authorize couting sort
  // with a certain type (for instance unsigned short)

  //==============================
  // counting_sort_copy
  //==============================

  #define BOOST_COUNTING_SORT_CREATE_COUNTER_AND_OFFSET( T ) \
      std::size_t counter_table[counting_sort_traits<T>::range_size]; \
      std::size_t *counter = (std::size_t*)counter_table \
                           - counting_sort_traits<T>::const_min; \
      std::size_t offset_table[counting_sort_traits<T>::range_size]; \
      std::size_t *offset = (std::size_t*)offset_table \
                           - counting_sort_traits<T>::const_min
 
  namespace detail {

    template <class ProjectionPropertyMap>
    bool needs_signed_float_radix_hack(ProjectionPropertyMap const&)
    {
      return false;
    }

    template <class T, class Radix, class PropertyMap>
    bool
    needs_signed_float_radix_hack
      (nth_radix_property_map<T,Radix,PropertyMap> project)
    {
      return project.get_float_hack();
    }

    template <class RandomAccessIter, class OutputRandomAccessIter, class Value,
              class ProjectionPropertyMap, class CounterPropertyMap,
	      class OutputPropertyMap>
    void counting_sort_copy(RandomAccessIter first, RandomAccessIter last,
                            OutputRandomAccessIter result,
                            ProjectionPropertyMap project,
                            CounterPropertyMap counter,
                            CounterPropertyMap offset,
			    OutputPropertyMap output_map,
                            linear_sort_function_dispatch<Value> const&)
    {
      typedef typename property_traits<ProjectionPropertyMap>::value_type T;
      typedef counting_sort_traits<T> traits;
      BOOST_STATIC_ASSERT( counting_sort_type<T>::value );
    
      for (T i = traits::const_min; i != traits::const_max; ++i)
        counter[i] = 0;
      counter[traits::const_max] = 0;
      for (RandomAccessIter it = first; it != last; ++it)
        ++counter[project[*it]];
      // this is crucial for radix sort of IEEE float types
      if (needs_signed_float_radix_hack(project)) {
        // accumulate the counters first for negative values, in
        // reverse, then for positive values
        // thanks to Pierre Terdiman for articulating the concept
        // http://codercorner.com/RadixSortRevisited.htm
	offset[-1] = 0;
        for (T i = -1; i != traits::const_min; --i)
          offset[i-1] = offset[i]+counter[i];
        offset[0] = offset[traits::const_min] + counter[traits::const_min];
        for (T i = 0; i != traits::const_max; ++i)
          offset[i+1] = offset[i]+counter[i];
        // now we have figured where everybody should go, output it
	// don't forget to output negative entries in REVERSE order
	std::less<T> less; // just to avoid warning below!
        for (RandomAccessIter it = first; it != last; ++it)
          if (less(project[*it],T(0)))
	    result[offset[project[*it]] + --counter[project[*it]]] = output_map[it];
	  else
            result[offset[project[*it]]++] = output_map[it];
      } else {
        // accumulate counters, they become offsets
	offset[traits::const_min] = 0;
        for (T i = traits::const_min; i != traits::const_max; ++i)
          offset[i+1] = offset[i]+counter[i];
        // now we have figured where everybody should go, output it
        for (RandomAccessIter it = first; it != last; ++it)
          result[offset[project[*it]]++] = output_map[it];
      }
    }
  
    template <class RandomAccessIter, class OutputRandomAccessIter, class Value,
              class ProjectionPropertyMap>
    inline void
    counting_sort_copy_dispatch(RandomAccessIter first, RandomAccessIter last,
                                OutputRandomAccessIter result,
                                ProjectionPropertyMap project,
                                linear_sort_function_dispatch<Value> const&t)
    {
      typedef typename property_traits<ProjectionPropertyMap>::value_type T;
      linear_sort_identity_property_map<Value> identity;
      BOOST_COUNTING_SORT_CREATE_COUNTER_AND_OFFSET( T );
      detail::counting_sort_copy(first, last, result, project, counter, offset,
	                         make_dereference_property_map(first, identity), t);
    }

    template <class RandomAccessIter, class OutputRandomAccessIter, class Value>
    inline void
    counting_sort_copy_dispatch(RandomAccessIter first, RandomAccessIter last,
                                OutputRandomAccessIter result,
                                linear_sort_function_dispatch<Value> const&t)
    {
      linear_sort_identity_property_map<Value> identity;
      BOOST_COUNTING_SORT_CREATE_COUNTER_AND_OFFSET( Value );
      detail::counting_sort_copy(first, last, result, identity, counter, offset,
	                         make_dereference_property_map(first, identity), t);
    }

  } // namespace detail

  //==============================
  // counting_sort
  //==============================

  // inplace version: uses a temporary buffer for counting sort, or else
  // simply sorts the elements using stable comparison sorting

  namespace detail {

    template <class RandomAccessIter, class Value, class ProjectionPropertyMap, 
              class CounterPropertyMap, class OutputPropertyMap>
    void counting_sort(RandomAccessIter first, RandomAccessIter last,
                       ProjectionPropertyMap project,
                       CounterPropertyMap counter,
                       CounterPropertyMap offset,
                       OutputPropertyMap output_map,
                       linear_sort_function_dispatch<Value> const&t)
    {
      using namespace std;
      ptrdiff_t length = last-first;
      pair<Value*, ptrdiff_t> p = get_temporary_buffer<Value>(length);
      
      if (p.second >= length) {
        detail::counting_sort_copy(first, last, p.first,
	                           project, counter, offset, output_map, t);
        copy(p.first, p.first+length, first);
        return_temporary_buffer(p.first);
      } else {
        if (p.first) return_temporary_buffer(p.first);
        // stable_sort has the usual problem of the SGI STL: they don't
        // explicity provide the template parameters in their internal
        // function calls, so they lose refs and builtin arrays become
        // pointers... Darn!
        // The following will therefore not compile if value type is a
        // builtin array...
        // What we want is that if first points to an array, then
        // first should point to a boost::ref to that array...
        // linear_sort_property_map_cmp<ProjectionPropertyMap> cmp(project);
        // stable_sort(first, last, cmp);
        cerr << "Not able to secure a temp buffer of length " << length << endl;
        assert( false /* No memory for counting sort */ );
      }
    }

    template <class RandomAccessIter, class Value, class ProjectionPropertyMap>
    inline void
    counting_sort_dispatch(RandomAccessIter first, RandomAccessIter last,
                           ProjectionPropertyMap project,
                           linear_sort_function_dispatch<Value> const& t)
    {
      typedef typename property_traits<ProjectionPropertyMap>::value_type T;
      linear_sort_identity_property_map<Value> identity;
      BOOST_COUNTING_SORT_CREATE_COUNTER_AND_OFFSET( T );
      detail::counting_sort(first, last, project, counter, offset,
	                    make_dereference_property_map(first, identity), t);
    }

    template <class RandomAccessIter, class Value>
    inline void
    counting_sort_dispatch(RandomAccessIter first, RandomAccessIter last,
                           linear_sort_function_dispatch<Value> const& t)
    {
      linear_sort_identity_property_map<Value> identity;
      BOOST_COUNTING_SORT_CREATE_COUNTER_AND_OFFSET( Value );
      detail::counting_sort(first, last, identity, counter, offset,
	                    make_dereference_property_map(first, identity), t);
    }

  } // namespace detail

  //==============================
  // indirect_counting_sort
  //==============================

  namespace detail {

    template <class RandomAccessIter, class OutputRandomAccessIter, class Value,
              class ProjectionPropertyMap, class CounterPropertyMap >
    void indirect_counting_sort(RandomAccessIter first, RandomAccessIter last,
                                OutputRandomAccessIter result,
                                ProjectionPropertyMap project,
                                CounterPropertyMap counter,
                                CounterPropertyMap offset)
    {
      // make proper IndirectRadixPropertyMap
      dereference_property_map<RandomAccessIter,ProjectionPropertyMap>
              indirect_project_map(project);

      // radix sort directly into result
      radix_sort_copy(first, last, result, indirect_project_map, counter, offset,
		      linear_sort_identity_property_map<RandomAccessIter>(),
                      linear_sort_function_dispatch<Value>());
    }
  
    template <class RandomAccessIter, class OutputRandomAccessIter, class Value,
              class ProjectionPropertyMap>
    inline void
    indirect_counting_sort_dispatch(RandomAccessIter first, RandomAccessIter last,
                                OutputRandomAccessIter result,
                                ProjectionPropertyMap project,
                                linear_sort_function_dispatch<Value> const&)
    {
      typedef typename property_traits<ProjectionPropertyMap>::value_type T;
      BOOST_COUNTING_SORT_CREATE_COUNTER_AND_OFFSET( T );
      detail::indirect_counting_sort(first, last, result, project, counter, offset);
    }

    template <class RandomAccessIter, class OutputRandomAccessIter, class Value>
    inline void
    indirect_counting_sort_dispatch(RandomAccessIter first, RandomAccessIter last,
                                OutputRandomAccessIter result,
                                linear_sort_function_dispatch<Value> const&)
    {
      linear_sort_identity_property_map<Value> identity;
      BOOST_COUNTING_SORT_CREATE_COUNTER_AND_OFFSET( Value );
      detail::indirect_counting_sort(first, last, result, identity, counter, offset);
    }

  } // namespace detail


  //=============================================
  // dispatching mechanism: principal functions
  //=============================================

  // variants with explicit ProjectionPropertyMap (no dispatch needed)

  template <class RandomAccessIter, class OutputRandomAccessIter,
            class ProjectionPropertyMap>
  void indirect_counting_sort(RandomAccessIter first, RandomAccessIter last,
                          OutputRandomAccessIter result,
                          ProjectionPropertyMap project)
  {
    if (first == last) return;
    detail::indirect_counting_sort_dispatch(first, last, result, project,
      detail::make_linear_sort_function_dispatch(*first));
  }

  template <class RandomAccessIter, class OutputRandomAccessIter,
            class ProjectionPropertyMap>
  void counting_sort_copy(RandomAccessIter first, RandomAccessIter last,
                          OutputRandomAccessIter result,
                          ProjectionPropertyMap project)
  {
    if (first == last) return;
    detail::counting_sort_copy_dispatch(first, last, result, project,
      detail::make_linear_sort_function_dispatch(*first));
  }

  template <class RandomAccessIter, class ProjectionPropertyMap>
  void counting_sort(RandomAccessIter first, RandomAccessIter last,
                     ProjectionPropertyMap project)
  {
    if (first == last) return;
    detail::counting_sort_dispatch(first, last, project,
      detail::make_linear_sort_function_dispatch(*first));
  }

  // variants with implicit ProjectionPropertyMap

  template <class RandomAccessIter, class OutputRandomAccessIter>
  void indirect_counting_sort(RandomAccessIter first, RandomAccessIter last,
                              OutputRandomAccessIter result)
  {
    if (first == last) return;
    detail::indirect_counting_sort_dispatch(first, last, result,
      detail::make_linear_sort_function_dispatch(*first));
  }

  template <class RandomAccessIter, class OutputRandomAccessIter>
  void counting_sort_copy(RandomAccessIter first, RandomAccessIter last,
                          OutputRandomAccessIter result)
  {
    if (first == last) return;
    detail::counting_sort_copy_dispatch(first, last, result,
      detail::make_linear_sort_function_dispatch(*first));
  }

  template <class RandomAccessIter>
  void counting_sort(RandomAccessIter first, RandomAccessIter last)
  {
    if (first == last) return;
    detail::counting_sort_dispatch(first, last,
      detail::make_linear_sort_function_dispatch(*first));
  }
 
} // namespace boost

#endif // BOOST_COUNTING_SORT_HPP
