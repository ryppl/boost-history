//  Copyright (c) 2001-2008 Oliver Kowalke. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_NET_IPV4_IPNUMBER_H
#define BOOST_NET_IPV4_IPNUMBER_H

#include <boost/net/detail/config.hpp>

extern "C"
{
#include <netinet/in.h>
#include <strings.h>
}

#include <iostream>
#include <stdexcept>
#include <string>

#include <boost/assert.hpp>
#include <boost/system/error_code.hpp>
#include <boost/system/system_error.hpp>

#include <boost/net/ipv4/detail/operations.hpp>
#include <boost/net/types.hpp>

namespace boost { namespace net {
namespace ipv4
{
class ipnumber
{
private:
	friend bool operator==( ipnumber const&, ipnumber const&);
	friend bool operator<( ipnumber const&, ipnumber const&);

	in4_addr_type	addr_;

	u_long_type to_long_() const
	{ return detail::network2host_long( static_cast< u_long_type >( addr_.s_addr) ); }

public:
	ipnumber( in4_addr_type const& addr)
	: addr_( addr)
	{}

	static const ipnumber invalid()
	{
		in4_addr_type addr;
		::bzero( & addr, sizeof( in4_addr_type) );
		addr.s_addr = detail::host2network_long( static_cast< u_long_type >( INADDR_NONE) );
		return addr;
	}

	static const ipnumber any()
	{
		in4_addr_type addr;
		::bzero( & addr, sizeof( in4_addr_type) );
		addr.s_addr = detail::host2network_long( static_cast< u_long_type >( INADDR_ANY) );
		return addr;
	}

	static const ipnumber bcast()
	{
		in4_addr_type addr;
		::bzero( & addr, sizeof( in4_addr_type) );
		addr.s_addr = detail::host2network_long( static_cast< u_long_type >( INADDR_BROADCAST) );
		return addr;
	}

	static const ipnumber loopback()
	{
		in4_addr_type addr;
		::bzero( & addr, sizeof( in4_addr_type) );
		addr.s_addr = detail::host2network_long( static_cast< u_long_type >( INADDR_LOOPBACK) );
		return addr;
	}

	bool is_class_a() const
	{ return IN_CLASSA( to_long_() ); }

	bool is_class_b() const
	{ return IN_CLASSB( to_long_() ); }

	bool is_class_c() const
	{ return IN_CLASSC( to_long_() ); }

	bool is_class_d() const
	{ return IN_CLASSD( to_long_() ); }

	bool is_multicast() const
	{ return IN_MULTICAST( to_long_() ); }

	const in4_addr_type in4_addr() const
	{ return addr_; }
};

inline
const std::string to_string( ipnumber const& number)
{
	system::error_code ec;
	std::string str( detail::inet_ntop( number.in4_addr(), ec) );
	if ( ec) throw_exception( system::system_error( ec) );
	return str;
}

inline
bool operator<( ipnumber const& l, ipnumber const& r)
{ return l.to_long_() < r.to_long_(); }

inline
bool operator==( ipnumber const& l, ipnumber const& r)
{ return l.addr_.s_addr == r.addr_.s_addr; }

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

#endif // BOOST_NET_IPV4_IPNUMBER_H
