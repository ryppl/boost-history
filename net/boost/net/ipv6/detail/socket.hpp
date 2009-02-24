//  Copyright (c) 2001-2008 Oliver Kowalke. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_NET_IPV6_DETAIL_SOCKET_H
#define BOOST_NET_IPV6_DETAIL_SOCKET_H

#include <boost/assert.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/system/error_code.hpp>
#include <boost/system/system_error.hpp>
#include <boost/utility.hpp>

#include <boost/net/constants.hpp>
#include <boost/net/detail/operations.hpp>
#include <boost/net/shutdown_mode.hpp>
#include <boost/net/ipv4/utility.hpp>
#include <boost/net/types.hpp>

namespace boost { namespace net {
namespace ipv6 { namespace detail
{
class socket
{
private:
	class impl
	{
	private:
		native_handle_type	handle_;

	public:
		impl( native_handle_type handle)
		: handle_( handle)
		{ BOOST_ASSERT( handle_ != invalid_native_handle); }

		impl(
			socket_type socktype,
			protocol_type protocol)
		: handle_( invalid_native_handle)
		{
			system::error_code ec;
			handle_ = net::detail::create_socket( PF_INET6, socktype, protocol, ec);
			if ( ec) throw_exception( system::system_error( ec) );
			BOOST_ASSERT( handle_ != invalid_native_handle);
		}

		~impl()
		{
			system::error_code ec;
			net::detail::close_socket( handle_, ec);
			handle_ = invalid_native_handle;
		}

		native_handle_type native_handle() const
		{ return handle_; }

		bool is_valid() const
		{ return handle_ != invalid_native_handle; }

		void close()
		{
			if ( is_valid() )
			{
				system::error_code ec;
				net::detail::close_socket( handle_, ec);
				handle_ = invalid_native_handle;
				if ( ec) throw_exception( system::system_error( ec) );
			}
		}

		void shutdown( shutdown_mode const& mode)
		{
			BOOST_ASSERT( is_valid() );
			system::error_code ec;
			net::detail::shutdown( handle_, mode, ec);
			if ( ec) throw_exception( system::system_error( ec) );
		}

		const system::error_code error() const
		{
			BOOST_ASSERT( is_valid() );
			int value( 0);
			system::error_code ec;
			net::detail::getsockopt( handle_, SOL_SOCKET, SO_ERROR, ( void *) & value, sizeof( int), ec);
			if ( ec) throw_exception( system::system_error( ec) );
			return system::error_code( value, system::get_system_category() );
		}

		bool is_non_blocking() const
		{
			BOOST_ASSERT( is_valid() );
			system::error_code ec;
			bool result( net::detail::non_blocking( handle_, ec) );
			if ( ec) throw_exception( system::system_error( ec) );
			return result;
		}

		void non_blocking( bool mode)
		{
			BOOST_ASSERT( is_valid() );
			system::error_code ec;
			net::detail::non_blocking( handle_, mode, ec);
			if ( ec) throw_exception( system::system_error( ec) );
		}
	};

	shared_ptr< impl >	impl_;

public:
	socket(
		socket_type socktype,
		protocol_type protocol)
	: impl_( new impl( socktype, protocol) )
	{ BOOST_ASSERT( impl_->is_valid() ); }

	socket( native_handle_type handle)
	: impl_( new impl( handle) )
	{ BOOST_ASSERT( impl_->is_valid() ); }

	native_handle_type native_handle() const
	{ return impl_->native_handle(); }

	bool is_valid() const
	{ return impl_->is_valid(); }

	void close()
	{ impl_->close(); }

	void shutdown( shutdown_mode const& mode)
	{ impl_->shutdown( mode); }

	const system::error_code error() const
	{ return impl_->error(); }

	bool is_non_blocking() const
	{ return impl_->is_non_blocking(); }

	void non_blocking( bool value)
	{ impl_->non_blocking( value); }
};
}}}}

#endif // BOOST_NET_IPV6_DETAIL_SOCKET_H
