//  Copyright (c) 2001-2008 Oliver Kowalke. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_NET_DETAIL_OPERATIONS_H
#define BOOST_NET_DETAIL_OPERATIONS_H

#include <cerrno>
#include <csignal>
#include <string>

#include <boost/net/detail/config.hpp>

extern "C"
{
#include <fcntl.h>
#include <net/if.h>
#include <poll.h>
#include <pthread.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
}

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/system/error_code.hpp>

#include <boost/net/constants.hpp>
#include <boost/net/error.hpp>
#include <boost/net/shutdown_mode.hpp>
#include <boost/net/types.hpp>

namespace boost { namespace net {
namespace detail
{
//------------------------------------------

inline
void sigaddset(
	sigset_type * set,
	int signo,
	system::error_code & ec)
{
	if ( ::sigaddset( set, signo) == -1)
		ec = system::error_code(
			errno,
			system::get_system_category() );
}

inline
void sigdelset(
	sigset_type * set,
	int signo,
	system::error_code & ec)
{
	if ( ::sigdelset( set, signo) == -1)
		ec = system::error_code(
			errno,
			system::get_system_category() );
}

inline
void sigfillset(
	sigset_type * set,
	system::error_code & ec)
{
	if ( ::sigfillset( set) == -1)
		ec = system::error_code(
			errno,
			system::get_system_category() );
}

inline
void sigemptyset(
	sigset_type * set,
	system::error_code & ec)
{
	if ( ::sigemptyset( set) == -1)
		ec = system::error_code(
			errno,
			system::get_system_category() );
}

inline
void sigaction(
	int signo,
	sigaction_type const* act,
	sigaction_type * oact,
	system::error_code & ec)
{
	if ( ::sigaction(
			signo,
			act,
			oact) == -1)
		ec = system::error_code(
			errno,
			system::get_system_category() );
}

inline
void sigwait(
	sigset_type const* se,
	int * signo,
	system::error_code & ec)
{
	int errno_( ::sigwait( se, signo) );
	if ( errno_ != 0)
		ec = system::error_code(
			errno_,
			system::get_system_category() );
}

inline
void pthread_sigmask(
	int how,
	sigset_type const* set,
	sigset_type * oset,
	system::error_code & ec)
{
	int errno_(
		::pthread_sigmask(
			how,
			set,
			oset) );
	if ( errno_ != 0)
		ec = system::error_code(
			errno_,
			system::get_system_category() );
}

//------------------------------------------

inline
native_handle_type create_socket(
	int family,
	int socktype,
	int protocol,
	system::error_code & ec)
{
	native_handle_type handle( invalid_native_handle);
	handle = ::socket( family, socktype, protocol);
	if ( handle == invalid_native_handle)
		ec = system::error_code(
			errno,
			system::get_system_category() );
	return handle;
}

inline
void close_socket( native_handle_type handle, system::error_code & ec)
{
	if ( ::close( handle) == -1)
		ec = system::error_code(
			errno,
			system::get_system_category() );
}

//------------------------------------------

inline
bool non_blocking( native_handle_type handle, system::error_code & ec)
{
	int val( ::fcntl( handle, F_GETFL, 0) );
	if ( val == -1)
		ec = system::error_code(
			errno,
			system::get_system_category() );
	return val & O_NONBLOCK;
}

inline
void non_blocking( native_handle_type handle, bool mode, system::error_code & ec)
{
	int val( ::fcntl( handle, F_GETFL, 0) );
	if ( val == -1)
	{
		ec = system::error_code(
			errno,
			system::get_system_category() );
		return;
	}

	if ( mode)
		val |= O_NONBLOCK;
	else
		val &= ~O_NONBLOCK;

	if ( ::fcntl( handle, F_SETFL, val) == -1)
		ec = system::error_code(
			errno,
			system::get_system_category() );
}

//------------------------------------------

inline
void shutdown( native_handle_type handle, int mode, system::error_code & ec)
{
	if ( ::shutdown( handle, mode) == -1)
		ec = system::error_code(
			errno,
			system::get_system_category() );
}

//------------------------------------------

inline
void getsockname(
	native_handle_type handle,
	sockaddr_type * addr,
	sockaddr_len_type * len,
	system::error_code & ec)
{
	if ( ::getsockname(
			handle,
			addr,
			len) == -1)
		ec = system::error_code(
			errno,
			system::get_system_category() );
}

inline
void getpeername(
	native_handle_type handle,
	sockaddr_type * addr,
	sockaddr_len_type * len,
	system::error_code & ec)
{
	if ( ::getpeername(
			handle,
			addr,
			len) == -1)
		ec = system::error_code(
			errno,
			system::get_system_category() );
}

//------------------------------------------

inline
void bind(
	native_handle_type handle,
	sockaddr_type const* addr,
	sockaddr_len_type len,
	system::error_code & ec)
{
	if ( ::bind(
			handle,
			addr,
			len) == -1)
		ec = system::error_code(
				errno,
				system::get_system_category() );
}

//------------------------------------------

inline
void listen(
	native_handle_type handle,
	int backlog,
	system::error_code & ec)
{
	if ( ::listen(
			handle,
			backlog) == -1)
		ec = system::error_code(
				errno,
				system::get_system_category() );
}

//------------------------------------------

inline
native_handle_type accept(
	native_handle_type handle,
	sockaddr_type * addr,
	sockaddr_len_type * len,
	system::error_code & ec)
{
	native_handle_type new_handle(
		::accept(
			handle,
			addr,
			len) );
	if ( new_handle == invalid_native_handle)
		ec = system::error_code(
			errno,
			system::get_system_category() );
	return new_handle;
}

//------------------------------------------

inline
void connect(
	native_handle_type handle,
	sockaddr_type const* addr,
	sockaddr_len_type len,
	system::error_code & ec)
{
	if ( ::connect(
			handle,
			addr,
			len) == -1)
		ec = system::error_code(
			errno,
			system::get_system_category() );
}

//------------------------------------------

inline
char * getenv( std::string const& env)
{ return ::getenv( env.c_str() ); }

//------------------------------------------

inline
void is_readable(
	native_handle_type handle,
	posix_time::time_duration const& timeout,
	system::error_code & ec)
{
	int ms( timeout.total_milliseconds() );

	pollfd_type pfd;
	pfd.fd = handle;
	pfd.events = POLLIN;
	pfd.revents = 0;

	int nready( ::poll( & pfd, 1, ms) );
	if ( nready < 0)
		ec = system::error_code(
			errno,
			system::get_system_category() );
	else if ( nready == 0)
		ec = system::error_code(
			ETIMEDOUT,
			system::get_system_category() );
}

inline
void is_writeable(
	native_handle_type handle,
	posix_time::time_duration const& timeout,
	system::error_code & ec)
{
	int ms( timeout.total_milliseconds() );

	pollfd_type pfd;
	pfd.fd = handle;
	pfd.events = POLLOUT;
	pfd.revents = 0;

	int nready( ::poll( & pfd, 1, ms) );
	if ( nready < 0)
		ec = system::error_code(
			errno,
			system::get_system_category() );
	else if ( nready == 0)
		ec = system::error_code(
			ETIMEDOUT,
			system::get_system_category() );
}

//------------------------------------------

inline
void getsockopt(
	native_handle_type handle,
	int level,
	int name,
	void * value,
	sockaddr_len_type * len,
	system::error_code & ec)
{
	if ( ::getsockopt(
			handle,
			level,
			name,
			value,
			len) == -1)
		ec = system::error_code(
			errno,
			system::get_system_category() );
}

inline
void setsockopt(
	native_handle_type handle,
	int level,
	int name,
	void const* value,
	sockaddr_len_type len,
	system::error_code & ec)
{
	if ( ::setsockopt(
			handle,
			level,
			name,
			value,
			len) == -1)
		ec = system::error_code(
			errno,
			system::get_system_category() );
}

//------------------------------------------

inline
u_long_type if_nametoindex(
	std::string const& ifname,
	system::error_code & ec)
{
	u_long_type ifindex( ::if_nametoindex( ifname.c_str() ) );
	if ( ifindex == 0)
		ec = system::error_code(
			errno,
			system::get_system_category() );
	return ifindex;
}

//------------------------------------------
}}}

#endif // BOOST_NET_DETAIL_OPERATIONS_H
