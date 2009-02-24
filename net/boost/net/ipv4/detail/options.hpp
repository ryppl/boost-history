//  Copyright (c) 2001-2008 Oliver Kowalke. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_NET_IPV4_DETAIL_OPTIONS_H
#define BOOST_NET_IPV4_DETAIL_OPTIONS_H

#include <boost/net/detail/config.hpp>

extern "C"
{
#include <netinet/in.h>
}

#include <cstddef>
#include <cstring>

#include <boost/assert.hpp>
#include <boost/system/error_code.hpp>
#include <boost/system/system_error.hpp>
#include <boost/utility.hpp>

#include <boost/net/detail/operations.hpp>
#include <boost/net/detail/socket.hpp>
#include <boost/net/ipv4/address.hpp>
#include <boost/net/ipv4/ipnumber.hpp>
#include <boost/net/types.hpp>

namespace boost { namespace net {
namespace ipv4 { namespace detail
{
class options
{
private:
	net::detail::socket	socket_;

public:
	options( net::detail::socket const& socket__)
	: socket_( socket__)
	{}

	bool include_header() const
	{
		BOOST_ASSERT( socket_.is_valid() );

		std::size_t value( 0);
		sockaddr_len_type len( sizeof( std::size_t) );
		system::error_code ec;
		net::detail::getsockopt(
			socket_.native_handle(),
			IPPROTO_IP,
			IP_HDRINCL,
			& value,
			& len,
			ec);
		if ( ec) throw_exception( system::system_error( ec) );
		return value != 0;
	}

	void include_header( bool value_)
	{
		BOOST_ASSERT( socket_.is_valid() );

		int value( value_ ? 1 : 0);
		system::error_code ec;
		net::detail::setsockopt(
			socket_.native_handle(),
			IPPROTO_IP,
			IP_HDRINCL,
			& value,
			sizeof( int),
			ec);
		if ( ec) throw_exception( system::system_error( ec) );
	}

	std::size_t ip_options() const
	{
		BOOST_ASSERT( socket_.is_valid() );

		std::size_t value( 0);
		sockaddr_len_type len( sizeof( std::size_t) );
		system::error_code ec;
		net::detail::getsockopt(
			socket_.native_handle(),
			IPPROTO_IP,
			IP_OPTIONS,
			& value,
			& len,
			ec);
		if ( ec) throw_exception( system::system_error( ec) );
		return value;
	}

	void ip_options( std::size_t value)
	{
		BOOST_ASSERT( socket_.is_valid() );
		BOOST_ASSERT( value > 0);

		system::error_code ec;
		net::detail::setsockopt(
			socket_.native_handle(),
			IPPROTO_IP,
			IP_OPTIONS,
			& value,
			sizeof( std::size_t),
			ec);
		if ( ec) throw_exception( system::system_error( ec) );
	}

	std::size_t time_to_live() const
	{
		BOOST_ASSERT( socket_.is_valid() );

		std::size_t value( 0);
		sockaddr_len_type len( sizeof( std::size_t) );
		system::error_code ec;
		net::detail::getsockopt(
			socket_.native_handle(),
			IPPROTO_IP,
			IP_TTL,
			& value,
			& len,
			ec);
		if ( ec) throw_exception( system::system_error( ec) );
		return value;
	}

	void time_to_live( std::size_t value)
	{
		BOOST_ASSERT( socket_.is_valid() );
		BOOST_ASSERT( value > 0);

		system::error_code ec;
		net::detail::setsockopt(
			socket_.native_handle(),
			IPPROTO_IP,
			IP_TTL,
			& value,
			sizeof( std::size_t),
			ec);
		if ( ec) throw_exception( system::system_error( ec) );
	}

	std::size_t type_of_service() const
	{
		BOOST_ASSERT( socket_.is_valid() );

		std::size_t value( 0);
		sockaddr_len_type len( sizeof( std::size_t) );
		system::error_code ec;
		net::detail::getsockopt(
			socket_.native_handle(),
			IPPROTO_IP,
			IP_TOS,
			& value,
			& len,
			ec);
		if ( ec) throw_exception( system::system_error( ec) );
		return value;
	}

