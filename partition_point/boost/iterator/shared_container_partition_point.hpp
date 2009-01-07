#ifndef BOOST_SHARED_CONTAINER_PARTITION_POINT_HPP
#define BOOST_SHARED_CONTAINER_PARTITION_POINT_HPP

#if defined(_MSC_VER)&&(_MSC_VER>=1200)
#pragma once
#endif

#include <boost/partition_point/partition_point.hpp>
#include <boost/iterator/shared_container_iterator.hpp>

namespace boost {

/* Template functions lower_bound, upper_bound, equal_range and binary_search
expressed in terms of partition_point, special version for shared_container_iterator */
template< class UnaryFunc, class Iterator, class UnaryPred >
shared_container_iterator<UnaryFunc, Iterator> partition_point( shared_container_iterator<UnaryFunc, Iterator> itBegin, shared_container_iterator<UnaryFunc, Iterator> itEnd, UnaryPred pred ) {
  using boost::partition_point;
  return boost::make_shared_container_iterator(
    partition_point(
      itBegin.base(),
      itEnd.base(),
	  pred
    ),
    itBegin.container_ref
  );
}

}; // namespace boost

#endif // BOOST_SHARED_CONTAINER_PARTITION_POINT_HPP