
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TASK_DETAIL_SEMAPHORE_POSIX_H
#define BOOST_TASK_DETAIL_SEMAPHORE_POSIX_H

extern "C"
{
#include <semaphore.h>
}

#include <boost/utility.hpp>

#include <boost/task/detail/config.hpp>

#include <boost/config/abi_prefix.hpp>

namespace boost { namespace task
{
namespace detail
{
class BOOST_TASK_DECL semaphore : private boost::noncopyable
{
private:
	sem_t	sem_;

public:
	semaphore( int);

	~semaphore();

	void post();

	bool try_wait();
	void wait();

	int value();
};
}}}

#include <boost/config/abi_suffix.hpp>

#endif // BOOST_TASK_DETAIL_SEMAPHORE_POSIX_H
