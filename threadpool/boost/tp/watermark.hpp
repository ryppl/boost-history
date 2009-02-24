//  Copyright (c) 2008 Oliver Kowalke. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TP_WATER_MARK_H
#define BOOST_TP_WATER_MARK_H

#include <cstddef>

#include <boost/tp/exceptions.hpp>

namespace boost { namespace tp
{
class high_watermark
{
private:
	std::size_t	value_;

public:
	explicit high_watermark( std::size_t value)
	: value_( value)
	{
		if ( value <= 0)
			throw invalid_watermark("high watermark must be greater than zero");
	}

	operator std::size_t () const
	{ return value_; }
};

class low_watermark
{
private:
	std::size_t	value_;

public:
	explicit low_watermark( std::size_t value)
	: value_( value)
	{
		if ( value < 0)
			throw invalid_watermark("low watermark must be greater than or equal to zero");
	}

	operator std::size_t () const
	{ return value_; }
};
} }

#endif // BOOST_TP_WATER_MARK_H