	void type_of_service( std::size_t value)
	{
		BOOST_ASSERT( socket_.is_valid() );
		BOOST_ASSERT( value > 0);

		system::error_code ec;
		net::detail::setsockopt(
			socket_.native_handle(),
			IPPROTO_IP,
			IP_TOS,
			& value,
			sizeof( std::size_t),
			ec);
		if ( ec) throw_exception( system::system_error( ec) );
	}

	int mc_time_to_life() const
	{
		BOOST_ASSERT( socket_.is_valid() );

		u_char_type value( 0);
		sockaddr_len_type len( sizeof( u_char_type) );
		system::error_code ec;
		net::detail::getsockopt(
			socket_.native_handle(),
			IPPROTO_IP,
			IP_MULTICAST_TTL,
			& value,
			& len,
			ec);
		if ( ec) throw_exception( system::system_error( ec) );
		return static_cast< int >( value);
	}

	void mc_time_to_life( int value_)
	{
		BOOST_ASSERT( socket_.is_valid() );

		u_char_type value( static_cast< u_char_type >( value_) );
		system::error_code ec;
		net::detail::setsockopt(
			socket_.native_handle(),
			IPPROTO_IP,
			IP_MULTICAST_TTL,
			& value,
			sizeof( int),
			ec);
		if ( ec) throw_exception( system::system_error( ec) );
	}

	bool mc_is_loopback() const
	{
		BOOST_ASSERT( socket_.is_valid() );

		u_char_type value( 0);
		sockaddr_len_type len( sizeof( u_char_type) );
		system::error_code ec;
		net::detail::getsockopt(
			socket_.native_handle(),
			IPPROTO_IP,
			IP_MULTICAST_LOOP,
			& value,
			& len,
			ec);
		if ( ec) throw_exception( system::system_error( ec) );
		return value != 0;
	}

	void mc_loopback( bool value_)
	{
		BOOST_ASSERT( socket_.is_valid() );

		u_char_type value( value_ ? 1 : 0);
		system::error_code ec;
		net::detail::setsockopt(
			socket_.native_handle(),
			IPPROTO_IP,
			IP_MULTICAST_LOOP,
			& value,
			sizeof( int),
			ec);
		if ( ec) throw_exception( system::system_error( ec) );
	}

	const ipnumber mc_interface() const
	{
		BOOST_ASSERT( socket_.is_valid() );

		in4_addr_type in4;
		sockaddr_len_type len( sizeof( in4_addr_type) );
		::bzero( & in4, sizeof( in4_addr_type) );
		system::error_code ec;
		net::detail::getsockopt(
			socket_.native_handle(),
			IPPROTO_IP,
			IP_MULTICAST_IF,
			& in4,
			& len,
			ec);
		if ( ec) throw_exception( system::system_error( ec) );
		return ipnumber( in4);
	}

	void mc_interface( ipnumber const& number)
	{
		BOOST_ASSERT( socket_.is_valid() );

		in4_addr_type in4( number.in4_addr() );
		system::error_code ec;
		net::detail::setsockopt(
			socket_.native_handle(),
			IPPROTO_IP,
			IP_MULTICAST_IF,
			& in4,
			sizeof( in4_addr_type),
			ec);
		if ( ec) throw_exception( system::system_error( ec) );
	}

	void mc_join_group( address const& addr, u_long_type ifindex = 0)
	{
		BOOST_ASSERT( socket_.is_valid() );

		group_req_type req;
		req.gr_interface = ifindex;
		if ( addr.size() > sizeof( req.gr_group) )
			throw_exception(
				system::system_error(
					system::error_code(
						ENXIO,
						system::get_system_category() ) ) );
		std::memcpy( & req.gr_group, addr.sockaddr_ptr(), addr.size() );
		system::error_code ec;
		net::detail::setsockopt(
			socket_.native_handle(),
			IPPROTO_IP,
			MCAST_JOIN_GROUP,
			& req,
			sizeof( group_req_type),
			ec);
		if ( ec) throw_exception( system::system_error( ec) );
	}

