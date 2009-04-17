
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TASK_SCANNS_H
#define BOOST_TASK_SCANNS_H

#include <cstddef>

#include <boost/task/detail/config.hpp>

#include <boost/config/abi_prefix.hpp>

namespace boost { namespace task
{
class BOOST_TASK_DECL scanns
{
private:
	std::size_t	value_;

public:
	explicit scanns( std::size_t value);

	operator std::size_t () const;
};
} }

#include <boost/config/abi_suffix.hpp>

#endif // BOOST_TASK_SCANNS_H
