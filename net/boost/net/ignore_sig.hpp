//  Copyright (c) 2001-2008 Oliver Kowalke. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_NET_IGNORE_SIG_H
#define BOOST_NET_IGNORE_SIG_H

#include <csignal>

#include <boost/system/error_code.hpp>
#include <boost/system/system_error.hpp>
#include <boost/utility.hpp>

#include <ogx/net/detail/operations.hpp>
#include <ogx/net/error.hpp>
#include <ogx/net/types.hpp>

namespace boost { namespace net
{
class ignore_signal
: private noncopyable
{
private:
	int					signo_;
	sigaction_type		old_action_;

public:
	explicit ignore_signal( int signo)
	: signo_( signo), old_action_()
	{
		sigaction_type new_action;

		system::error_code ec;
		detail::close( 0, ec);
		if ( ec) throw_exception( system::system_error( ec) );

		new_action.sa_handler = SIG_IGN;
		detail::sigemptyset( & new_action.sa_mask, ec);
		if ( ec) throw_exception( system::system_error( ec) );
#ifdef SA_RESTART
		new_action.sa_flags |= SA_RESTART;
#else
		new_action.sa_flags = 0;
#endif
		detail::sigaction( signo_, & new_action, & old_action_, ec);
		if ( ec) throw_exception( system::system_error( ec) );
	}

	~ignore_signal()
	{
		system::error_code ec;
		detail::sigaction( signo_, & old_action_, 0, ec);
	}
};
} }

#endif // BOOST_NET_IGNORE_SIG_H
