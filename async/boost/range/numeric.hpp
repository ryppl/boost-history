///////////////////////////////////////////////////////////////////////////////
/// \file algorithm.hpp
///   Contains range-based versions of the std algorithms
//
/////////////////////////////////////////////////////////////////////////////

// Copyright 2006 Thorsten Ottosen.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
// 
// Copyright 2004 Eric Niebler.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#if defined(_MSC_VER) && _MSC_VER >= 1000
	#pragma once
#endif

#ifndef BOOST_RANGE_NUMERIC_HPP
#define BOOST_RANGE_NUMERIC_HPP

#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>
#include <numeric>


namespace boost
{
	template< class InputRng, class T >
	inline T accumulate( const InputRng& r, T init )
	{
		return std::accumulate( boost::begin(r), boost::end(r), init );
	}

	template< class InputRng, class T, class BinaryOperation >
	inline T accumulate( const InputRng& r, T init, BinaryOperation bin )
	{
		return std::accumulate( boost::begin(r), boost::end(r), init, bin );
	}


	template< class InputRng, class InputIterator, class T >
	inline T inner_product ( const InputRng& r, InputIterator first, T init )
	{
		return std::inner_product( boost::begin(r), boost::end(r), 
								   first, init ); 
	}

	template< class InputRng, class InputIterator, class T, 
		      class BinaryOperation1, class BinaryOperation2 >
	inline T inner_product ( const InputRng& r, InputIterator first, T init, 
							 BinaryOperation1 bin1, BinaryOperation2 bin2 )
	{
		return std::inner_product( boost::begin(r), boost::end(r), 
								   first, init, bin1, bin2 ); 
	}

	template< class InputRng, class OutputIterator >
	inline OutputIterator partial_sum ( const InputRng& r, 
										OutputIterator result )
	{
		return std::partial_sum( boost::begin(r), boost::end(r), result );
	}

	template< class InputRng, class OutputIterator, class BinaryOperation >
	inline OutputIterator partial_sum ( const InputRng& r, OutputIterator result, 
										BinaryOperation bin )
	{
		return std::partial_sum( boost::begin(r), boost::end(r), result, bin );
	}

	template< class InputRng, class OutputIterator >
	inline OutputIterator adjacent_difference ( const InputRng& r,
												OutputIterator result )
	{
		return std::adjacent_difference( boost::begin(r), boost::end(r), 
										 result );
	}

	template< class InputRng, class OutputIterator, class BinaryOperation >
	inline OutputIterator adjacent_difference ( const InputRng& r,
												OutputIterator result,
												BinaryOperation bin )
	{
		return std::adjacent_difference( boost::begin(r), boost::end(r), 
										 result, bin );
	}
	
}

#endif
