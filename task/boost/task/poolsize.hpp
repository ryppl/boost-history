
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TASK_POOLSIZE_H
#define BOOST_TASK_POOLSIZE_H

#include <cstddef>

#include <boost/config/abi_prefix.hpp>

namespace boost { namespace task
{
class poolsize
{
private:
	std::size_t	value_;

public:
	explicit poolsize( std::size_t value);

	operator std::size_t () const;
};
} }

#include <boost/config/abi_suffix.hpp>

#endif // BOOST_TASK_POOLSIZE_H
