#ifndef BOOST_PARTITION_RANGE_ALGORITHMS_HPP
#define BOOST_PARTITION_RANGE_ALGORITHMS_HPP

#if defined(_MSC_VER)&&(_MSC_VER>=1200)
#pragma once
#endif

#include <boost/partition_point/partition_algorithms.hpp>
#include <boost/config.hpp>
#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>
#include <boost/range/sub_range.hpp>
#include <boost/partition_point/detail/lazy_typedef.hpp>

namespace boost {
	namespace improved {
		///////////////////////////////////////////////////////////////////////////////////////////////////////////
		// Versions of range_const/mutable_iterator<C> that if C::(const_)iterator is undefined,
		// do not define range_const/mutable_iterator<C>::type, instead of defining it but 
		// failing to compile when type is actually accessed. This helps disambiguating overloads.

		//////////////////////////////////////////////////////////////////////////
		// default
		//////////////////////////////////////////////////////////////////////////
	    
		namespace detail {
			BOOST_DECLARE_LAZY_TYPEDEF( const_iterator )
		}

		template< typename C >
		struct range_const_iterator: detail::lazy_typedef_const_iterator<C>
		{};
	    
		//////////////////////////////////////////////////////////////////////////
		// pair
		//////////////////////////////////////////////////////////////////////////

		template< typename Iterator >
		struct range_const_iterator< std::pair<Iterator,Iterator> >
		{
			typedef Iterator type;
		};
	    
		//////////////////////////////////////////////////////////////////////////
		// array
		//////////////////////////////////////////////////////////////////////////

		template< typename T, std::size_t sz >
		struct range_const_iterator< T[sz] >
		{
			typedef const T* type;
		};

		//////////////////////////////////////////////////////////////////////////
		// default
		//////////////////////////////////////////////////////////////////////////
	    
		namespace detail {
			BOOST_DECLARE_LAZY_TYPEDEF( iterator )
		}

		template< typename C >
		struct range_mutable_iterator: detail::lazy_typedef_iterator<C>
		{};
	    
		//////////////////////////////////////////////////////////////////////////
		// pair
		//////////////////////////////////////////////////////////////////////////

		template< typename Iterator >
		struct range_mutable_iterator< std::pair<Iterator,Iterator> >
		{
			typedef Iterator type;
		};

		//////////////////////////////////////////////////////////////////////////
		// array
		//////////////////////////////////////////////////////////////////////////

		template< typename T, std::size_t sz >
		struct range_mutable_iterator< T[sz] >
		{
			typedef T* type;
		};
	} // namespace improved

	namespace partition_algorithms_adl_barrier {

		template< typename Rng, typename Val >
		BOOST_DEDUCED_TYPENAME boost::improved::range_const_iterator<Rng>::type lower_bound(Rng const& rng,Val const& x) {
			return boost::lower_bound(boost::begin(rng),boost::end(rng),x);
		}

		template< typename Rng, typename Val >
		BOOST_DEDUCED_TYPENAME boost::improved::range_mutable_iterator<Rng>::type lower_bound(Rng & rng,Val const& x) {
			return boost::lower_bound(boost::begin(rng),boost::end(rng),x);
		}

		template< typename Rng, typename Val, typename BinPred>
		BOOST_DEDUCED_TYPENAME boost::improved::range_const_iterator<Rng>::type lower_bound(Rng const& rng,Val const& x,BinPred pred) {
			return boost::lower_bound(boost::begin(rng),boost::end(rng),x,pred);
		}

		template< typename Rng, typename Val, typename BinPred>
		BOOST_DEDUCED_TYPENAME boost::improved::range_mutable_iterator<Rng>::type lower_bound(Rng & rng,Val const& x,BinPred pred) {
			return boost::lower_bound(boost::begin(rng),boost::end(rng),x,pred);
		}

		template< typename Rng, typename Val >
		BOOST_DEDUCED_TYPENAME boost::improved::range_const_iterator<Rng>::type upper_bound(Rng const& rng,Val const& x) {
			return boost::upper_bound(boost::begin(rng),boost::end(rng),x);
		}

		template< typename Rng, typename Val >
		BOOST_DEDUCED_TYPENAME boost::improved::range_mutable_iterator<Rng>::type upper_bound(Rng & rng,Val const& x) {
			return boost::upper_bound(boost::begin(rng),boost::end(rng),x);
		}

		template< typename Rng, typename Val, typename BinPred>
		BOOST_DEDUCED_TYPENAME boost::improved::range_const_iterator<Rng>::type upper_bound(Rng const& rng,Val const& x,BinPred pred) {
			return boost::upper_bound(boost::begin(rng),boost::end(rng),x,pred);
		}

		template< typename Rng, typename Val, typename BinPred>
		BOOST_DEDUCED_TYPENAME boost::improved::range_mutable_iterator<Rng>::type upper_bound(Rng & rng,Val const& x,BinPred pred) {
			return boost::upper_bound(boost::begin(rng),boost::end(rng),x,pred);
		}

		template< typename Rng, typename Val >
		bool binary_search(Rng const& rng,Val const& x) {
			return boost::binary_search(boost::begin(rng),boost::end(rng),x);
		}

		template< typename Rng, typename Val, typename BinPred>
		bool binary_search(Rng const& rng,Val const& x,BinPred pred) {
			return boost::binary_search(boost::begin(rng),boost::end(rng),x,pred);
		}

		template< typename Rng, typename Val >
		boost::sub_range<Rng const> equal_range(Rng const& rng,Val const& x) {
			return boost::equal_range(boost::begin(rng),boost::end(rng),x);
		}

		template< typename Rng, typename Val >
		boost::sub_range<Rng> equal_range(Rng & rng,Val const& x) {
			return boost::equal_range(boost::begin(rng),boost::end(rng),x);
		}

		template< typename Rng, typename Val, typename BinPred>
		boost::sub_range<Rng const> equal_range(Rng const& rng,Val const& x,BinPred pred) {
			return boost::equal_range(boost::begin(rng),boost::end(rng),x,pred);
		}

		template< typename Rng, typename Val, typename BinPred>
		boost::sub_range<Rng> equal_range(Rng & rng,Val const& x,BinPred pred) {
			return boost::equal_range(boost::begin(rng),boost::end(rng),x,pred);
		}
	} // namespace partition_algorithms_adl_barrier

} // namespace boost

#endif // BOOST_PARTITION_RANGE_ALGORITHMS_HPP