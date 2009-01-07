#ifndef BOOST_REVERSE_PARTITION_POINT_HPP
#define BOOST_REVERSE_PARTITION_POINT_HPP

#if defined(_MSC_VER)&&(_MSC_VER>=1200)
#pragma once
#endif

#include <boost/partition_point/partition_point.hpp>
#include <boost/iterator/reverse_iterator.hpp>
#include <boost/bind.hpp>

namespace boost {

/* Template functions lower_bound, upper_bound, equal_range and binary_search
expressed in terms of partition_point, special version for reverse_iterator */
template< class Iterator, class UnaryPred >
reverse_iterator<Iterator> partition_point( reverse_iterator<Iterator> itBegin, reverse_iterator<Iterator> itEnd, UnaryPred pred ) {
  using boost::partition_point;
  return reverse_iterator<Iterator>(
    partition_point(
      itEnd.base(),
      itBegin.base(),
	  !boost::bind<bool>(pred,_1)
    )
  );
}

}; /* namespace */

#endif // BOOST_REVERSE_PARTITION_POINT_HPP
