// Boost.Range library
//
//  Copyright Thorsten Ottosen, Neil Groves 2006 - 2008. Use, modification and
//  distribution is subject to the Boost Software License, Version
//  1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
// For more information, see http://www.boost.org/libs/range/
//

#ifndef BOOST_RANGE_ADAPTOR_SLICED_HPP
#define BOOST_RANGE_ADAPTOR_SLICED_HPP

#include <boost/range/adaptor/argument_fwd.hpp>
#include <boost/range/size_type.hpp>
#include <boost/range/iterator_range.hpp>

namespace boost
{
	namespace range_detail
	{
		template< class T >
		struct slice_holder 
			: holder2<std::size_t> 
		{
			slice_holder( std::size_t t, std::size_t u ) 
			    : holder2<std::size_t>(t,u)
			{ }
		};

		template< class R, class H >
		inline iterator_range< BOOST_DEDUCED_TYPENAME range_iterator<R>::type > 
		sliced_impl( R& r, const H& f )
		{
			BOOST_ASSERT( f.val1 <= f.val2 && 
						  "error in slice indexes" );
            BOOST_ASSERT( static_cast<std::size_t>(boost::size(r)) >= f.val2 && 
						  "second slice index out of bounds" );

			return make_iterator_range( r, f.val1, 
										f.val2 - boost::size(r) ); 
		}
		
		template< class RandomAccessRng, class Int >
		inline iterator_range< 
			     BOOST_DEDUCED_TYPENAME range_iterator<RandomAccessRng>::type > 
		operator|( RandomAccessRng& r, const slice_holder<Int>& f )
		{
			return sliced_impl( r, f );
		}

		template< class RandomAccessRng, class Int >
		inline iterator_range< 
				 BOOST_DEDUCED_TYPENAME range_iterator<const RandomAccessRng>::type > 
		operator|( const RandomAccessRng& r, const slice_holder<Int>& f )
		{
			return sliced_impl( r, f );
		}

	} // 'range_detail'

	namespace adaptors
	{ 
		namespace
		{
			const range_detail::forwarder2<range_detail::slice_holder> 
  	           sliced = range_detail::forwarder2<range_detail::slice_holder>();
		}
	}
	
	template<class RandomAccessRng>
	inline iterator_range<
	        BOOST_DEDUCED_TYPENAME range_iterator<RandomAccessRng>::type>
	make_sliced_range(RandomAccessRng& rng, std::size_t t, std::size_t u)
	{
	    return rng | adaptors::sliced(t, u);
	}
	
	template<class RandomAccessRng>
	inline iterator_range<
	        BOOST_DEDUCED_TYPENAME range_iterator<const RandomAccessRng>::type>
	make_sliced_range(const RandomAccessRng& rng, std::size_t t, std::size_t u)
	{
	    return rng | adaptors::sliced(t, u);
	}
}

#endif
