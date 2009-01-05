#ifndef BOOST_PARTITION_POINT_HPP
#define BOOST_PARTITION_POINT_HPP

#if defined(_MSC_VER)&&(_MSC_VER>=1200)
#pragma once
#endif

#include <boost/iterator/iterator_traits.hpp>

namespace boost {

template<class It, class UnaryPred>
It partition_point( It itBegin, It itEnd, UnaryPred pred ) {
#ifdef _DEBUG /* is pred a partitioning? True must not occur after false */
	It itPartitionPoint = std::find_if(itBegin, itEnd, !boost::bind<bool>(pred, _1));
	_ASSERT( std::find_if(itPartitionPoint, itEnd, pred) == itEnd );
#endif
	boost::iterator_difference<It>::type nCount = std::distance(itBegin, itEnd);
	while( 0 < nCount ) {
		boost::iterator_difference<It>::type nCount2 = nCount / 2;
		It itMid = itBegin;
		std::advance(itMid, nCount2);
		if( pred(*itMid) ) {
			itBegin = ++itMid;
			nCount -= nCount2 + 1;
		} else {
			nCount = nCount2;
		}
	}
#ifdef _DEBUG /* we have already found the partition point; compare result */
	_ASSERT( itBegin == itPartitionPoint );
#endif
	return itBegin;
}

}; /* namespace boost */

#endif /* BOOST_PARTITION_POINT_HPP */
