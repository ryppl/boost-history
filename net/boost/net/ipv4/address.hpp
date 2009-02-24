//  Copyright (c) 2001-2008 Oliver Kowalke. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_NET_IPV4_ADDRESS_H
#define BOOST_NET_IPV4_ADDRESS_H

#include <boost/net/detail/config.hpp>

extern "C"
{
#include <netinet/in.h>
#include <strings.h>
}

#include <cstddef>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#include <boost/array.hpp>
#include <boost/system/error_code.hpp>
#include <boost/system/system_error.hpp>
#include <boost/utility.hpp>

#include <boost/net/ipv4/detail/operations.hpp>
#include <boost/net/ipv4/ipnumber.hpp>
#include <boost/net/ipv4/utility.hpp>
#include <boost/net/types.hpp>

namespace boost { namespace net {
namespace ipv4
{
class address
{
private:
	sockaddr_in4_type	addr_;

	address()
	: addr_()
	{
		::bzero( & addr_, sizeof( sockaddr_in4_type) );
		addr_.sin_family = family_ipv4;
	}

public:
	address( sockaddr_in4_type const& addr)
	: addr_()
	{
		::bzero( & addr_, sizeof( sockaddr_in4_type) );
		addr_ = addr;
	}

	address( ipnumber const& ip, unsigned int port)
	: addr_()
	{
		::bzero( & addr_, sizeof( sockaddr_in4_type) );
		addr_.sin_family = family_ipv4;
		addr_.sin_addr = ip.in4_addr();
		addr_.sin_port = detail::host2network_short( port);
#ifdef BOOST_NET_SIN4_LEN
		addr_.sin_len = sizeof( sockaddr_in4_type);
#endif
	}

	static const address invalid()
	{
		address addr;
		addr.addr_.sin_addr.s_addr = detail::host2network_long(
			static_cast< u_long_type >( INADDR_NONE) );
#ifdef BOOST_NET_SIN4_LEN
		addr.addr_.sin_len = sizeof( sockaddr_in4_type);
#endif
		return addr;
	}

	static const address any( unsigned int port)
	{
		address addr;
		addr.addr_.sin_addr.s_addr =
			detail::host2network_long( static_cast< u_long_type >( INADDR_ANY) );
		addr.addr_.sin_port = detail::host2network_short( port);
#ifdef BOOST_NET_SIN4_LEN
		addr.addr_.sin_len = sizeof( sockaddr_in4_type);
#endif
		return addr;
	}

	static const address bcast( unsigned int port)
	{
		address addr;
		addr.addr_.sin_addr.s_addr =
			detail::host2network_long( static_cast< u_long_type >( INADDR_BROADCAST) );
		addr.addr_.sin_port = detail::host2network_short( port);
#ifdef BOOST_NET_SIN4_LEN
		addr.addr_.sin_len = sizeof( sockaddr_in4_type);
#endif
		return addr;
	}

	static const address loopback( unsigned int port)
	{
		address addr;
		addr.addr_.sin_addr.s_addr =
			detail::host2network_long( static_cast< u_long_type >( INADDR_LOOPBACK) );
		addr.addr_.sin_port = detail::host2network_short( port);
#ifdef BOOST_NET_SIN4_LEN
		addr.addr_.sin_len = sizeof( sockaddr_in4_type);
#endif
		return addr;
	}

	const ipnumber ip_number() const
	{ return ipnumber( addr_.sin_addr); }

	unsigned int port() const
	{ return detail::network2host_short( addr_.sin_port); }

	sockaddr_type const* sockaddr_ptr() const
	{
		void const* vp( & addr_);
		return static_cast< sockaddr_type const* >( vp);
	}

	sockaddr_len_type size() const
	{ return sizeof( sockaddr_in4_type); }
};

inline
bool operator<( address const& l, address const& r)
{
	if ( l.ip_number() < r.ip_number() )
		return true;
	if ( l.ip_number() != r.ip_number() )
		return false;
	return l.port() < r.port();
}

inline
bool operator==( address const& l, address const& r)
{ return l.ip_number() == r.ip_number() && l.port() == r.port(); }

inline
bool operator!=( address const& l, address const& r)
{ return ! ( l == r); }

template<
	typename charT,
	typename Traits
>
std::basic_ostream< charT, Traits > &
operator<<( std::basic_ostream< charT, Traits > & os, address const& addr)
{
	if ( ! os.good() ) return os;
	try
	{ os << addr.ip_number() << ":" << addr.port(); }
	catch ( std::bad_alloc &)
	{
		std::ios_base::iostate exception_mask( os.exceptions() );
		if ( ( exception_mask & std::ios_base::failbit) && ! ( exception_mask & std::ios_base::badbit) )
			os.setstate( std::ios_base::badbit);
		else if ( exception_mask & std::ios_base::badbit)
		{
			try
			{ os.setstate( std::ios_base::badbit); }
			catch ( std::ios_base::failure &)
			{}
			throw;
		}
	}
	catch (...)
	{
		std::ios_base::iostate exception_mask( os.exceptions() );
		if ( exception_mask & std::ios_base::failbit)
		{
			try
			{ os.setstate( std::ios_base::failbit); }
			catch ( std::ios_base::failure &)
			{}
			throw;
		}
	}
	return os;
}


inline
const std::string fqd_name( address const& addr)
{
	boost::array< char, fqdn_size > array;
	::bzero( array.c_array(), array.size() );
	system::error_code ec;
	detail::getnameinfo(
		addr.sockaddr_ptr(),
		addr.size(),
		array.c_array(),
		array.size(),
		0,
		0,
		NI_NAMEREQD,
		ec);
	if ( ec) throw_exception( system::system_error( ec) );
	return std::string( array.data(), array.size() );
}

inline
const std::vector< std::string > aliases( address const& addr)
{
	std::vector< std::string > array;
	std::vector< char > buffer( 8192);

	hostent_type result;

	void const* vp( addr.sockaddr_ptr() );
	system::error_code ec;
	sockaddr_in4_type const* in4( static_cast< sockaddr_in4_type const* >( vp) );
	detail::gethostbyaddr_r(
		* const_cast< sockaddr_in4_type * >( in4),
		result,
		& buffer[0],
		buffer.size(),
		ec);
	if ( ec) throw_exception( system::system_error( ec) );

	for ( char ** pptr = result.h_aliases; * pptr != 0; ++pptr)
		array.push_back( * pptr);

	return array;
}
}}}

#endif // BOOST_NET_IPV4_ADDRESS_H