	void mc_join_group( address const& addr, std::string const& ifname)
	{
		BOOST_ASSERT( socket_.is_valid() );
		BOOST_ASSERT( ! ifname.empty() );

		group_req_type req;
		system::error_code ec;
		req.gr_interface = net::detail::if_nametoindex( ifname, ec);
		if ( ec) throw_exception( system::system_error( ec) );
		if ( addr.size() > sizeof( req.gr_group) )
			throw_exception(
				system::system_error(
					system::error_code(
						ENXIO,
						system::get_system_category() ) ) );
		std::memcpy( & req.gr_group, addr.sockaddr_ptr(), addr.size() );
		net::detail::setsockopt(
			socket_.native_handle(),
			IPPROTO_IP,
			MCAST_JOIN_GROUP,
			& req,
			sizeof( group_req_type),
			ec);
		if ( ec) throw_exception( system::system_error( ec) );
	}

	void mc_leave_group( address const& addr, u_long_type ifindex = 0)
	{
		BOOST_ASSERT( socket_.is_valid() );

		group_req_type req;
		req.gr_interface = ifindex;
		if ( addr.size() > sizeof( req.gr_group) )
			throw_exception(
				system::system_error(
					system::error_code(
						ENXIO,
						system::get_system_category() ) ) );
		std::memcpy( & req.gr_group, addr.sockaddr_ptr(), addr.size() );
		system::error_code ec;
		net::detail::setsockopt(
			socket_.native_handle(),
			IPPROTO_IP,
			MCAST_LEAVE_GROUP,
			& req,
			sizeof( group_req_type),
			ec);
		if ( ec) throw_exception( system::system_error( ec) );
	}

	void mc_leave_group( address const& addr, std::string const& ifname)
	{
		BOOST_ASSERT( socket_.is_valid() );
		BOOST_ASSERT( ! ifname.empty() );

		group_req_type req;
		system::error_code ec;
		req.gr_interface = net::detail::if_nametoindex( ifname, ec);
		if ( ec) throw_exception( system::system_error( ec) );
		if ( addr.size() > sizeof( req.gr_group) )
			throw_exception(
				system::system_error(
					system::error_code(
						ENXIO,
						system::get_system_category() ) ) );
		std::memcpy( & req.gr_group, addr.sockaddr_ptr(), addr.size() );
		net::detail::setsockopt(
			socket_.native_handle(),
			IPPROTO_IP,
			MCAST_LEAVE_GROUP,
			& req,
			sizeof( group_req_type),
			ec);
		if ( ec) throw_exception( system::system_error( ec) );
	}

	void mc_join_source_group(
		address const& group_addr,
		address const& source_addr,
		u_long_type ifindex = 0)
	{
		BOOST_ASSERT( socket_.is_valid() );

		group_source_req_type req;
		req.gsr_interface = ifindex;
		if ( group_addr.size() > sizeof( req.gsr_group) )
			throw_exception(
				system::system_error(
					system::error_code(
						ENXIO,
						system::get_system_category() ) ) );
		if ( source_addr.size() > sizeof( req.gsr_source) )
			throw_exception(
				system::system_error(
					system::error_code(
						ENXIO,
						system::get_system_category() ) ) );
		std::memcpy( & req.gsr_group, group_addr.sockaddr_ptr(), group_addr.size() );
		std::memcpy( & req.gsr_source, source_addr.sockaddr_ptr(), source_addr.size() );
		system::error_code ec;
		net::detail::setsockopt(
			socket_.native_handle(),
			IPPROTO_IP,
			MCAST_JOIN_SOURCE_GROUP,
			& req,
			sizeof( group_source_req_type),
			ec);
		if ( ec) throw_exception( system::system_error( ec) );
	}

