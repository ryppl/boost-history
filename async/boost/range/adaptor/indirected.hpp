// Boost.Range library
//
//  Copyright Thorsten Ottosen, Neil Groves 2006 - 2008. Use, modification and
//  distribution is subject to the Boost Software License, Version
//  1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
// For more information, see http://www.boost.org/libs/range/
//

#ifndef BOOST_RANGE_ADAPTOR_INDIRECTED_HPP
#define BOOST_RANGE_ADAPTOR_INDIRECTED_HPP

#include <boost/range/iterator_range.hpp>
#include <boost/iterator/indirect_iterator.hpp>

namespace boost
{
	namespace range_detail
	{
		template< class R >
		struct indirect_range : 
			public boost::iterator_range< 
			          boost::indirect_iterator<
			            BOOST_DEDUCED_TYPENAME range_iterator<R>::type 
			                                  >
			                             >
		{
		private:
			typedef boost::iterator_range< 
			          boost::indirect_iterator<
			            BOOST_DEDUCED_TYPENAME range_iterator<R>::type 
			                                  >
			                             >
				base;
			
		public:
			indirect_range( R& r ) 
			: base( r )
			{ }
		};

		struct indirect_forwarder {};
		
		template< class InputRng >
		inline indirect_range<InputRng> 
		operator|( InputRng& r, indirect_forwarder )
		{
			return indirect_range<InputRng>( r );   
		}
	
		template< class InputRng >
		inline indirect_range<const InputRng> 
		operator|( const InputRng& r, indirect_forwarder )
		{
			return indirect_range<const InputRng>( r );   
		}
		
	} // 'range_detail'

	using range_detail::indirect_range;

	namespace adaptors
	{ 
		namespace
		{
			const range_detail::indirect_forwarder indirected = 
				                            range_detail::indirect_forwarder();
		}
	}

	template<class InputRng>
	inline indirect_range<InputRng>
	make_indirected_range(InputRng& rng)
	{
		return rng | adaptors::indirected;
	}

	template<class InputRng>
	inline indirect_range<const InputRng>
	make_indirected_range(const InputRng& rng)
	{
		return rng | adaptors::indirected;
	}
}

#endif
