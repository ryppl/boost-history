//  Copyright (c) 2001-2008 Oliver Kowalke. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_NET_SHUTDOWN_MODE_H
#define BOOST_NET_SHUTDOWN_MODE_H

#include <boost/net/detail/config.hpp>

extern "C"
{
#include <sys/socket.h>
}

namespace boost { namespace net
{
class shutdown_mode;
const shutdown_mode shutdown_rd();
const shutdown_mode shutdown_wr();
const shutdown_mode shutdown_rdwr();

class shutdown_mode
{
private:
	friend const shutdown_mode shutdown_rd();
	friend const shutdown_mode shutdown_wr();
	friend const shutdown_mode shutdown_rdwr();

	int		mode_;

	explicit shutdown_mode( int mode)
	: mode_( mode)
	{}

public:
	operator int () const
	{ return mode_; }
};

inline
const shutdown_mode shutdown_rd()
{ return shutdown_mode( SHUT_RD); }

inline
const shutdown_mode shutdown_wr()
{ return shutdown_mode( SHUT_WR); }

inline
const shutdown_mode shutdown_rdwr()
{ return shutdown_mode( SHUT_RDWR); }
} }

#endif // BOOST_NET_SHUTDOWN_MODE_H
