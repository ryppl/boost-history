#ifndef BOOST_INDIRECT_PARTITION_POINT_HPP
#define BOOST_INDIRECT_PARTITION_POINT_HPP

#if defined(_MSC_VER)&&(_MSC_VER>=1200)
#pragma once
#endif

#include <boost/partition_point/partition_point.hpp>
#include <boost/iterator/indirect_iterator.hpp>
#include <boost/bind.hpp> // for bind

namespace boost {

// As a functor for dereferencing, operator* is not defined for pointers, 
// so a pointer-to-member cannot be used. There is also no functor defined 
// in std for the dereference operator, so we define our own.
// It takes the functor by value just like boost::bind.
namespace detail { namespace iterator {
	template< class UnaryPred >
	struct chain_dereference {
	private:
		UnaryPred m_pred;

	public:
		chain_dereference( UnaryPred pred )
		:	m_pred( pred ) {};

		template< class T >
		bool operator()( T const& t ) const {
			return m_pred( *t );
		}
	};
}} // namespace detail::iterator

/* Template functions lower_bound, upper_bound, equal_range and binary_search
expressed in terms of partition_point, special version for indirect_iterator */
template< class Iterator, class UnaryPred >
indirect_iterator<Iterator> partition_point( indirect_iterator<Iterator> itBegin, indirect_iterator<Iterator> itEnd, UnaryPred pred ) {
  using boost::partition_point;
  return boost::make_indirect_iterator(
    partition_point(
      itBegin.base(),
      itEnd.base(),
	  detail::iterator::chain_dereference<UnaryPred>( pred ) 
    )
  );
}

}; /* namespace */

#endif // BOOST_INDIRECT_PARTITION_POINT_HPP