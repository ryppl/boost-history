// (C) Arno Schoedl 2008.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PARTITION_ALGORITHMS_HPP
#define BOOST_PARTITION_ALGORITHMS_HPP

#if defined(_MSC_VER)&&(_MSC_VER>=1200)
#pragma once
#endif

#include <boost/partition_point/partition_point.hpp>
#include <boost/ref.hpp> // for boost::cref
#include <boost/bind.hpp> // for bind
#include <functional> // for std::less
#include <utility> // for std::pair

namespace boost {

	namespace detail {
		struct generic_less {
			template< class T1, class T2 >
			bool operator()( T1 const& t1, T2 const& t2 ) const {
				return t1 < t2;
			}
		};
	}

	namespace partition_algorithms_adl_barrier {

		// std::bind1st/2nd require argument_type to be defined, at least in Dinkumware's implementation.
		// The standard does not require this for the predicate passed to lower_bound et. al.
		// We thus use boost::bind, which with explicit return type does not require any typedefs.

		template< typename It, typename Val, typename BinPred >
		It lower_bound(It itBegin,It itEnd,Val const& x,BinPred pred) {
			using boost::partition_point;
			return partition_point(itBegin,itEnd,boost::bind<bool>(pred,_1,boost::cref(x)));
		}

		template< typename It, typename Val, typename BinPred >
		It upper_bound(It itBegin,It itEnd,Val const& x,BinPred pred) {
			using boost::partition_point;
			return partition_point(itBegin,itEnd,!boost::bind<bool>(pred,boost::cref(x),_1));
		}

		template< typename It, typename Val, typename BinPred >
		bool binary_search(It itBegin,It itEnd,Val const& x,BinPred pred) {
			It it = boost::lower_bound(itBegin,itEnd,x,pred);
			return it!=itEnd && !pred(x,*it);
		}

		template< typename It, typename Val, typename BinPred >
		std::pair<It,It> equal_range(It itBegin,It itEnd,Val const& x,BinPred pred) {
			// Construct std::pair<It,It> initialized so that transform_iterator functor
			// does not have to be default-constructible. This is non-standard conformant,
			// but may be practical.
			itBegin=boost::lower_bound(itBegin,itEnd,x,pred);
			return std::pair<It,It>( itBegin, boost::upper_bound(itBegin,itEnd,x,pred) );
		}

		// According to http://www.open-std.org/jtc1/sc22/wg21/docs/lwg-defects.html#270
		// the less-than comparison may be carried out on unequal types. Thus 
		// we use our own implementation instead of std::less.

		template< typename It, typename Val >
		It lower_bound(It itBegin,It itEnd,Val const& x) {
			return boost::lower_bound( itBegin, itEnd, x, ::boost::detail::generic_less() );
		}

		template< typename It, typename Val >
		It upper_bound(It itBegin,It itEnd,Val const& x) {
			return boost::upper_bound( itBegin, itEnd, x, ::boost::detail::generic_less() );
		}

		template< typename It, typename Val >
		bool binary_search(It itBegin,It itEnd,Val const& x) {
			return boost::binary_search( itBegin, itEnd, x, ::boost::detail::generic_less() );
		}

		template< typename It, typename Val >
		std::pair<It,It> equal_range(It itBegin,It itEnd,Val const& x) {
			return boost::equal_range( itBegin, itEnd, x, ::boost::detail::generic_less() );
		}
	}

	using namespace partition_algorithms_adl_barrier;

} // namespace boost

#endif // BOOST_PARTITION_ALGORITHMS_HPP
