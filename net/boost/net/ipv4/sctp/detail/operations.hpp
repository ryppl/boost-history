//  Copyright (c) 2001-2008 Oliver Kowalke. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_NET_IPV4_SCTP_DETAIL_OPERATIONS_H
#define BOOST_NET_IPV4_SCTP_DETAIL_OPERATIONS_H

#include <boost/net/detail/config.hpp>

extern "C"
{
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/sctp.h>
}

#include <cerrno>

#include <boost/config.hpp>
#include <boost/system/error_code.hpp>

#include <boost/net/constants.hpp>
#include <boost/net/error.hpp>
#include <boost/net/types.hpp>

namespace boost { namespace net {
namespace ipv4 { namespace sctp {
namespace detail
{
#if defined BOOST_NET_SCTP
inline
void sctp_bindx(
	native_handle_type handle,
	sockaddr_type const* addr,
	int count,
	int flags,
	system::error_code & ec)
{
	if ( ::sctp_bindx(
			handle,
			const_cast< sockaddr_type * >( addr),
			count,
			flags) == -1)
		ec = system::error_code(
			errno,
			system::get_system_category() );
}

inline
void sctp_connectx(
	native_handle_type handle,
	sockaddr_type const* addr,
	int count,
	system::error_code & ec)
{
	if ( ::sctp_connectx(
			handle,
			const_cast< sockaddr_type * >( addr),
			count) == -1)
		ec = system::error_code(
			errno,
			system::get_system_category() );
}

inline
void sctp_opt_info(
	native_handle_type handle,
	sctp_assoc_type assoc_id,
	int opt,
	void * arg,
	sockaddr_len_type * len,
	system::error_code & ec)
{
	if ( ::sctp_opt_info(
			handle,
			assoc_id,
			opt,
			arg,
			len) == -1)
		ec = system::error_code(
			errno,
			system::get_system_category() );
}

inline
native_handle_type sctp_peeloff(
	native_handle_type handle,
	sctp_assoc_type assoc_id,
	system::error_code & ec)
{
	native_handle_type new_handle( ::sctp_peeloff( handle, assoc_id) );
	if ( new_handle == invalid_native_handle)
		ec = system::error_code(
			errno,
			system::get_system_category() );
	return new_handle;
}

inline
int sctp_getpaddrs(
	native_handle_type handle,
	sctp_assoc_type assoc_id,
	sockaddr_type ** addrs,
	system::error_code & ec)
{
	int n( ::sctp_getpaddrs(
			handle,
			assoc_id,
			addrs) );
	if ( n == -1)
		ec = system::error_code(
			errno,
			system::get_system_category() );
	return n;
}

inline
void sctp_freepaddrs( sockaddr_type * addrs)
{ ::sctp_freepaddrs( addrs); }

inline
int sctp_getladdrs(
	native_handle_type handle,
	sctp_assoc_type assoc_id,
	sockaddr_type ** addrs,
	system::error_code & ec)
{
	int n( ::sctp_getladdrs(
			handle,
			assoc_id,
			addrs) );
	if ( n == -1)
		ec = system::error_code(
			errno,
			system::get_system_category() );
	return n;
}

inline
void sctp_freeladdrs( sockaddr_type * addrs)
{ ::sctp_freeladdrs( addrs); }
#endif
}}}}}

#endif // BOOST_NET_IPV4_SCTP_DETAIL_OPERATIONS_H
