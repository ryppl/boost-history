//  Copyright (c) 2008 Oliver Kowalke. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_FIBERS_EXCEPTIONS_H
#define BOOST_FIBERS_EXCEPTIONS_H

#include <stdexcept>
#include <string>

namespace boost {
namespace fibers
{
class invalid_stacksize
: public std::invalid_argument
{
public:
    invalid_stacksize( std::string const& msg)
	: std::invalid_argument( msg)
	{}
};
} }

#endif // BOOST_FIBERS_EXCEPTIONS_H
