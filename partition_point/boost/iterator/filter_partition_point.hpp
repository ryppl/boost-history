#ifndef BOOST_FILTER_PARTITION_POINT_HPP
#define BOOST_FILTER_PARTITION_POINT_HPP

#if defined(_MSC_VER)&&(_MSC_VER>=1200)
#pragma once
#endif

#include <boost/partition_point/partition_point.hpp>
#include <boost/iterator/filter_iterator.hpp>

namespace boost {

/* Template functions lower_bound, upper_bound, equal_range and binary_search
expressed in terms of partition_point, special version for filter_iterator */
template< class FilterPred, class Iterator, class UnaryPred >
filter_iterator<FilterPred, Iterator> partition_point( filter_iterator<FilterPred, Iterator> itBegin, filter_iterator<FilterPred, Iterator> itEnd, UnaryPred pred ) {
  _ASSERT(itBegin.end()==itEnd.end());
  using boost::partition_point;
  return boost::make_filter_iterator(
    itBegin.predicate(),
    partition_point(
      itBegin.base(),
      itEnd.base(),
      pred
    ),
    itBegin.end()
  );
}

}; /* namespace */

#endif // BOOST_FILTER_PARTITION_POINT_HPP