	void mc_join_source_group(
		address const& group_addr,
		address const& source_addr,
		std::string const& ifname)
	{
		BOOST_ASSERT( socket_.is_valid() );
		BOOST_ASSERT( ! ifname.empty() );

		group_source_req_type req;
		system::error_code ec;
		req.gsr_interface = net::detail::if_nametoindex( ifname, ec);
		if ( ec) throw_exception( system::system_error( ec) );
		if ( group_addr.size() > sizeof( req.gsr_group) )
			throw_exception(
				system::system_error(
					system::error_code(
						ENXIO,
						system::get_system_category() ) ) );
		if ( source_addr.size() > sizeof( req.gsr_source) )
			throw_exception(
				system::system_error(
					system::error_code(
						ENXIO,
						system::get_system_category() ) ) );
		std::memcpy( & req.gsr_group, group_addr.sockaddr_ptr(), group_addr.size() );
		std::memcpy( & req.gsr_source, source_addr.sockaddr_ptr(), source_addr.size() );
		net::detail::setsockopt(
			socket_.native_handle(),
			IPPROTO_IP,
			MCAST_JOIN_SOURCE_GROUP,
			& req,
			sizeof( group_req_type),
			ec);
		if ( ec) throw_exception( system::system_error( ec) );
	}

	void mc_leave_source_group(
		address const& group_addr,
		address const& source_addr,
		u_long_type ifindex = 0)
	{
		BOOST_ASSERT( socket_.is_valid() );

		group_source_req_type req;
		req.gsr_interface = ifindex;
		if ( group_addr.size() > sizeof( req.gsr_group) )
			throw_exception(
				system::system_error(
					system::error_code(
						ENXIO,
						system::get_system_category() ) ) );
		if ( source_addr.size() > sizeof( req.gsr_source) )
			throw_exception(
				system::system_error(
					system::error_code(
						ENXIO,
						system::get_system_category() ) ) );
		std::memcpy( & req.gsr_group, group_addr.sockaddr_ptr(), group_addr.size() );
		std::memcpy( & req.gsr_source, source_addr.sockaddr_ptr(), source_addr.size() );
		system::error_code ec;
		net::detail::setsockopt(
			socket_.native_handle(),
			IPPROTO_IP,
			MCAST_LEAVE_SOURCE_GROUP,
			& req,
			sizeof( group_source_req_type),
			ec);
		if ( ec) throw_exception( system::system_error( ec) );
	}

	void mc_leave_source_group(
		address const& group_addr,
		address const& source_addr,
		std::string const& ifname)
	{
		BOOST_ASSERT( socket_.is_valid() );
		BOOST_ASSERT( ! ifname.empty() );

		group_source_req_type req;
		system::error_code ec;
		req.gsr_interface = net::detail::if_nametoindex( ifname, ec);
		if ( ec) throw_exception( system::system_error( ec) );
		if ( group_addr.size() > sizeof( req.gsr_group) )
			throw_exception(
				system::system_error(
					system::error_code(
						ENXIO,
						system::get_system_category() ) ) );
		if ( source_addr.size() > sizeof( req.gsr_source) )
			throw_exception(
				system::system_error(
					system::error_code(
						ENXIO,
						system::get_system_category() ) ) );
		std::memcpy( & req.gsr_group, group_addr.sockaddr_ptr(), group_addr.size() );
		std::memcpy( & req.gsr_source, source_addr.sockaddr_ptr(), source_addr.size() );
		net::detail::setsockopt(
			socket_.native_handle(),
			IPPROTO_IP,
			MCAST_LEAVE_SOURCE_GROUP,
			& req,
			sizeof( group_req_type),
			ec);
		if ( ec) throw_exception( system::system_error( ec) );
	}

	void mc_block_source(
		address const& group_addr,
		address const& source_addr,
		u_long_type ifindex = 0)
	{
		BOOST_ASSERT( socket_.is_valid() );

		group_source_req_type req;
		req.gsr_interface = ifindex;
		if ( group_addr.size() > sizeof( req.gsr_group) )
			throw_exception(
				system::system_error(
					system::error_code(
						ENXIO,
						system::get_system_category() ) ) );
		if ( source_addr.size() > sizeof( req.gsr_source) )
			throw_exception(
				system::system_error(
					system::error_code(
						ENXIO,
						system::get_system_category() ) ) );
		std::memcpy( & req.gsr_group, group_addr.sockaddr_ptr(), group_addr.size() );
		std::memcpy( & req.gsr_source, source_addr.sockaddr_ptr(), source_addr.size() );
		system::error_code ec;
		net::detail::setsockopt(
			socket_.native_handle(),
			IPPROTO_IP,
			MCAST_BLOCK_SOURCE,
			& req,
			sizeof( group_source_req_type),
			ec);
		if ( ec) throw_exception( system::system_error( ec) );
	}

