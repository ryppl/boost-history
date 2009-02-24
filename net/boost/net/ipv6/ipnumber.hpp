//  Copyright (c) 2001-2008 Oliver Kowalke. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_NET_IPV6_IPNUMBER_V6_H
#define BOOST_NET_IPV6_IPNUMBER_V6_H

#include <boost/net/detail/config.hpp>

extern "C"
{
#include <netinet/in.h>
#include <strings.h>
}

#include <cstring>
#include <iostream>
#include <stdexcept>
#include <string>

#include <boost/array.hpp>
#include <boost/assert.hpp>
#include <boost/system/error_code.hpp>
#include <boost/system/system_error.hpp>

#include <boost/net/ipv4/ipnumber.hpp>
#include <boost/net/ipv6/detail/operations.hpp>
#include <boost/net/ipv6/ipnumber.hpp>
#include <boost/net/types.hpp>

namespace boost { namespace net {
namespace ipv6
{
class ipnumber
{
private:
	friend bool operator<( ipnumber const&, ipnumber const&);
	friend bool operator==( ipnumber const&, ipnumber const&);

	in6_addr_type	addr_;

public:
	ipnumber( in6_addr_type const& addr)
	: addr_( addr)
	{}

	static const ipnumber any()
	{
		in6_addr_type addr;
		::bzero( & addr, sizeof( in6_addr_type) );
		addr = in6addr_any;
		return addr;
	}

	static const ipnumber loopback()
	{
		in6_addr_type addr;
		::bzero( & addr, sizeof( in6_addr_type) );
		addr = in6addr_loopback;
		return addr;
	}

	static const ipnumber v4_mapped( ipv4::ipnumber const& number)
	{
		array< unsigned char, 4 > buffer;
		std::memcpy( buffer.elems, & number.in4_addr().s_addr, 4);
		in6_addr_type addr;
		::bzero( & addr, sizeof( in6_addr_type) );
		addr.s6_addr[10] = 0xFF;
		addr.s6_addr[11] = 0xFF;
		addr.s6_addr[12] = buffer[0];
		addr.s6_addr[13] = buffer[1];
		addr.s6_addr[14] = buffer[2];
		addr.s6_addr[15] = buffer[3];
		return addr;
	}

	static const ipnumber v4_compatible( ipv4::ipnumber const& number)
	{
		array< unsigned char, 4 > buffer;
		std::memcpy( buffer.elems, & number.in4_addr().s_addr, 4);
		in6_addr_type addr;
		::bzero( & addr, sizeof( in6_addr_type) );
		addr.s6_addr[12] = buffer[0];
		addr.s6_addr[13] = buffer[1];
		addr.s6_addr[14] = buffer[2];
		addr.s6_addr[15] = buffer[3];
		return addr;
	}

	bool is_unspec() const
	{ return IN6_IS_ADDR_UNSPECIFIED( & addr_) != 0; }

	bool is_loopback() const
	{ return IN6_IS_ADDR_LOOPBACK( & addr_) != 0; }

	bool is_multicast() const
	{ return IN6_IS_ADDR_MULTICAST( & addr_) != 0; }

	bool is_link_local() const
	{ return IN6_IS_ADDR_LINKLOCAL( & addr_) != 0; }

	bool is_site_local() const
	{ return IN6_IS_ADDR_SITELOCAL( & addr_) != 0; }

	bool is_v4_mapped() const
	{ return IN6_IS_ADDR_V4MAPPED( & addr_) != 0; }

	bool is_v4_compatible() const
	{ return IN6_IS_ADDR_V4COMPAT( & addr_) != 0; }

	bool is_mc_node_local() const
	{ return IN6_IS_ADDR_MC_NODELOCAL( & addr_) != 0; }

	bool is_mc_link_local() const
	{ return IN6_IS_ADDR_MC_LINKLOCAL( & addr_) != 0; }

	bool is_mc_size_local() const
	{ return IN6_IS_ADDR_MC_SITELOCAL( & addr_) != 0; }

	bool is_mc_org_local() const
	{ return IN6_IS_ADDR_MC_ORGLOCAL( & addr_) != 0; }

	bool is_mc_global() const
	{ return IN6_IS_ADDR_MC_GLOBAL( & addr_) != 0; }

	const ipv4::ipnumber to_v4() const
	{
		if ( ! is_v4_mapped() && ! is_v4_compatible() )
			throw_exception( std::bad_cast() );
		array< unsigned char, 4 > buffer;
		buffer[0] = addr_.s6_addr[12];
		buffer[1] = addr_.s6_addr[13];
		buffer[2] = addr_.s6_addr[14];
		buffer[3] = addr_.s6_addr[15];
		in4_addr_type addr;
		std::memcpy( & addr.s_addr, buffer.elems, 4);
		return ipv4::ipnumber( addr);
	}

	const in6_addr_type in6_addr() const
	{ return addr_; }
};

const std::string to_string( ipnumber const& number)
{
	system::error_code ec;
	std::string str( detail::inet_ntop( number.in6_addr(), ec) );
	if ( ec) throw_exception( system::system_error( ec) );
	return str;
}

inline
bool operator<( ipnumber const& l, ipnumber const& r)
{ return ::memcmp( & l.addr_, & r.addr_, sizeof( in6_addr_type) ) < 0; }

inline
bool operator==( ipnumber const& l, ipnumber const& r)
{ return ::memcmp( & l.addr_, & r.addr_, sizeof( in6_addr_type) ) == 0; }

inline
bool operator!=( ipnumber const& l, ipnumber const& r)
{ return ! ( l == r); }

template<
	typename charT,
	typename Traits
>
std::basic_ostream< charT, Traits > &
operator<<( std::basic_ostream< charT, Traits > & os, ipnumber const& number)
{
	if ( ! os.good() ) return os;
	try
	{ os << to_string( number); }
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
}}}

#endif // BOOST_NET_IPV6_IPNUMBER_V6_H
