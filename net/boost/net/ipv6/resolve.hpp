//  Copyright (c) 2001-2008 Oliver Kowalke. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_NET_IPV6_RESOLVE_H
#define BOOST_NET_IPV6_RESOLVE_H

#include <string>
#include <vector>

extern "C"
{
#include <netinet/in.h>
}

#include <boost/lexical_cast.hpp>
#include <boost/none.hpp>
#include <boost/optional.hpp>
#include <boost/system/error_code.hpp>
#include <boost/system/system_error.hpp>
#include <boost/utility.hpp>

#include <boost/net/ipv6/detail/operations.hpp>
#include <boost/net/ipv6/address_info.hpp>
#include <boost/net/ipv6/utility.hpp>
#include <boost/net/types.hpp>

namespace boost { namespace net {
namespace ipv6
{
namespace detail
{
struct addrinfo_deleter
{
	void operator()( addrinfo_type * ptr)
	{
		detail::freeaddrinfo( ptr);
		ptr = 0;
	}
};
}

class resolve_hints
{
private:
	int		flags_;
	int		socket_;
	int		protocol_;

public:
	resolve_hints()
	:
	flags_( 0),
	socket_( socket_unspec),
	protocol_( protocol_unspec)
	{}

	resolve_hints operator|( resolve_flags flgs)
	{
		flags_ = flgs;
		return * this;
	}

	resolve_hints operator|( socket_type sck)
	{
		socket_ |= sck;
		return * this;
	}

	resolve_hints operator|( protocol_type proto)
	{
		protocol_ |= proto;
		return * this;
	}

	int flags() const
	{ return flags_; }

	int socket() const
	{ return socket_; }

	int protocol() const
	{ return protocol_; }
};

inline
const std::vector< address_info > resolve(
	std::string const& host,
	std::string const& service,
	optional< resolve_hints > hints = none)
{
	BOOST_ASSERT( ! host.empty() );
	BOOST_ASSERT( ! service.empty() );

	addrinfo_type info_hints, * tmp( 0);

	// hints
	::bzero( & info_hints, sizeof( addrinfo_type) );
	info_hints.ai_family = PF_INET6;
	if ( hints)
	{
		info_hints.ai_flags = hints->flags();
		info_hints.ai_socktype= hints->socket();
		info_hints.ai_protocol = hints->protocol();
	}

	system::error_code ec;
	detail::getaddrinfo(
		host.c_str(),
		service.c_str(),
		& info_hints,
		& tmp,
		ec);
	if ( ec) throw_exception( system::system_error( ec) );

	shared_ptr< addrinfo_type > info_ptr( tmp, detail::addrinfo_deleter() );

	std::vector< address_info > infos;

	if ( ! info_ptr) return infos;
	for ( addrinfo_type * ptr( info_ptr.get() ); ptr; ptr = ptr->ai_next)
		infos.push_back( address_info( info_ptr, ptr) );
	return infos;
}

inline
const std::vector< address_info > resolve(
	std::string const& host,
	unsigned int port,
	optional< resolve_hints > hints = none)
{
	BOOST_ASSERT( ! host.empty() );

	addrinfo_type info_hints, * tmp( 0);

	// hints
	::bzero( & info_hints, sizeof( addrinfo_type) );
	info_hints.ai_family = PF_INET6;
	if ( hints)
	{
		info_hints.ai_flags = hints->flags();
		info_hints.ai_socktype= hints->socket();
		info_hints.ai_protocol = hints->protocol();
	}
	info_hints.ai_flags |= AI_NUMERICSERV;

	std::string service( boost::lexical_cast< std::string >( port) );

	system::error_code ec;
	detail::getaddrinfo(
		host.c_str(),
		service.c_str(),
		& info_hints,
		& tmp,
		ec);
	if ( ec) throw_exception( system::system_error( ec) );

	shared_ptr< addrinfo_type > info_ptr( tmp, detail::addrinfo_deleter() );

	std::vector< address_info > infos;

	if ( ! info_ptr) return infos;
	for ( addrinfo_type * ptr( info_ptr.get() ); ptr; ptr = ptr->ai_next)
		infos.push_back( address_info( info_ptr, ptr) );
	return infos;
}

inline
const std::vector< address_info > resolve(
	std::string const& service,
	optional< resolve_hints > hints = none)
{
	BOOST_ASSERT( ! service.empty() );

	addrinfo_type info_hints, * tmp( 0);

	// hints
	::bzero( & info_hints, sizeof( addrinfo_type) );
	info_hints.ai_family = PF_INET6;
	if ( hints)
	{
		info_hints.ai_flags = hints->flags();
		info_hints.ai_socktype= hints->socket();
		info_hints.ai_protocol = hints->protocol();
	}
	info_hints.ai_flags |= AI_PASSIVE;

	system::error_code ec;
	detail::getaddrinfo(
		0,
		service.c_str(),
		& info_hints,
		& tmp,
		ec);
	if ( ec) throw_exception( system::system_error( ec) );

	shared_ptr< addrinfo_type > info_ptr( tmp, detail::addrinfo_deleter() );

	std::vector< address_info > infos;

	if ( ! info_ptr) return infos;
	for ( addrinfo_type * ptr( info_ptr.get() ); ptr; ptr = ptr->ai_next)
		infos.push_back( address_info( info_ptr, ptr) );
	return infos;
}
}}}

#endif // BOOST_NET_IPV6_RESOLVE_H
