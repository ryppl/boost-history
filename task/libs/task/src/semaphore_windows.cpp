
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "boost/task/semaphore.hpp"

#include <cerrno>
#include <limits>

#include <boost/system/error_code.hpp>
#include <boost/system/system_error.hpp>

#include "boost/task/exceptions.hpp"
#include "boost/task/utility.hpp"

namespace boost {
namespace task {

semaphore::semaphore( int value) :
	handle_()
{
	if ( ( handle_ = ::CreateSemaphore( 0, value, (std::numeric_limits< int >::max)(), 0) ) == 0)
		throw system::system_error( ::GetLastError(), system::system_category);
}

semaphore::~semaphore()
{ ::CloseHandle( handle_); }

void
semaphore::post()
{
	if ( ! ::ReleaseSemaphore( handle_, 1, 0) )
		throw system::system_error( ::GetLastError(), system::system_category);
}

void
semaphore::wait()
{
	if ( this_task::runs_in_pool() )
	{
		while ( ! try_wait() )
			this_task::block();
	}
	else
	{
		if ( ::WaitForSingleObject( handle_, INFINITE) != WAIT_OBJECT_0)
			throw system::system_error( ::GetLastError(), system::system_category);
	}
}

bool 
semaphore::try_wait()
{
	switch ( ::WaitForSingleObject( handle_, 0) )
	{
	case WAIT_OBJECT_0:
		return true;
	case WAIT_TIMEOUT:
		return false;
	default:
		throw system::system_error( ::GetLastError(), system::system_category);
	}
	return true;
}

int
semaphore::value()
{
	int value( 0);
	if ( ::WaitForSingleObject( handle_, 0) != WAIT_TIMEOUT)
	{
		if ( ! ::ReleaseSemaphore( handle_, 1, ( LPLONG) & value) )
			throw system::system_error( ::GetLastError(), system::system_category);
		++ value;
	}
	return value;
}

}}
