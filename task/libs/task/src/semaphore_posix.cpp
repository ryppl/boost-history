
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "boost/task/detail/semaphore_posix.hpp"

#include <cerrno>

#include <boost/system/error_code.hpp>
#include <boost/system/system_error.hpp>

namespace boost { namespace task
{
namespace detail
{
semaphore::semaphore( int value)
: sem_()
{
	if ( ::sem_init( & sem_, 0, value) == -1)
		throw system::system_error( errno, system::system_category);
}

semaphore::~semaphore()
{ ::sem_destroy( & sem_); }

void
semaphore::post()
{
	if ( ::sem_post( & sem_) == -1)
		throw system::system_error( errno, system::system_category);
}

void
semaphore::wait()
{
	if ( ::sem_wait( & sem_) == -1)
		throw system::system_error( errno, system::system_category);
}

bool 
semaphore::try_wait()
{
	if ( ::sem_trywait( & sem_) == -1)
	{
		if ( errno == EAGAIN)
			return false;
		else
			throw system::system_error( errno, system::system_category);
	}
	return true;
}

int
semaphore::value()
{
	int value( 0);
	if ( ::sem_getvalue( & sem_, & value) == -1)
		throw system::system_error( errno, system::system_category);
	return value;
}
}}}
