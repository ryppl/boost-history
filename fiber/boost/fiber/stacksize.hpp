//  Copyright (c) 2008 Oliver Kowalke. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_FIBERS_STACKSIZE_H
#define BOOST_FIBERS_STACKSIZE_H

#include <cstddef>

#include <boost/fibers/exceptions.hpp>

namespace boost {
namespace fibers
{
class stacksize
{
private:
	std::size_t	value_;

public:
	explicit stacksize( std::size_t value)
	: value_( value)
	{ if ( value <= 0) throw invalid_stacksize("stacksize must be greater than zero"); }

	operator std::size_t () const
	{ return value_; }
};
} }

#endif // BOOST_FIBERS_STACKSIZE_H
