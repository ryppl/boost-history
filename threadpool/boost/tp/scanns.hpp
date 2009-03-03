//  Copyright (c) 2008 Oliver Kowalke. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TP_SCANNS_H
#define BOOST_TP_SCANNS_H

#include <cstddef>

#include <boost/tp/exceptions.hpp>

namespace boost { namespace tp
{

class scanns
{
private:
	std::size_t	value_;

public:
	explicit scanns( std::size_t value);

	operator std::size_t () const;
};
} }

#endif // BOOST_TP_SCANNS_H
