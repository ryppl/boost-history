//  Copyright (c) 2001-2008 Oliver Kowalke. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_NET_BLOCK_SIGS_H
#define BOOST_NET_BLOCK_SIGS_H

#include <csignal>

#include <boost/preprocessor/arithmetic/inc.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/repetition.hpp>
#include <boost/system/error_code.hpp>
#include <boost/system/system_error.hpp>
#include <boost/utility.hpp>

#include <ogx/net/detail/operations.hpp>
#include <ogx/net/error.hpp>
#include <ogx/net/types.hpp>

namespace boost { namespace net
{
#define BOOST_NET_SIG_ADD_SET_FN(z, n, data)							\
sigaddset( & new_set_, BOOST_PP_CAT( data, n), throw_exception() );

#define BOOST_NET_BLOCK_SIGNALS_CTOR(z, n, data)						\
block_signals( BOOST_PP_ENUM_PARAMS_Z(z, n, int data) )					\
{																		\
	sigset_type new_set_;												\
	system::error_code ec;												\
	detail::sigemptyset( & new_set_, ec);								\
	if ( ec) throw_exception( system::system_error( ec) );				\
	BOOST_PP_REPEAT(													\
		n,																\
		BOOST_NET_SIG_ADD_SET_FN,										\
		data															\
	)																	\
	detail::pthread_sigmask(											\
			SIG_BLOCK,													\
			& new_set_,													\
			& old_set_,													\
			ec);														\
	if ( ec) throw_exception( system::system_error( ec) );				\
}

class block_signals
: private noncopyable
{
private:
	sigset_type old_set_;

public:
	block_signals()
	: old_set_()
	{
		sigset_type new_set_;
		system::error_code ec;
		detail::sigfillset( & new_set_, ec);
		if ( ec) throw_exception( system::system_error( ec) );
		detail::pthread_sigmask(
			SIG_BLOCK,
			& new_set_,
			& old_set_,
			ec);
		if ( ec) throw_exception( system::system_error( ec) );
	}

	BOOST_PP_REPEAT_FROM_TO(
		1,
		BOOST_PP_INC(NSIG),
		BOOST_NET_BLOCK_SIGNALS_CTOR,
		sig
	);

	~block_signals()
	{
		system::error_code ec;
		detail::pthread_sigmask( SIG_SETMASK, & old_set_, 0, ec);
	}
};
#undef BOOST_NET_BLOCK_SIGNALS_CTOR
#undef BOOST_NET_SIG_ADD_SET_FN
} }

#endif // BOOST_NET_BLOCK_SIGS_H
