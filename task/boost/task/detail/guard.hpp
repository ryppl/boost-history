//  Copyright (c) 2008 Oliver Kowalke. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TASK_DETAIL_GUARD_H
#define BOOST_TASK_DETAIL_GUARD_H

#include <boost/utility.hpp>

namespace boost { namespace task
{
namespace detail
{
class guard : private noncopyable
{
private:
	unsigned int	&	active_worker_;

public:
	guard( unsigned int & active_worker);

	~guard();
};
} } }

#endif // BOOST_TASK_DETAIL_GUARD_H

