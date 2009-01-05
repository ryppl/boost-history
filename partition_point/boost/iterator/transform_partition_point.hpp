#ifndef BOOST_TRANSFORM_PARTITION_POINT_HPP
#define BOOST_TRANSFORM_PARTITION_POINT_HPP

#if defined(_MSC_VER)&&(_MSC_VER>=1200)
#pragma once
#endif

#include <boost/partition_point/partition_point.hpp>
#include <boost/iterator/transform_iterator.hpp>
#include <boost/bind.hpp> // for bind

namespace boost {

/* Template functions lower_bound, upper_bound, equal_range and binary_search
expressed in terms of partition_point, special version for transform_iterator */
template< class UnaryFunc, class Iterator, class UnaryPred >
transform_iterator<UnaryFunc, Iterator> partition_point( transform_iterator<UnaryFunc, Iterator> itBegin, transform_iterator<UnaryFunc, Iterator> itEnd, UnaryPred pred ) {
  using boost::partition_point;
  return boost::make_transform_iterator(
    partition_point(
      itBegin.base(),
      itEnd.base(),
	  boost::bind( pred, boost::bind( itBegin.functor(), _1 ) )
    ),
    itBegin.functor()
  );
}

}; /* namespace */

#endif // BOOST_TRANSFORM_PARTITION_POINT_HPP