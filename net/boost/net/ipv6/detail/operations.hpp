//  Copyright (c) 2001-2008 Oliver Kowalke. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_NET_IPV6_DETAIL_OPERATIONS_H
#define BOOST_NET_IPV6_DETAIL_OPERATIONS_H

#include <cerrno>

#include <boost/net/detail/config.hpp>

extern "C"
{
#ifdef	BOOST_NET_HAVE_ARPA_INET_H
#include <arpa/inet.h>
#endif

#ifdef	BOOST_NET_HAVE_NETINET_IN_H
#include <netinet/in.h>
#endif
}

#include <boost/config.hpp>
#include <boost/system/error_code.hpp>

#include <boost/net/constants.hpp>
#include <boost/net/error.hpp>
#include <boost/net/types.hpp>

namespace boost { namespace net {
namespace ipv6 { namespace detail
{
	inline
	u_long_type network2host_long( u_long_type value)
	{ return ntohl( value); }

	inline
	u_long_type host2network_long( u_long_type value)
	{ return htonl( value); }

	inline
	u_short_type network2host_short( u_short_type value)
	{ return ntohs( value); }

	inline
	u_short_type host2network_short( u_short_type value)
	{ return htons(value); }

	inline
	in6_addr_type inet6_pton(
		std::string const& ip_number,
		system::error_code & ec)
	{
		in6_addr_type addr;
		::bzero( & addr, sizeof addr);

		int result(
			::inet_pton(
				PF_INET6,
				ip_number.c_str(),
				& addr) );
		if ( result == 0)
			ec = system::error_code(
				EFAULT,
				system::get_system_category() );
		else if ( result < 0)
			ec = system::error_code(
				errno,
				system::get_system_category() );
		return addr;
	}

	inline
	const std::string inet_ntop(
		in6_addr_type const& addr,
		system::error_code & ec)
	{
		char buf[addrstrlen_ipv6];
		::bzero( buf, addrstrlen_ipv6);

		if ( ::inet_ntop(
				PF_INET6,
				& addr,
				buf,
				sizeof buf) == 0)
			ec = system::error_code(
				errno,
				system::get_system_category() );
		return buf;
	}


	inline
	void getaddrinfo(
		char const* host,
		char const* service,
		addrinfo_type const* hints,
		addrinfo_type ** result,
		system::error_code & ec)
	{
		int errno_(
			::getaddrinfo(
				host,
				service,
				hints,
				result) );
		if ( errno_ != 0)
			ec = system::error_code(
				errno_,
				get_gai_category() );
	}

	inline
	void getnameinfo(
		sockaddr_type const* addr,
		sockaddr_len_type addrlen,
		char * host,
		sockaddr_len_type hostlen,
		char * service,
		sockaddr_len_type servicelen,
		int flags,
		system::error_code & ec)
	{
		int errno_(
			::getnameinfo(
				addr,
				addrlen,
				host,
				hostlen,
				service,
				servicelen,
				flags) );
		if ( errno_ != 0)
			ec = system::error_code(
				errno_,
				get_gai_category() );
	}

	inline
	void freeaddrinfo( addrinfo_type * result)
	{ ::freeaddrinfo( result); }

	inline
	void gethostbyaddr_r(
		sockaddr_in6_type & addr,
		hostent_type & ent,
		char * buf,
		int buflen,
		system::error_code & ec)
	{
		hostent_type * result( 0);
#if defined(__hpux)
		void * addr_ = addr;
		result = ::gethostbyaddr(
			addr_,
			len,
			type);
#else
		void * vp( & addr.sin6_addr);
		int errno_( 0);
		::gethostbyaddr_r(
			static_cast< char * >( vp),
			sizeof( in6_addr_type),
			PF_INET6,
			& ent,
			buf,
			buflen,
			& result,
			& errno_);
		if ( errno_ != 0)
			ec = system::error_code(
				errno_,
				get_gai_category() );
#endif
	}
}}}}

#endif // BOOST_NET_IPV6_DETAIL_OPERATIONS_H
