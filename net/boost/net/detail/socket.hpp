//  Copyright (c) 2001-2008 Oliver Kowalke. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_NET_DETAIL_SOCKET_H
#define BOOST_NET_DETAIL_SOCKET_H

#include <boost/net/detail/config.hpp>

extern "C"
{
#include <sys/socket.h>
}

#include <boost/config.hpp>

#include <boost/assert.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/system/error_code.hpp>
#include <boost/system/system_error.hpp>
#include <boost/utility.hpp>

#include <boost/net/constants.hpp>
#include <boost/net/detail/move.hpp>
#include <boost/net/detail/operations.hpp>
#include <boost/net/shutdown_mode.hpp>
#include <boost/net/types.hpp>

namespace boost { namespace net {
namespace detail
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
		{}

		impl( int family, int type, int protocol)
		: handle_( invalid_native_handle)
		{
			system::error_code ec;
			handle_ = detail::create_socket( family, type, protocol, ec);
			if ( ec) throw_exception( system::system_error( ec) );
			BOOST_ASSERT( handle_ != invalid_native_handle);
		}

#ifdef BOOST_HAS_RVALUE_REFS
		impl( impl && rhs)
		: handle_( rhs.handle_)
		{ rhs.handle_ = invalid_native_handle; }
#else
		impl( detail::move_type< impl > rhs)
		: handle_( rhs->handle_)
		{ rhs->handle_ = invalid_native_handle; }
#endif

		~impl()
		{
			system::error_code ec;
			detail::close_socket( handle_, ec);
			handle_ = invalid_native_handle;
		}

#ifdef BOOST_HAS_RVALUE_REFS
		impl && move()
		{
			return static_cast< impl && >( * this);
		}
#else
		detail::move_type< impl > move()
		{
			detail::move_type< impl > i( * this);
			return i;
		}
#endif

		native_handle_type native_handle() const
		{ return handle_; }

		bool is_valid() const
		{ return handle_ != invalid_native_handle; }

		void close()
		{
			if ( is_valid() )
			{
				system::error_code ec;
				detail::close_socket( native_handle(), ec);
				handle_ = invalid_native_handle;
				if ( ec) throw_exception( system::system_error( ec) );
			}
		}

		const system::error_code error() const
		{
			BOOST_ASSERT( is_valid() );
	
			int value( 0);
			sockaddr_len_type len( sizeof( int) );
			system::error_code ec;
			net::detail::getsockopt( native_handle(), SOL_SOCKET, SO_ERROR, & value, & len, ec);
			if ( ec) throw_exception( system::system_error( ec) );
			return system::error_code( value, system::get_system_category() );
		}

		bool is_non_blocking() const
		{
			BOOST_ASSERT( is_valid() );

			system::error_code ec;
			bool result( detail::non_blocking( native_handle(), ec) );
			if ( ec) throw_exception( system::system_error( ec) );
			return result;
		}

		void non_blocking( bool mode)
		{
			BOOST_ASSERT( is_valid() );

			system::error_code ec;
			detail::non_blocking( native_handle(), mode, ec);
			if ( ec) throw_exception( system::system_error( ec) );
		}
	};

	shared_ptr< impl >	impl_;

public:
//------------------------------------------
	explicit socket( native_handle_type handle)
	: impl_( new impl( handle) )
	{}

#ifdef BOOST_HAS_RVALUE_REFS
	socket( socket && rhs)
	: impl_()
	{ impl_.reset( new impl( rhs.impl_->move() ) ); }
#else
	socket( detail::move_type< socket > rhs)
	: impl_()
	{ impl_.reset( new impl( rhs->impl_->move() ) ); }
#endif

	socket( int family, int type, int protocol)
	: impl_( new impl( family, type, protocol) )
	{ BOOST_ASSERT( impl_->is_valid() ); }

//------------------------------------------

#ifdef BOOST_HAS_RVALUE_REFS
	socket && move()
	{
		return static_cast< socket && >( * this);
	}
#else
	detail::move_type< socket > move()
	{
		detail::move_type< socket > s( * this);
		return s;
	}
#endif

	native_handle_type native_handle() const
	{ return impl_->native_handle(); }

	bool is_valid() const
	{ return impl_->is_valid(); }

	void close()
	{ impl_->close(); }

	const system::error_code error() const
	{ return impl_->error(); }

	bool is_non_blocking() const
	{ return impl_->is_non_blocking(); }

	void non_blocking( bool value)
	{ impl_->non_blocking( value); }
};
}}}

#endif // BOOST_NET_DETAIL_SOCKET_H