	void mc_block_source(
		address const& group_addr,
		address const& source_addr,
		std::string const& ifname)
	{
		BOOST_ASSERT( socket_.is_valid() );
		BOOST_ASSERT( ! ifname.empty() );

		group_source_req_type req;
		system::error_code ec;
		req.gsr_interface = net::detail::if_nametoindex( ifname, ec);
		if ( group_addr.size() > sizeof( req.gsr_group) )
			throw_exception(
				system::system_error(
					system::error_code(
						ENXIO,
						system::get_system_category() ) ) );
		if ( source_addr.size() > sizeof( req.gsr_source) )
			throw_exception(
				system::system_error(
					system::error_code(
						ENXIO,
						system::get_system_category() ) ) );
		std::memcpy( & req.gsr_group, group_addr.sockaddr_ptr(), group_addr.size() );
		std::memcpy( & req.gsr_source, source_addr.sockaddr_ptr(), source_addr.size() );
		net::detail::setsockopt(
			socket_.native_handle(),
			IPPROTO_IP,
			MCAST_BLOCK_SOURCE,
			& req,
			sizeof( group_source_req_type),
			ec);
		if ( ec) throw_exception( system::system_error( ec) );
	}

	void mc_unblock_source(
		address const& group_addr,
		address const& source_addr,
		u_long_type ifindex = 0)
	{
		BOOST_ASSERT( socket_.is_valid() );

		group_source_req_type req;
		req.gsr_interface = ifindex;
		if ( group_addr.size() > sizeof( req.gsr_group) )
			throw_exception(
				system::system_error(
					system::error_code(
						ENXIO,
						system::get_system_category() ) ) );
		if ( source_addr.size() > sizeof( req.gsr_source) )
			throw_exception(
				system::system_error(
					system::error_code(
						ENXIO,
						system::get_system_category() ) ) );
		std::memcpy( & req.gsr_group, group_addr.sockaddr_ptr(), group_addr.size() );
		std::memcpy( & req.gsr_source, source_addr.sockaddr_ptr(), source_addr.size() );
		system::error_code ec;
		net::detail::setsockopt(
			socket_.native_handle(),
			IPPROTO_IP,
			MCAST_UNBLOCK_SOURCE,
			& req,
			sizeof( group_source_req_type),
			ec);
		if ( ec) throw_exception( system::system_error( ec) );
	}

	void mc_unblock_source(
		address const& group_addr,
		address const& source_addr,
		std::string const& ifname)
	{
		BOOST_ASSERT( socket_.is_valid() );
		BOOST_ASSERT( ! ifname.empty() );

		group_source_req_type req;
		system::error_code ec;
		req.gsr_interface = net::detail::if_nametoindex( ifname, ec);
		if ( group_addr.size() > sizeof( req.gsr_group) )
			throw_exception(
				system::system_error(
					system::error_code(
						ENXIO,
						system::get_system_category() ) ) );
		if ( source_addr.size() > sizeof( req.gsr_source) )
			throw_exception(
				system::system_error(
					system::error_code(
						ENXIO,
						system::get_system_category() ) ) );
		std::memcpy( & req.gsr_group, group_addr.sockaddr_ptr(), group_addr.size() );
		std::memcpy( & req.gsr_source, source_addr.sockaddr_ptr(), source_addr.size() );
		net::detail::setsockopt(
			socket_.native_handle(),
			IPPROTO_IP,
			MCAST_UNBLOCK_SOURCE,
			& req,
			sizeof( group_source_req_type),
			ec);
		if ( ec) throw_exception( system::system_error( ec) );
	}
};
}}}}

#endif // BOOST_NET_IPV4_DETAIL_OPTIONS_H
