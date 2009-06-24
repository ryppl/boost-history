//  Copyright (c) 2008 Oliver Kowalke. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_FIBERS_WRAPPER_H
#define BOOST_FIBERS_WRAPPER_H

#include <boost/fibers/context.hpp>

namespace boost {
namespace fibers
{
template< typename Act >
class wrapper
{
private:
	Act	act_;

public:
	wrapper( Act act)
	: act_( act)
	{}

	typename Act::result_type operator()( context &)
	{ return act_(); }
};

template< typename Act >
wrapper< Act > wrap( Act act)
{ return wrapper< Act >( act); }
} }

#endif // BOOST_FIBERS_WRAPPER_H

